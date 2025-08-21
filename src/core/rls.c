/*
 * rls.c
 * 
 * Recursive Least Squares (RLS) 모듈 구현
 * 배터리 모델 파라미터 추정을 위한 RLS 구현
 */

#include "rls.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* 상수 정의 */
#define RLS_MIN_LAMBDA        0.8     /* 최소 망각 인자 */
#define RLS_MAX_LAMBDA        1.0     /* 최대 망각 인자 */
#define RLS_MIN_COVARIANCE    1e-6    /* 최소 공분산 값 */
#define RLS_MAX_COVARIANCE    1e6     /* 최대 공분산 값 */

/**
 * @brief RLS 초기화
 */
boolean_T RLS_Initialize(RLS_T* rls, const RLS_Params_T* params, uint32_T num_parameters)
{
    if (rls == NULL || params == NULL || num_parameters == 0) {
        return false;
    }
    
    /* 파라미터 검증 및 설정 */
    rls->params.lambda = (params->lambda < RLS_MIN_LAMBDA) ? RLS_MIN_LAMBDA : 
                         (params->lambda > RLS_MAX_LAMBDA) ? RLS_MAX_LAMBDA : params->lambda;
    
    rls->params.initial_covariance = (params->initial_covariance < RLS_MIN_COVARIANCE) ? RLS_MIN_COVARIANCE :
                                    (params->initial_covariance > RLS_MAX_COVARIANCE) ? RLS_MAX_COVARIANCE : params->initial_covariance;
    
    rls->params.max_parameters = params->max_parameters;
    rls->internal.num_parameters = num_parameters;
    
    /* 메모리 할당 */
    uint32_T matrix_size = num_parameters * num_parameters;
    uint32_T vector_size = num_parameters;
    
    rls->internal.P = (real_T*)malloc(matrix_size * sizeof(real_T));
    rls->internal.theta = (real_T*)malloc(vector_size * sizeof(real_T));
    rls->internal.phi = (real_T*)malloc(vector_size * sizeof(real_T));
    rls->internal.K = (real_T*)malloc(vector_size * sizeof(real_T));
    rls->internal.P_phi = (real_T*)malloc(vector_size * sizeof(real_T));
    rls->internal.phi_transpose = (real_T*)malloc(vector_size * sizeof(real_T));
    
    /* 메모리 할당 실패 확인 */
    if (!rls->internal.P || !rls->internal.theta || !rls->internal.phi ||
        !rls->internal.K || !rls->internal.P_phi || !rls->internal.phi_transpose) {
        RLS_Cleanup(rls);
        return false;
    }
    
    /* 공분산 행렬 초기화 - 단위 행렬 * 초기값 */
    for (uint32_T i = 0; i < matrix_size; i++) {
        rls->internal.P[i] = 0.0;
    }
    for (uint32_T i = 0; i < num_parameters; i++) {
        rls->internal.P[i * num_parameters + i] = rls->params.initial_covariance;
    }
    
    /* 파라미터 벡터 초기화 */
    for (uint32_T i = 0; i < num_parameters; i++) {
        rls->internal.theta[i] = 0.0;
    }
    
    /* 기타 변수 초기화 */
    rls->internal.innovation = 0.0;
    rls->internal.innovation_covariance = 1.0;
    rls->internal.initialized = true;
    
    return true;
}

/**
 * @brief RLS 해제 (메모리 정리)
 */
void RLS_Cleanup(RLS_T* rls)
{
    if (rls == NULL) {
        return;
    }
    
    /* 메모리 해제 */
    if (rls->internal.P) {
        free(rls->internal.P);
        rls->internal.P = NULL;
    }
    if (rls->internal.theta) {
        free(rls->internal.theta);
        rls->internal.theta = NULL;
    }
    if (rls->internal.phi) {
        free(rls->internal.phi);
        rls->internal.phi = NULL;
    }
    if (rls->internal.K) {
        free(rls->internal.K);
        rls->internal.K = NULL;
    }
    if (rls->internal.P_phi) {
        free(rls->internal.P_phi);
        rls->internal.P_phi = NULL;
    }
    if (rls->internal.phi_transpose) {
        free(rls->internal.phi_transpose);
        rls->internal.phi_transpose = NULL;
    }
    
    rls->internal.initialized = false;
}

/**
 * @brief RLS 파라미터 추정 실행
 */
void RLS_Update(RLS_T* rls, const real_T* phi, real_T y)
{
    if (!rls->internal.initialized || phi == NULL) {
        return;
    }
    
    uint32_T n = rls->internal.num_parameters;
    
    /* 회귀 벡터 복사 */
    memcpy(rls->internal.phi, phi, n * sizeof(real_T));
    
    /* P * phi 계산 */
    for (uint32_T i = 0; i < n; i++) {
        rls->internal.P_phi[i] = 0.0;
        for (uint32_T j = 0; j < n; j++) {
            rls->internal.P_phi[i] += rls->internal.P[i * n + j] * phi[j];
        }
    }
    
    /* 혁신 공분산 계산: S = phi^T * P * phi + lambda */
    rls->internal.innovation_covariance = 0.0;
    for (uint32_T i = 0; i < n; i++) {
        rls->internal.innovation_covariance += phi[i] * rls->internal.P_phi[i];
    }
    rls->internal.innovation_covariance += rls->params.lambda;
    
    /* 칼만 게인 계산: K = P * phi / S */
    if (rls->internal.innovation_covariance > 1e-10) {
        real_T S_inv = 1.0 / rls->internal.innovation_covariance;
        for (uint32_T i = 0; i < n; i++) {
            rls->internal.K[i] = rls->internal.P_phi[i] * S_inv;
        }
    } else {
        /* 수치적 안정성을 위해 작은 값으로 설정 */
        for (uint32_T i = 0; i < n; i++) {
            rls->internal.K[i] = 0.0;
        }
    }
    
    /* 혁신 계산: innovation = y - phi^T * theta */
    rls->internal.innovation = y;
    for (uint32_T i = 0; i < n; i++) {
        rls->internal.innovation -= phi[i] * rls->internal.theta[i];
    }
    
    /* 파라미터 업데이트: theta = theta + K * innovation */
    for (uint32_T i = 0; i < n; i++) {
        rls->internal.theta[i] += rls->internal.K[i] * rls->internal.innovation;
    }
    
    /* 공분산 업데이트: P = (P - K * phi^T * P) / lambda */
    /* 먼저 K * phi^T 계산 */
    for (uint32_T i = 0; i < n; i++) {
        for (uint32_T j = 0; j < n; j++) {
            rls->internal.phi_transpose[i * n + j] = rls->internal.K[i] * phi[j];
        }
    }
    
    /* P - K * phi^T * P 계산 */
    real_T* P_new = (real_T*)malloc(n * n * sizeof(real_T));
    if (P_new) {
        for (uint32_T i = 0; i < n; i++) {
            for (uint32_T j = 0; j < n; j++) {
                P_new[i * n + j] = rls->internal.P[i * n + j];
                for (uint32_T k = 0; k < n; k++) {
                    P_new[i * n + j] -= rls->internal.phi_transpose[i * n + k] * rls->internal.P[k * n + j];
                }
            }
        }
        
        /* lambda로 나누기 */
        for (uint32_T i = 0; i < n * n; i++) {
            P_new[i] /= rls->params.lambda;
        }
        
        /* 공분산 행렬 업데이트 */
        memcpy(rls->internal.P, P_new, n * n * sizeof(real_T));
        free(P_new);
    }
    
    /* 공분산 행렬의 대각선 요소가 너무 작아지지 않도록 제한 */
    for (uint32_T i = 0; i < n; i++) {
        if (rls->internal.P[i * n + i] < RLS_MIN_COVARIANCE) {
            rls->internal.P[i * n + i] = RLS_MIN_COVARIANCE;
        }
    }
}

/**
 * @brief 현재 파라미터 값 반환
 */
void RLS_GetParameters(const RLS_T* rls, real_T* theta, uint32_T num_parameters)
{
    if (!rls->internal.initialized || theta == NULL) {
        return;
    }
    
    uint32_T n = (num_parameters < rls->internal.num_parameters) ? num_parameters : rls->internal.num_parameters;
    memcpy(theta, rls->internal.theta, n * sizeof(real_T));
}

/**
 * @brief 특정 파라미터 값 반환
 */
real_T RLS_GetParameter(const RLS_T* rls, uint32_T index)
{
    if (!rls->internal.initialized || index >= rls->internal.num_parameters) {
        return 0.0;
    }
    return rls->internal.theta[index];
}

/**
 * @brief RLS 상태 정보 출력 (디버깅용)
 */
void RLS_PrintStatus(const RLS_T* rls)
{
    if (!rls->internal.initialized) {
        return;
    }
    
    /* 현재는 빈 함수로 구현 - 필요시 printf 등으로 확장 */
    (void)rls; /* 컴파일러 경고 방지 */
}

/**
 * @brief RLS 리셋 (파라미터 초기화)
 */
void RLS_Reset(RLS_T* rls)
{
    if (!rls->internal.initialized) {
        return;
    }
    
    uint32_T n = rls->internal.num_parameters;
    
    /* 파라미터 벡터 초기화 */
    for (uint32_T i = 0; i < n; i++) {
        rls->internal.theta[i] = 0.0;
    }
    
    /* 공분산 행렬 초기화 */
    for (uint32_T i = 0; i < n * n; i++) {
        rls->internal.P[i] = 0.0;
    }
    for (uint32_T i = 0; i < n; i++) {
        rls->internal.P[i * n + i] = rls->params.initial_covariance;
    }
    
    /* 기타 변수 초기화 */
    rls->internal.innovation = 0.0;
    rls->internal.innovation_covariance = 1.0;
}
