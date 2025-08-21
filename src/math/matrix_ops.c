/*
 * matrix_ops.c
 * 
 * 행렬 연산 모듈 구현
 * 2x2, 3x3 행렬 연산을 위한 함수들을 제공
 */

#include "matrix_ops.h"
#include <string.h>
#include <math.h>

/* 상수 정의 */
#define MATRIX_EPSILON       1e-10    /* 수치적 안정성을 위한 작은 값 */

/* 2x2 행렬 연산 구현 */

void Matrix2x2_Multiply(const real_T* A, const real_T* B, real_T* C)
{
    if (A == NULL || B == NULL || C == NULL) {
        return;
    }
    
    /* C[0] = A[0]*B[0] + A[1]*B[2] */
    C[0] = A[0] * B[0] + A[1] * B[2];
    /* C[1] = A[0]*B[1] + A[1]*B[3] */
    C[1] = A[0] * B[1] + A[1] * B[3];
    /* C[2] = A[2]*B[0] + A[3]*B[2] */
    C[2] = A[2] * B[0] + A[3] * B[2];
    /* C[3] = A[2]*B[1] + A[3]*B[3] */
    C[3] = A[2] * B[1] + A[3] * B[3];
}

void Matrix2x2_Transpose(const real_T* A, real_T* B)
{
    if (A == NULL || B == NULL) {
        return;
    }
    
    B[0] = A[0];  /* B[0] = A[0] */
    B[1] = A[2];  /* B[1] = A[2] */
    B[2] = A[1];  /* B[2] = A[1] */
    B[3] = A[3];  /* B[3] = A[3] */
}

void Matrix2x2_Add(const real_T* A, const real_T* B, real_T* C)
{
    if (A == NULL || B == NULL || C == NULL) {
        return;
    }
    
    for (int i = 0; i < MATRIX_2X2_SIZE; i++) {
        C[i] = A[i] + B[i];
    }
}

void Matrix2x2_Subtract(const real_T* A, const real_T* B, real_T* C)
{
    if (A == NULL || B == NULL || C == NULL) {
        return;
    }
    
    for (int i = 0; i < MATRIX_2X2_SIZE; i++) {
        C[i] = A[i] - B[i];
    }
}

real_T Matrix2x2_Determinant(const real_T* A)
{
    if (A == NULL) {
        return 0.0;
    }
    
    /* det(A) = A[0]*A[3] - A[1]*A[2] */
    return A[0] * A[3] - A[1] * A[2];
}

boolean_T Matrix2x2_Inverse(const real_T* A, real_T* B)
{
    if (A == NULL || B == NULL) {
        return false;
    }
    
    real_T det = Matrix2x2_Determinant(A);
    
    /* 행렬식이 0이면 역행렬이 존재하지 않음 */
    if (fabs(det) < MATRIX_EPSILON) {
        return false;
    }
    
    real_T det_inv = 1.0 / det;
    
    /* B = (1/det) * [A[3], -A[1]; -A[2], A[0]] */
    B[0] = A[3] * det_inv;
    B[1] = -A[1] * det_inv;
    B[2] = -A[2] * det_inv;
    B[3] = A[0] * det_inv;
    
    return true;
}

void Matrix2x2_ScalarMultiply(const real_T* A, real_T k, real_T* B)
{
    if (A == NULL || B == NULL) {
        return;
    }
    
    for (int i = 0; i < MATRIX_2X2_SIZE; i++) {
        B[i] = A[i] * k;
    }
}

/* 3x3 행렬 연산 구현 */

void Matrix3x3_Multiply(const real_T* A, const real_T* B, real_T* C)
{
    if (A == NULL || B == NULL || C == NULL) {
        return;
    }
    
    /* C[i,j] = sum(A[i,k] * B[k,j]) for k = 0 to 2 */
    for (int i = 0; i < MATRIX_3X3_DIM; i++) {
        for (int j = 0; j < MATRIX_3X3_DIM; j++) {
            C[i * MATRIX_3X3_DIM + j] = 0.0;
            for (int k = 0; k < MATRIX_3X3_DIM; k++) {
                C[i * MATRIX_3X3_DIM + j] += A[i * MATRIX_3X3_DIM + k] * B[k * MATRIX_3X3_DIM + j];
            }
        }
    }
}

void Matrix3x3_Transpose(const real_T* A, real_T* B)
{
    if (A == NULL || B == NULL) {
        return;
    }
    
    for (int i = 0; i < MATRIX_3X3_DIM; i++) {
        for (int j = 0; j < MATRIX_3X3_DIM; j++) {
            B[j * MATRIX_3X3_DIM + i] = A[i * MATRIX_3X3_DIM + j];
        }
    }
}

void Matrix3x3_Add(const real_T* A, const real_T* B, real_T* C)
{
    if (A == NULL || B == NULL || C == NULL) {
        return;
    }
    
    for (int i = 0; i < MATRIX_3X3_SIZE; i++) {
        C[i] = A[i] + B[i];
    }
}

void Matrix3x3_Subtract(const real_T* A, const real_T* B, real_T* C)
{
    if (A == NULL || B == NULL || C == NULL) {
        return;
    }
    
    for (int i = 0; i < MATRIX_3X3_SIZE; i++) {
        C[i] = A[i] - B[i];
    }
}

real_T Matrix3x3_Determinant(const real_T* A)
{
    if (A == NULL) {
        return 0.0;
    }
    
    /* Sarrus 공식 사용 */
    real_T det = A[0] * A[4] * A[8] + A[1] * A[5] * A[6] + A[2] * A[3] * A[7]
                - A[2] * A[4] * A[6] - A[1] * A[3] * A[8] - A[0] * A[5] * A[7];
    
    return det;
}

boolean_T Matrix3x3_Inverse(const real_T* A, real_T* B)
{
    if (A == NULL || B == NULL) {
        return false;
    }
    
    real_T det = Matrix3x3_Determinant(A);
    
    /* 행렬식이 0이면 역행렬이 존재하지 않음 */
    if (fabs(det) < MATRIX_EPSILON) {
        return false;
    }
    
    real_T det_inv = 1.0 / det;
    
    /* 수반 행렬 계산 후 det로 나누기 */
    B[0] = (A[4] * A[8] - A[5] * A[7]) * det_inv;
    B[1] = (A[2] * A[7] - A[1] * A[8]) * det_inv;
    B[2] = (A[1] * A[5] - A[2] * A[4]) * det_inv;
    
    B[3] = (A[5] * A[6] - A[3] * A[8]) * det_inv;
    B[4] = (A[0] * A[8] - A[2] * A[6]) * det_inv;
    B[5] = (A[2] * A[3] - A[0] * A[5]) * det_inv;
    
    B[6] = (A[3] * A[7] - A[4] * A[6]) * det_inv;
    B[7] = (A[1] * A[6] - A[0] * A[7]) * det_inv;
    B[8] = (A[0] * A[4] - A[1] * A[3]) * det_inv;
    
    return true;
}

void Matrix3x3_ScalarMultiply(const real_T* A, real_T k, real_T* B)
{
    if (A == NULL || B == NULL) {
        return;
    }
    
    for (int i = 0; i < MATRIX_3X3_SIZE; i++) {
        B[i] = A[i] * k;
    }
}

/* 일반 행렬 연산 구현 */

void Matrix_Identity(real_T* A, uint32_T dim)
{
    if (A == NULL || dim == 0) {
        return;
    }
    
    /* 모든 요소를 0으로 초기화 */
    for (uint32_T i = 0; i < dim * dim; i++) {
        A[i] = 0.0;
    }
    
    /* 대각선 요소를 1로 설정 */
    for (uint32_T i = 0; i < dim; i++) {
        A[i * dim + i] = 1.0;
    }
}

void Matrix_Zero(real_T* A, uint32_T dim)
{
    if (A == NULL || dim == 0) {
        return;
    }
    
    for (uint32_T i = 0; i < dim * dim; i++) {
        A[i] = 0.0;
    }
}

void Matrix_Copy(const real_T* A, real_T* B, uint32_T dim)
{
    if (A == NULL || B == NULL || dim == 0) {
        return;
    }
    
    memcpy(B, A, dim * dim * sizeof(real_T));
}

void Matrix_Print(const real_T* A, uint32_T dim, const char* name)
{
    if (A == NULL || name == NULL) {
        return;
    }
    
    /* 현재는 빈 함수로 구현 - 필요시 printf 등으로 확장 */
    (void)A;    /* 컴파일러 경고 방지 */
    (void)dim;  /* 컴파일러 경고 방지 */
    (void)name; /* 컴파일러 경고 방지 */
}
