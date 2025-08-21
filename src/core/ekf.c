/*
 * ekf.c
 * 
 * Extended Kalman Filter (EKF) 모듈 구현
 * 배터리 State of Charge (SoC) 추정을 위한 EKF 구현
 */

#include "ekf.h"
#include <string.h>
#include <math.h>

/* 상수 정의 */
#define EKF_STATE_DIM      2       /* 상태 차원 */
#define EKF_MEASUREMENT_DIM 1      /* 측정 차원 */
#define EKF_MIN_SOC        0.0     /* 최소 SoC */
#define EKF_MAX_SOC        1.0     /* 최대 SoC */
#define EKF_MIN_VOLTAGE_ERROR -1.0 /* 최소 전압 오차 */
#define EKF_MAX_VOLTAGE_ERROR  1.0 /* 최대 전압 오차 */

/* 전역 변수 - 기존 코드와의 호환성을 위해 */
extern real_T look1_binlxpw(real_T u0, const real_T bp0[], const real_T table[],
                           uint32_T maxIndex);

/**
 * @brief EKF 초기화
 */
boolean_T EKF_Initialize(EKF_T* ekf, const EKF_Params_T* params)
{
    if (ekf == NULL || params == NULL) {
        return false;
    }
    
    /* 파라미터 복사 */
    memcpy(&ekf->params, params, sizeof(EKF_Params_T));
    
    /* 상태 초기화 */
    ekf->state.soc = 0.5;  /* 중간값으로 초기화 */
    ekf->state.voltage_error = 0.0;
    
    /* 공분산 행렬 초기화 - 대각선 요소만 초기값 설정 */
    ekf->internal.P[0] = 1.0;  /* SoC 분산 */
    ekf->internal.P[1] = 0.0;  /* SoC-전압오차 공분산 */
    ekf->internal.P[2] = 0.0;  /* 전압오차-SoC 공분산 */
    ekf->internal.P[3] = 1.0;  /* 전압오차 분산 */
    
    /* 상태 전이 행렬 초기화 */
    ekf->internal.F[0] = 1.0;  /* F11 */
    ekf->internal.F[1] = 0.0;  /* F12 */
    ekf->internal.F[2] = 0.0;  /* F21 */
    ekf->internal.F[3] = 1.0;  /* F22 */
    
    /* 측정 행렬 초기화 */
    ekf->internal.H[0] = 1.0;  /* H1 */
    ekf->internal.H[1] = 0.0;  /* H2 */
    
    /* 칼만 게인 초기화 */
    ekf->internal.K[0] = 0.0;
    ekf->internal.K[1] = 0.0;
    
    /* 혁신 관련 변수 초기화 */
    ekf->internal.innovation = 0.0;
    ekf->internal.innovation_covariance = 1.0;
    
    /* 초기화 완료 플래그 설정 */
    ekf->initialized = true;
    
    return true;
}

/**
 * @brief EKF 상태 예측 단계
 */
void EKF_Predict(EKF_T* ekf, real_T current, real_T dt)
{
    if (!ekf->initialized) {
        return;
    }
    
    /* 상태 전이 행렬 업데이트 */
    real_T capacity_factor = dt / (ekf->params.capacity * 3600.0); /* 시간을 시간 단위로 변환 */
    
    /* F[0] = 1, F[1] = -capacity_factor, F[2] = 0, F[3] = 1 */
    ekf->internal.F[1] = -capacity_factor;
    
    /* 상태 예측 */
    real_T soc_pred = ekf->state.soc + ekf->internal.F[1] * current;
    real_T voltage_error_pred = ekf->state.voltage_error;
    
    /* SoC 범위 제한 */
    if (soc_pred > EKF_MAX_SOC) {
        soc_pred = EKF_MAX_SOC;
    } else if (soc_pred < EKF_MIN_SOC) {
        soc_pred = EKF_MIN_SOC;
    }
    
    /* 상태 업데이트 */
    ekf->state.soc = soc_pred;
    ekf->state.voltage_error = voltage_error_pred;
    
    /* 공분산 예측: P = F * P * F^T + Q */
    real_T F_P[4];
    real_T P_pred[4];
    
    /* F * P 계산 */
    F_P[0] = ekf->internal.F[0] * ekf->internal.P[0] + ekf->internal.F[1] * ekf->internal.P[2];
    F_P[1] = ekf->internal.F[0] * ekf->internal.P[1] + ekf->internal.F[1] * ekf->internal.P[3];
    F_P[2] = ekf->internal.F[2] * ekf->internal.P[0] + ekf->internal.F[3] * ekf->internal.P[2];
    F_P[3] = ekf->internal.F[2] * ekf->internal.P[1] + ekf->internal.F[3] * ekf->internal.P[3];
    
    /* (F * P) * F^T 계산 */
    P_pred[0] = F_P[0] * ekf->internal.F[0] + F_P[1] * ekf->internal.F[1];
    P_pred[1] = F_P[0] * ekf->internal.F[2] + F_P[1] * ekf->internal.F[3];
    P_pred[2] = F_P[2] * ekf->internal.F[0] + F_P[3] * ekf->internal.F[1];
    P_pred[3] = F_P[2] * ekf->internal.F[2] + F_P[3] * ekf->internal.F[3];
    
    /* 프로세스 노이즈 추가 */
    P_pred[0] += ekf->params.Q[0];
    P_pred[1] += ekf->params.Q[1];
    P_pred[2] += ekf->params.Q[2];
    P_pred[3] += ekf->params.Q[3];
    
    /* 공분산 업데이트 */
    memcpy(ekf->internal.P, P_pred, sizeof(P_pred));
}

/**
 * @brief EKF 측정 업데이트 단계
 */
void EKF_Update(EKF_T* ekf, real_T voltage_measured, real_T current_measured)
{
    if (!ekf->initialized) {
        return;
    }
    
    /* 측정 예측 */
    real_T voltage_predicted = voltage_measured + ekf->state.voltage_error;
    
    /* 혁신 계산 (Innovation) */
    ekf->internal.innovation = voltage_measured - voltage_predicted;
    
    /* 혁신 공분산 계산: S = H * P * H^T + R */
    real_T H_P[2];
    H_P[0] = ekf->internal.H[0] * ekf->internal.P[0] + ekf->internal.H[1] * ekf->internal.P[1];
    H_P[1] = ekf->internal.H[0] * ekf->internal.P[2] + ekf->internal.H[1] * ekf->internal.P[3];
    
    ekf->internal.innovation_covariance = H_P[0] * ekf->internal.H[0] + H_P[1] * ekf->internal.H[1] + ekf->params.R;
    
    /* 칼만 게인 계산: K = P * H^T * S^(-1) */
    if (ekf->internal.innovation_covariance > 1e-10) {
        real_T S_inv = 1.0 / ekf->internal.innovation_covariance;
        
        ekf->internal.K[0] = (ekf->internal.P[0] * ekf->internal.H[0] + ekf->internal.P[1] * ekf->internal.H[1]) * S_inv;
        ekf->internal.K[1] = (ekf->internal.P[2] * ekf->internal.H[0] + ekf->internal.P[3] * ekf->internal.H[1]) * S_inv;
    }
    
    /* 상태 업데이트: x = x + K * innovation */
    ekf->state.soc += ekf->internal.K[0] * ekf->internal.innovation;
    ekf->state.voltage_error += ekf->internal.K[1] * ekf->internal.innovation;
    
    /* SoC 범위 제한 */
    if (ekf->state.soc > EKF_MAX_SOC) {
        ekf->state.soc = EKF_MAX_SOC;
    } else if (ekf->state.soc < EKF_MIN_SOC) {
        ekf->state.soc = EKF_MIN_SOC;
    }
    
    /* 전압 오차 범위 제한 */
    if (ekf->state.voltage_error > EKF_MAX_VOLTAGE_ERROR) {
        ekf->state.voltage_error = EKF_MAX_VOLTAGE_ERROR;
    } else if (ekf->state.voltage_error < EKF_MIN_VOLTAGE_ERROR) {
        ekf->state.voltage_error = EKF_MIN_VOLTAGE_ERROR;
    }
    
    /* 공분산 업데이트: P = (I - K * H) * P */
    real_T I_KH[4];
    real_T P_update[4];
    
    /* I - K * H 계산 */
    I_KH[0] = 1.0 - ekf->internal.K[0] * ekf->internal.H[0];
    I_KH[1] = 0.0 - ekf->internal.K[0] * ekf->internal.H[1];
    I_KH[2] = 0.0 - ekf->internal.K[1] * ekf->internal.H[0];
    I_KH[3] = 1.0 - ekf->internal.K[1] * ekf->internal.H[1];
    
    /* (I - K * H) * P 계산 */
    P_update[0] = I_KH[0] * ekf->internal.P[0] + I_KH[1] * ekf->internal.P[2];
    P_update[1] = I_KH[0] * ekf->internal.P[1] + I_KH[1] * ekf->internal.P[3];
    P_update[2] = I_KH[2] * ekf->internal.P[0] + I_KH[3] * ekf->internal.P[2];
    P_update[3] = I_KH[2] * ekf->internal.P[1] + I_KH[3] * ekf->internal.P[3];
    
    /* 공분산 업데이트 */
    memcpy(ekf->internal.P, P_update, sizeof(P_update));
}

/**
 * @brief EKF 한 스텝 실행 (예측 + 업데이트)
 */
void EKF_Step(EKF_T* ekf, real_T voltage_measured, real_T current_measured, real_T dt)
{
    if (!ekf->initialized) {
        return;
    }
    
    /* 예측 단계 */
    EKF_Predict(ekf, current_measured, dt);
    
    /* 업데이트 단계 */
    EKF_Update(ekf, voltage_measured, current_measured);
}

/**
 * @brief 현재 SoC 값 반환
 */
real_T EKF_GetSoC(const EKF_T* ekf)
{
    if (ekf == NULL || !ekf->initialized) {
        return 0.0;
    }
    return ekf->state.soc;
}

/**
 * @brief EKF 상태 정보 출력 (디버깅용)
 */
void EKF_PrintStatus(const EKF_T* ekf)
{
    if (ekf == NULL || !ekf->initialized) {
        return;
    }
    
    /* 현재는 빈 함수로 구현 - 필요시 printf 등으로 확장 */
    (void)ekf; /* 컴파일러 경고 방지 */
}
