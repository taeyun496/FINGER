/*
 * main.c
 * 
 * SoC 추정 시스템 메인 모듈
 * 모든 하위 모듈을 통합하여 배터리 State of Charge 추정을 수행
 * 
 * 주요 기능:
 * - EKF, RLS, Lookup Table 모듈 통합
 * - 시스템 초기화 및 실행
 * - 입력/출력 인터페이스 관리
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* 모듈 헤더 포함 */
#include "core/ekf.h"
#include "core/rls.h"
#include "core/lookup_table.h"
#include "math/matrix_ops.h"
#include "math/simd_ops.h"

/* 기존 코드와의 호환성을 위한 헤더 */
#include "../rtwtypes.h"
#include "../SoCesti.h"

/* 시스템 구조체 */
typedef struct {
    EKF_T ekf;                         /* Extended Kalman Filter */
    RLS_T rls;                         /* Recursive Least Squares */
    LookupTable_T ocv_table;           /* OCV Lookup Table */
    LookupTable_T docv_table;          /* dOCV/dSOC Lookup Table */
    
    /* 시스템 상태 */
    real_T current;                    /* 현재 전류 */
    real_T voltage;                    /* 현재 전압 */
    real_T soc;                        /* 현재 SoC */
    real_T temperature;                /* 현재 온도 */
    
    /* 시스템 파라미터 */
    real_T sampling_time;              /* 샘플링 시간 */
    real_T battery_capacity;           /* 배터리 용량 */
    
    /* 초기화 상태 */
    boolean_T initialized;             /* 시스템 초기화 완료 여부 */
} SoC_System_T;

/* 전역 변수 - 기존 코드와의 호환성을 위해 */
DW_SoCesti_T SoCesti_DW;
ExtU_SoCesti_T SoCesti_U;
ExtY_SoCesti_T SoCesti_Y;
static RT_MODEL_SoCesti_T SoCesti_M_;
RT_MODEL_SoCesti_T *const SoCesti_M = &SoCesti_M_;

/* 시스템 인스턴스 */
static SoC_System_T soc_system;

/* 상수 정의 */
#define DEFAULT_SAMPLING_TIME     1.0     /* 기본 샘플링 시간 (초) */
#define DEFAULT_BATTERY_CAPACITY  2.0     /* 기본 배터리 용량 (Ah) */
#define MIN_SOC                   0.0     /* 최소 SoC */
#define MAX_SOC                   1.0     /* 최대 SoC */

/* 함수 선언 */

/**
 * @brief 시스템 초기화
 * @return 초기화 성공 여부
 */
boolean_T SoC_System_Initialize(void);

/**
 * @brief 시스템 해제 (메모리 정리)
 */
void SoC_System_Cleanup(void);

/**
 * @brief 시스템 한 스텝 실행
 * @param current 입력 전류
 * @param voltage 입력 전압
 * @return 출력 SoC
 */
real_T SoC_System_Step(real_T current, real_T voltage);

/**
 * @brief 시스템 상태 출력 (디버깅용)
 */
void SoC_System_PrintStatus(void);

/**
 * @brief 기존 코드와의 호환성을 위한 함수들
 */
void SoCesti_initialize(void);
void SoCesti_step(void);
void SoCesti_terminate(void);

/* 구현 */

boolean_T SoC_System_Initialize(void)
{
    /* 시스템 구조체 초기화 */
    memset(&soc_system, 0, sizeof(SoC_System_T));
    
    /* 기본 파라미터 설정 */
    soc_system.sampling_time = DEFAULT_SAMPLING_TIME;
    soc_system.battery_capacity = DEFAULT_BATTERY_CAPACITY;
    
    /* EKF 초기화 */
    EKF_Params_T ekf_params;
    ekf_params.Q[0] = 1e-9;           /* 프로세스 노이즈 공분산 */
    ekf_params.Q[1] = 0.0;
    ekf_params.Q[2] = 0.0;
    ekf_params.Q[3] = 1.0;
    ekf_params.R = 0.1;                /* 측정 노이즈 분산 */
    ekf_params.dt = soc_system.sampling_time;
    ekf_params.capacity = soc_system.battery_capacity;
    
    if (!EKF_Initialize(&soc_system.ekf, &ekf_params)) {
        printf("EKF 초기화 실패\n");
        return false;
    }
    
    /* RLS 초기화 */
    RLS_Params_T rls_params;
    rls_params.lambda = 0.95;          /* 망각 인자 */
    rls_params.initial_covariance = 1.0;
    rls_params.max_parameters = 3;
    
    if (!RLS_Initialize(&soc_system.rls, &rls_params, 3)) {
        printf("RLS 초기화 실패\n");
        EKF_Cleanup(&soc_system.ekf);
        return false;
    }
    
    /* Lookup Table 초기화 */
    LookupTable_Params_T table_params;
    table_params.max_points = 201;
    table_params.use_binary_search = true;
    table_params.enable_extrapolation = true;
    
    /* OCV 테이블 초기화 */
    if (!LookupTable_Initialize(&soc_system.ocv_table, &table_params,
                               SoCesti_ConstP.pooled5,
                               SoCesti_ConstP.uDLookupTable1_tableData, 201)) {
        printf("OCV Lookup Table 초기화 실패\n");
        RLS_Cleanup(&soc_system.rls);
        EKF_Cleanup(&soc_system.ekf);
        return false;
    }
    
    /* dOCV/dSOC 테이블 초기화 */
    if (!LookupTable_Initialize(&soc_system.docv_table, &table_params,
                               SoCesti_ConstP.pooled5,
                               SoCesti_ConstP.uDLookupTable4_tableData, 201)) {
        printf("dOCV/dSOC Lookup Table 초기화 실패\n");
        LookupTable_Cleanup(&soc_system.ocv_table);
        RLS_Cleanup(&soc_system.rls);
        EKF_Cleanup(&soc_system.ekf);
        return false;
    }
    
    /* 시스템 초기화 완료 */
    soc_system.initialized = true;
    
    printf("SoC 추정 시스템 초기화 완료\n");
    return true;
}

void SoC_System_Cleanup(void)
{
    if (soc_system.initialized) {
        LookupTable_Cleanup(&soc_system.docv_table);
        LookupTable_Cleanup(&soc_system.ocv_table);
        RLS_Cleanup(&soc_system.rls);
        EKF_Cleanup(&soc_system.ekf);
        soc_system.initialized = false;
    }
}

real_T SoC_System_Step(real_T current, real_T voltage)
{
    if (!soc_system.initialized) {
        return 0.0;
    }
    
    /* 입력 값 저장 */
    soc_system.current = current;
    soc_system.voltage = voltage;
    
    /* 현재 SoC 값 가져오기 */
    real_T current_soc = EKF_GetSoC(&soc_system.ekf);
    
    /* OCV 및 dOCV/dSOC 계산 */
    real_T ocv = LookupTable_Interpolate(&soc_system.ocv_table, current_soc);
    real_T docv_dsoc = LookupTable_Interpolate(&soc_system.docv_table, current_soc);
    
    /* 회귀 벡터 구성 (RLS용) */
    real_T phi[3];
    phi[0] = 1.0;                     /* 상수항 */
    phi[1] = current;                  /* 전류 */
    phi[2] = current_soc;              /* SoC */
    
    /* RLS 업데이트 */
    RLS_Update(&soc_system.rls, phi, voltage);
    
    /* EKF 스텝 실행 */
    EKF_Step(&soc_system.ekf, voltage, current, soc_system.sampling_time);
    
    /* 새로운 SoC 값 가져오기 */
    soc_system.soc = EKF_GetSoC(&soc_system.ekf);
    
    /* SoC 범위 제한 */
    if (soc_system.soc > MAX_SOC) {
        soc_system.soc = MAX_SOC;
    } else if (soc_system.soc < MIN_SOC) {
        soc_system.soc = MIN_SOC;
    }
    
    return soc_system.soc;
}

void SoC_System_PrintStatus(void)
{
    if (!soc_system.initialized) {
        printf("시스템이 초기화되지 않았습니다.\n");
        return;
    }
    
    printf("=== SoC 추정 시스템 상태 ===\n");
    printf("전류: %.6f A\n", soc_system.current);
    printf("전압: %.6f V\n", soc_system.voltage);
    printf("SoC: %.6f\n", soc_system.soc);
    printf("온도: %.6f °C\n", soc_system.temperature);
    printf("샘플링 시간: %.6f s\n", soc_system.sampling_time);
    printf("배터리 용량: %.6f Ah\n", soc_system.battery_capacity);
    printf("==========================\n");
}

/* 기존 코드와의 호환성을 위한 함수들 */

void SoCesti_initialize(void)
{
    /* 시스템 초기화 */
    if (!SoC_System_Initialize()) {
        printf("SoC 시스템 초기화 실패\n");
        return;
    }
    
    /* 기존 코드와의 호환성을 위한 초기화 */
    memset(&SoCesti_M_, 0, sizeof(RT_MODEL_SoCesti_T));
    rtmSetTFinal(SoCesti_M, 37660.0);
    SoCesti_M->Timing.stepSize0 = 1.0;
    
    /* 기타 초기화 코드는 기존과 동일하게 유지 */
    memset(&SoCesti_DW, 0, sizeof(DW_SoCesti_T));
    memset(&SoCesti_U, 0, sizeof(ExtU_SoCesti_T));
    SoCesti_Y.SoC = 0.0;
    
    printf("SoCesti 초기화 완료\n");
}

void SoCesti_step(void)
{
    /* 기존 입력 값 사용 */
    real_T current = SoCesti_U.current;
    real_T voltage = SoCesti_U.voltage;
    
    /* 새로운 시스템으로 SoC 계산 */
    real_T soc = SoCesti_Y.SoC;
    
    if (soc_system.initialized) {
        soc = SoC_System_Step(current, voltage);
    }
    
    /* 출력 업데이트 */
    SoCesti_Y.SoC = soc;
    
    /* 기존 코드와의 호환성을 위한 상태 업데이트 */
    SoCesti_DW.Delay_DSTATE_k[0] = soc;
    
    /* 기타 필요한 상태 업데이트는 기존 코드와 동일하게 유지 */
}

void SoCesti_terminate(void)
{
    /* 시스템 정리 */
    SoC_System_Cleanup();
    printf("SoCesti 종료\n");
}

/* 메인 함수 (테스트용) */
#ifdef TEST_MODE
int main(void)
{
    printf("SoC 추정 시스템 테스트 시작\n");
    
    /* 시스템 초기화 */
    if (!SoC_System_Initialize()) {
        printf("시스템 초기화 실패\n");
        return -1;
    }
    
    /* 테스트 데이터로 시스템 실행 */
    real_T test_current[] = {1.0, 0.5, -0.5, -1.0};
    real_T test_voltage[] = {4.0, 3.8, 3.6, 3.4};
    
    for (int i = 0; i < 4; i++) {
        real_T soc = SoC_System_Step(test_current[i], test_voltage[i]);
        printf("Step %d: Current=%.1fA, Voltage=%.1fV, SoC=%.6f\n", 
               i, test_current[i], test_voltage[i], soc);
    }
    
    /* 시스템 상태 출력 */
    SoC_System_PrintStatus();
    
    /* 시스템 정리 */
    SoC_System_Cleanup();
    
    printf("테스트 완료\n");
    return 0;
}
#endif
