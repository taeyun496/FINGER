/*
 * simd_ops.h
 * 
 * SIMD (Single Instruction Multiple Data) 최적화 모듈
 * Intel SSE2 명령어를 사용한 벡터 연산 최적화
 * 
 * 주요 기능:
 * - 벡터 덧셈/뺄셈 (Vector Addition/Subtraction)
 * - 벡터 곱셈/나눗셈 (Vector Multiplication/Division)
 * - 벡터 로드/저장 (Vector Load/Store)
 * - 플랫폼 독립적인 SIMD 구현
 */

#ifndef SIMD_OPS_H
#define SIMD_OPS_H

#include "rtwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* SIMD 지원 여부 확인 */
#define SIMD_SUPPORT_SSE2    1       /* SSE2 지원 여부 */
#define SIMD_VECTOR_SIZE     2       /* double 타입 벡터 크기 (SSE2) */

/* 함수 선언 */

/**
 * @brief SIMD 지원 여부 확인
 * @return SIMD 지원 여부
 */
boolean_T SIMD_IsSupported(void);

/**
 * @brief SIMD 벡터 덧셈: C = A + B (2개 double)
 * @param A 입력 벡터 A
 * @param B 입력 벡터 B
 * @param C 출력 벡터 C
 */
void SIMD_VectorAdd2(const real_T* A, const real_T* B, real_T* C);

/**
 * @brief SIMD 벡터 뺄셈: C = A - B (2개 double)
 * @param A 입력 벡터 A
 * @param B 입력 벡터 B
 * @param C 출력 벡터 C
 */
void SIMD_VectorSubtract2(const real_T* A, const real_T* B, real_T* C);

/**
 * @brief SIMD 벡터 곱셈: C = A * B (2개 double)
 * @param A 입력 벡터 A
 * @param B 입력 벡터 B
 * @param C 출력 벡터 C
 */
void SIMD_VectorMultiply2(const real_T* A, const real_T* B, real_T* C);

/**
 * @brief SIMD 벡터 나눗셈: C = A / B (2개 double)
 * @param A 입력 벡터 A
 * @param B 입력 벡터 B
 * @param C 출력 벡터 C
 */
void SIMD_VectorDivide2(const real_T* A, const real_T* B, real_T* C);

/**
 * @brief SIMD 벡터를 스칼라로 곱하기: C = k * A (2개 double)
 * @param A 입력 벡터 A
 * @param k 스칼라 값
 * @param C 출력 벡터 C
 */
void SIMD_VectorScalarMultiply2(const real_T* A, real_T k, real_T* C);

/**
 * @brief SIMD 벡터 덧셈: C = A + B (4개 double)
 * @param A 입력 벡터 A
 * @param B 입력 벡터 B
 * @param C 출력 벡터 C
 */
void SIMD_VectorAdd4(const real_T* A, const real_T* B, real_T* C);

/**
 * @brief SIMD 벡터 뺄셈: C = A - B (4개 double)
 * @param A 입력 벡터 A
 * @param B 입력 벡터 B
 * @param C 출력 벡터 C
 */
void SIMD_VectorSubtract4(const real_T* A, const real_T* B, real_T* C);

/**
 * @brief SIMD 벡터 곱셈: C = A * B (4개 double)
 * @param A 입력 벡터 A
 * @param B 입력 벡터 B
 * @param C 출력 벡터 C
 */
void SIMD_VectorMultiply4(const real_T* A, const real_T* B, real_T* C);

/**
 * @brief SIMD 벡터를 스칼라로 곱하기: C = k * A (4개 double)
 * @param A 입력 벡터 A
 * @param k 스칼라 값
 * @param C 출력 벡터 C
 */
void SIMD_VectorScalarMultiply4(const real_T* A, real_T k, real_T* C);

/**
 * @brief SIMD 벡터 덧셈: C = A + B (8개 double)
 * @param A 입력 벡터 A
 * @param B 입력 벡터 B
 * @param C 출력 벡터 C
 */
void SIMD_VectorAdd8(const real_T* A, const real_T* B, real_T* C);

/**
 * @brief SIMD 벡터 뺄셈: C = A - B (8개 double)
 * @param A 입력 벡터 A
 * @param B 입력 벡터 B
 * @param C 출력 벡터 C
 */
void SIMD_VectorSubtract8(const real_T* A, const real_T* B, real_T* C);

/**
 * @brief SIMD 벡터를 스칼라로 곱하기: C = k * A (8개 double)
 * @param A 입력 벡터 A
 * @param k 스칼라 값
 * @param C 출력 벡터 C
 */
void SIMD_VectorScalarMultiply8(const real_T* A, real_T k, real_T* C);

/**
 * @brief SIMD 벡터 내적 계산 (2개 double)
 * @param A 입력 벡터 A
 * @param B 입력 벡터 B
 * @return 내적 값
 */
real_T SIMD_VectorDotProduct2(const real_T* A, const real_T* B);

/**
 * @brief SIMD 벡터 내적 계산 (4개 double)
 * @param A 입력 벡터 A
 * @param B 입력 벡터 B
 * @return 내적 값
 */
real_T SIMD_VectorDotProduct4(const real_T* A, const real_T* B);

/**
 * @brief SIMD 벡터 내적 계산 (8개 double)
 * @param A 입력 벡터 A
 * @param B 입력 벡터 B
 * @return 내적 값
 */
real_T SIMD_VectorDotProduct8(const real_T* A, const real_T* B);

/**
 * @brief SIMD 벡터 최대값 찾기 (2개 double)
 * @param A 입력 벡터 A
 * @return 최대값
 */
real_T SIMD_VectorMax2(const real_T* A);

/**
 * @brief SIMD 벡터 최소값 찾기 (2개 double)
 * @param A 입력 벡터 A
 * @return 최소값
 */
real_T SIMD_VectorMin2(const real_T* A);

/**
 * @brief SIMD 벡터 절댓값: B = |A| (2개 double)
 * @param A 입력 벡터 A
 * @param B 출력 벡터 B
 */
void SIMD_VectorAbs2(const real_T* A, real_T* B);

/**
 * @brief SIMD 벡터 제곱근: B = sqrt(A) (2개 double)
 * @param A 입력 벡터 A
 * @param B 출력 벡터 B
 */
void SIMD_VectorSqrt2(const real_T* A, real_T* B);

#ifdef __cplusplus
}
#endif

#endif /* SIMD_OPS_H */
