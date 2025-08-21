/*
 * rls.h
 * 
 * Recursive Least Squares (RLS) 모듈
 * 배터리 모델 파라미터 추정을 위한 RLS 구현
 * 
 * 주요 기능:
 * - 회귀 벡터 구성 (Regression Vector)
 * - 칼만 게인 계산 (Kalman Gain)
 * - 파라미터 업데이트 (Parameter Update)
 * - 공분산 업데이트 (Covariance Update)
 */

#ifndef RLS_H
#define RLS_H

#include "rtwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* RLS 파라미터 구조체 */
typedef struct {
    real_T lambda;                  /* 망각 인자 (Forgetting Factor) */
    real_T initial_covariance;      /* 초기 공분산 값 */
    uint32_T max_parameters;        /* 최대 파라미터 개수 */
} RLS_Params_T;

/* RLS 내부 상태 구조체 */
typedef struct {
    real_T* P;                      /* 공분산 행렬 (동적 할당) */
    real_T* theta;                  /* 파라미터 벡터 */
    real_T* phi;                    /* 회귀 벡터 */
    real_T* K;                      /* 칼만 게인 */
    real_T* P_phi;                  /* P * phi 계산 결과 */
    real_T* phi_transpose;          /* phi 전치 */
    real_T innovation;              /* 혁신 (Innovation) */
    real_T innovation_covariance;   /* 혁신 공분산 */
    uint32_T num_parameters;        /* 실제 파라미터 개수 */
    boolean_T initialized;          /* 초기화 완료 플래그 */
} RLS_Internal_T;

/* RLS 전체 구조체 */
typedef struct {
    RLS_Params_T params;            /* RLS 파라미터 */
    RLS_Internal_T internal;        /* 내부 상태 */
} RLS_T;

/* 함수 선언 */

/**
 * @brief RLS 초기화
 * @param rls RLS 구조체 포인터
 * @param params RLS 파라미터
 * @param num_parameters 파라미터 개수
 * @return 초기화 성공 여부
 */
boolean_T RLS_Initialize(RLS_T* rls, const RLS_Params_T* params, uint32_T num_parameters);

/**
 * @brief RLS 해제 (메모리 정리)
 * @param rls RLS 구조체 포인터
 */
void RLS_Cleanup(RLS_T* rls);

/**
 * @brief RLS 파라미터 추정 실행
 * @param rls RLS 구조체 포인터
 * @param phi 회귀 벡터
 * @param y 측정값
 */
void RLS_Update(RLS_T* rls, const real_T* phi, real_T y);

/**
 * @brief 현재 파라미터 값 반환
 * @param rls RLS 구조체 포인터
 * @param theta 출력 파라미터 벡터
 * @param num_parameters 파라미터 개수
 */
void RLS_GetParameters(const RLS_T* rls, real_T* theta, uint32_T num_parameters);

/**
 * @brief 특정 파라미터 값 반환
 * @param rls RLS 구조체 포인터
 * @param index 파라미터 인덱스
 * @return 파라미터 값
 */
real_T RLS_GetParameter(const RLS_T* rls, uint32_T index);

/**
 * @brief RLS 상태 정보 출력 (디버깅용)
 * @param rls RLS 구조체 포인터
 */
void RLS_PrintStatus(const RLS_T* rls);

/**
 * @brief RLS 리셋 (파라미터 초기화)
 * @param rls RLS 구조체 포인터
 */
void RLS_Reset(RLS_T* rls);

#ifdef __cplusplus
}
#endif

#endif /* RLS_H */
