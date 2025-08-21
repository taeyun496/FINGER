/*
 * matrix_ops.h
 * 
 * 행렬 연산 모듈
 * 2x2, 3x3 행렬 연산을 위한 함수들을 제공
 * 
 * 주요 기능:
 * - 행렬 곱셈 (Matrix Multiplication)
 * - 행렬 전치 (Matrix Transpose)
 * - 행렬 덧셈/뺄셈 (Matrix Addition/Subtraction)
 * - 행렬 역행렬 (Matrix Inverse)
 * - 행렬식 계산 (Determinant)
 */

#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

#include "rtwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 행렬 크기 상수 */
#define MATRIX_2X2_SIZE     4       /* 2x2 행렬의 요소 개수 */
#define MATRIX_3X3_SIZE     9       /* 3x3 행렬의 요소 개수 */
#define MATRIX_2X2_DIM      2       /* 2x2 행렬의 차원 */
#define MATRIX_3X3_DIM      3       /* 3x3 행렬의 차원 */

/* 함수 선언 */

/* 2x2 행렬 연산 */

/**
 * @brief 2x2 행렬 곱셈: C = A * B
 * @param A 입력 행렬 A (2x2)
 * @param B 입력 행렬 B (2x2)
 * @param C 출력 행렬 C (2x2)
 */
void Matrix2x2_Multiply(const real_T* A, const real_T* B, real_T* C);

/**
 * @brief 2x2 행렬 전치: B = A^T
 * @param A 입력 행렬 A (2x2)
 * @param B 출력 행렬 B (2x2)
 */
void Matrix2x2_Transpose(const real_T* A, real_T* B);

/**
 * @brief 2x2 행렬 덧셈: C = A + B
 * @param A 입력 행렬 A (2x2)
 * @param B 입력 행렬 B (2x2)
 * @param C 출력 행렬 C (2x2)
 */
void Matrix2x2_Add(const real_T* A, const real_T* B, real_T* C);

/**
 * @brief 2x2 행렬 뺄셈: C = A - B
 * @param A 입력 행렬 A (2x2)
 * @param B 입력 행렬 B (2x2)
 * @param C 출력 행렬 C (2x2)
 */
void Matrix2x2_Subtract(const real_T* A, const real_T* B, real_T* C);

/**
 * @brief 2x2 행렬 역행렬: B = A^(-1)
 * @param A 입력 행렬 A (2x2)
 * @param B 출력 행렬 B (2x2)
 * @return 성공 여부 (행렬식이 0이면 실패)
 */
boolean_T Matrix2x2_Inverse(const real_T* A, real_T* B);

/**
 * @brief 2x2 행렬식 계산
 * @param A 입력 행렬 A (2x2)
 * @return 행렬식 값
 */
real_T Matrix2x2_Determinant(const real_T* A);

/**
 * @brief 2x2 행렬을 스칼라로 곱하기: B = k * A
 * @param A 입력 행렬 A (2x2)
 * @param k 스칼라 값
 * @param B 출력 행렬 B (2x2)
 */
void Matrix2x2_ScalarMultiply(const real_T* A, real_T k, real_T* B);

/* 3x3 행렬 연산 */

/**
 * @brief 3x3 행렬 곱셈: C = A * B
 * @param A 입력 행렬 A (3x3)
 * @param B 입력 행렬 B (3x3)
 * @param C 출력 행렬 C (3x3)
 */
void Matrix3x3_Multiply(const real_T* A, const real_T* B, real_T* C);

/**
 * @brief 3x3 행렬 전치: B = A^T
 * @param A 입력 행렬 A (3x3)
 * @param B 출력 행렬 B (3x3)
 */
void Matrix3x3_Transpose(const real_T* A, real_T* B);

/**
 * @brief 3x3 행렬 덧셈: C = A + B
 * @param A 입력 행렬 A (3x3)
 * @param B 입력 행렬 B (3x3)
 * @param C 출력 행렬 C (3x3)
 */
void Matrix3x3_Add(const real_T* A, const real_T* B, real_T* C);

/**
 * @brief 3x3 행렬 뺄셈: C = A - B
 * @param A 입력 행렬 A (3x3)
 * @param B 입력 행렬 B (3x3)
 * @param C 출력 행렬 C (3x3)
 */
void Matrix3x3_Subtract(const real_T* A, const real_T* B, real_T* C);

/**
 * @brief 3x3 행렬 역행렬: B = A^(-1)
 * @param A 입력 행렬 A (3x3)
 * @param B 출력 행렬 B (3x3)
 * @return 성공 여부 (행렬식이 0이면 실패)
 */
boolean_T Matrix3x3_Inverse(const real_T* A, real_T* B);

/**
 * @brief 3x3 행렬식 계산
 * @param A 입력 행렬 A (3x3)
 * @return 행렬식 값
 */
real_T Matrix3x3_Determinant(const real_T* A);

/**
 * @brief 3x3 행렬을 스칼라로 곱하기: B = k * A
 * @param A 입력 행렬 A (3x3)
 * @param k 스칼라 값
 * @param B 출력 행렬 B (3x3)
 */
void Matrix3x3_ScalarMultiply(const real_T* A, real_T k, real_T* B);

/* 일반 행렬 연산 */

/**
 * @brief 행렬을 단위 행렬로 초기화
 * @param A 출력 행렬
 * @param dim 행렬 차원
 */
void Matrix_Identity(real_T* A, uint32_T dim);

/**
 * @brief 행렬을 0으로 초기화
 * @param A 출력 행렬
 * @param dim 행렬 차원
 */
void Matrix_Zero(real_T* A, uint32_T dim);

/**
 * @brief 행렬 복사: B = A
 * @param A 입력 행렬
 * @param B 출력 행렬
 * @param dim 행렬 차원
 */
void Matrix_Copy(const real_T* A, real_T* B, uint32_T dim);

/**
 * @brief 행렬 출력 (디버깅용)
 * @param A 출력할 행렬
 * @param dim 행렬 차원
 * @param name 행렬 이름
 */
void Matrix_Print(const real_T* A, uint32_T dim, const char* name);

#ifdef __cplusplus
}
#endif

#endif /* MATRIX_OPS_H */
