/*
 * ekf.h
 * 
 * Extended Kalman Filter (EKF) 모듈
 * 배터리 State of Charge (SoC) 추정을 위한 EKF 구현
 * 
 * 주요 기능:
 * - 상태 예측 (State Prediction)
 * - 칼만 게인 계산 (Kalman Gain Calculation)
 * - 상태 업데이트 (State Update)
 * - 공분산 업데이트 (Covariance Update)
 */

#ifndef EKF_H
#define EKF_H

#include "rtwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* EKF 상태 구조체 */
typedef struct {
    real_T soc;                    /* State of Charge (0.0 ~ 1.0) */
    real_T voltage_error;          /* 전압 예측 오차 */
} EKF_State_T;

/* EKF 파라미터 구조체 */
typedef struct {
    real_T Q[4];                   /* 프로세스 노이즈 공분산 행렬 (2x2) */
    real_T R;                      /* 측정 노이즈 분산 */
    real_T dt;                     /* 샘플링 시간 */
    real_T capacity;               /* 배터리 용량 (Ah) */
} EKF_Params_T;

/* EKF 내부 상태 구조체 */
typedef struct {
    real_T P[4];                   /* 상태 추정 오차 공분산 행렬 (2x2) */
    real_T F[4];                   /* 상태 전이 행렬 (2x2) */
    real_T H[2];                   /* 측정 행렬 (1x2) */
    real_T K[2];                   /* 칼만 게인 (2x1) */
    real_T innovation;             /* 혁신 (Innovation) */
    real_T innovation_covariance;  /* 혁신 공분산 */
} EKF_Internal_T;

/* EKF 전체 구조체 */
typedef struct {
    EKF_State_T state;             /* 현재 상태 */
    EKF_Params_T params;           /* EKF 파라미터 */
    EKF_Internal_T internal;       /* 내부 계산 변수 */
    boolean_T initialized;         /* 초기화 완료 플래그 */
} EKF_T;

/* 함수 선언 */

/**
 * @brief EKF 초기화
 * @param ekf EKF 구조체 포인터
 * @param params EKF 파라미터
 * @return 초기화 성공 여부
 */
boolean_T EKF_Initialize(EKF_T* ekf, const EKF_Params_T* params);

/**
 * @brief EKF 상태 예측 단계
 * @param ekf EKF 구조체 포인터
 * @param current 배터리 전류 (A)
 * @param dt 샘플링 시간 (s)
 */
void EKF_Predict(EKF_T* ekf, real_T current, real_T dt);

/**
 * @brief EKF 측정 업데이트 단계
 * @param ekf EKF 구조체 포인터
 * @param voltage_measured 측정된 전압 (V)
 * @param current_measured 측정된 전류 (A)
 */
void EKF_Update(EKF_T* ekf, real_T voltage_measured, real_T current_measured);

/**
 * @brief EKF 한 스텝 실행 (예측 + 업데이트)
 * @param ekf EKF 구조체 포인터
 * @param voltage_measured 측정된 전압 (V)
 * @param current_measured 측정된 전류 (A)
 * @param dt 샘플링 시간 (s)
 */
void EKF_Step(EKF_T* ekf, real_T voltage_measured, real_T current_measured, real_T dt);

/**
 * @brief 현재 SoC 값 반환
 * @param ekf EKF 구조체 포인터
 * @return 현재 SoC 값 (0.0 ~ 1.0)
 */
real_T EKF_GetSoC(const EKF_T* ekf);

/**
 * @brief EKF 상태 정보 출력 (디버깅용)
 * @param ekf EKF 구조체 포인터
 */
void EKF_PrintStatus(const EKF_T* ekf);

#ifdef __cplusplus
}
#endif

#endif /* EKF_H */
