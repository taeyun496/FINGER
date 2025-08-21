/*
 * simd_ops.c
 * 
 * SIMD (Single Instruction Multiple Data) 최적화 모듈 구현
 * Intel SSE2 명령어를 사용한 벡터 연산 최적화
 */

#include "simd_ops.h"
#include <string.h>
#include <math.h>

/* SSE2 헤더 포함 */
#ifdef _MSC_VER
    #include <intrin.h>
#else
    #include <emmintrin.h>
#endif

/* 상수 정의 */
#define SIMD_ALIGNMENT      16      /* SIMD 정렬 요구사항 */

/* SIMD 지원 여부 확인 */
static boolean_T simd_supported = false;
static boolean_T simd_checked = false;

/**
 * @brief SIMD 지원 여부 확인
 */
boolean_T SIMD_IsSupported(void)
{
    if (simd_checked) {
        return simd_supported;
    }
    
    /* 간단한 SSE2 지원 확인 - 실제로는 CPUID 등을 사용해야 함 */
    #ifdef _MSC_VER
        /* Windows MSVC */
        simd_supported = true;
    #else
        /* GCC/Clang */
        simd_supported = true;
    #endif
    
    simd_checked = true;
    return simd_supported;
}

/* 2개 double 벡터 연산 */

void SIMD_VectorAdd2(const real_T* A, const real_T* B, real_T* C)
{
    if (!SIMD_IsSupported() || A == NULL || B == NULL || C == NULL) {
        /* SIMD가 지원되지 않으면 일반 연산 사용 */
        if (A && B && C) {
            C[0] = A[0] + B[0];
            C[1] = A[1] + B[1];
        }
        return;
    }
    
    #ifdef _MSC_VER
        /* Windows MSVC */
        __m128d a = _mm_loadu_pd(A);
        __m128d b = _mm_loadu_pd(B);
        __m128d c = _mm_add_pd(a, b);
        _mm_storeu_pd(C, c);
    #else
        /* GCC/Clang */
        __m128d a = _mm_loadu_pd(A);
        __m128d b = _mm_loadu_pd(B);
        __m128d c = _mm_add_pd(a, b);
        _mm_storeu_pd(C, c);
    #endif
}

void SIMD_VectorSubtract2(const real_T* A, const real_T* B, real_T* C)
{
    if (!SIMD_IsSupported() || A == NULL || B == NULL || C == NULL) {
        if (A && B && C) {
            C[0] = A[0] - B[0];
            C[1] = A[1] - B[1];
        }
        return;
    }
    
    #ifdef _MSC_VER
        __m128d a = _mm_loadu_pd(A);
        __m128d b = _mm_loadu_pd(B);
        __m128d c = _mm_sub_pd(a, b);
        _mm_storeu_pd(C, c);
    #else
        __m128d a = _mm_loadu_pd(A);
        __m128d b = _mm_loadu_pd(B);
        __m128d c = _mm_sub_pd(a, b);
        _mm_storeu_pd(C, c);
    #endif
}

void SIMD_VectorMultiply2(const real_T* A, const real_T* B, real_T* C)
{
    if (!SIMD_IsSupported() || A == NULL || B == NULL || C == NULL) {
        if (A && B && C) {
            C[0] = A[0] * B[0];
            C[1] = A[1] * B[1];
        }
        return;
    }
    
    #ifdef _MSC_VER
        __m128d a = _mm_loadu_pd(A);
        __m128d b = _mm_loadu_pd(B);
        __m128d c = _mm_mul_pd(a, b);
        _mm_storeu_pd(C, c);
    #else
        __m128d a = _mm_loadu_pd(A);
        __m128d b = _mm_loadu_pd(B);
        __m128d c = _mm_mul_pd(a, b);
        _mm_storeu_pd(C, c);
    #endif
}

void SIMD_VectorDivide2(const real_T* A, const real_T* B, real_T* C)
{
    if (!SIMD_IsSupported() || A == NULL || B == NULL || C == NULL) {
        if (A && B && C) {
            C[0] = (B[0] != 0.0) ? A[0] / B[0] : 0.0;
            C[1] = (B[1] != 0.0) ? A[1] / B[1] : 0.0;
        }
        return;
    }
    
    #ifdef _MSC_VER
        __m128d a = _mm_loadu_pd(A);
        __m128d b = _mm_loadu_pd(B);
        __m128d c = _mm_div_pd(a, b);
        _mm_storeu_pd(C, c);
    #else
        __m128d a = _mm_loadu_pd(A);
        __m128d b = _mm_loadu_pd(B);
        __m128d c = _mm_div_pd(a, b);
        _mm_storeu_pd(C, c);
    #endif
}

void SIMD_VectorScalarMultiply2(const real_T* A, real_T k, real_T* C)
{
    if (!SIMD_IsSupported() || A == NULL || C == NULL) {
        if (A && C) {
            C[0] = A[0] * k;
            C[1] = A[1] * k;
        }
        return;
    }
    
    #ifdef _MSC_VER
        __m128d a = _mm_loadu_pd(A);
        __m128d scalar = _mm_set1_pd(k);
        __m128d c = _mm_mul_pd(a, scalar);
        _mm_storeu_pd(C, c);
    #else
        __m128d a = _mm_loadu_pd(A);
        __m128d scalar = _mm_set1_pd(k);
        __m128d c = _mm_mul_pd(a, scalar);
        _mm_storeu_pd(C, c);
    #endif
}

/* 4개 double 벡터 연산 */

void SIMD_VectorAdd4(const real_T* A, const real_T* B, real_T* C)
{
    if (!SIMD_IsSupported() || A == NULL || B == NULL || C == NULL) {
        if (A && B && C) {
            for (int i = 0; i < 4; i++) {
                C[i] = A[i] + B[i];
            }
        }
        return;
    }
    
    /* 2개씩 나누어서 처리 */
    SIMD_VectorAdd2(A, B, C);
    SIMD_VectorAdd2(A + 2, B + 2, C + 2);
}

void SIMD_VectorSubtract4(const real_T* A, const real_T* B, real_T* C)
{
    if (!SIMD_IsSupported() || A == NULL || B == NULL || C == NULL) {
        if (A && B && C) {
            for (int i = 0; i < 4; i++) {
                C[i] = A[i] - B[i];
            }
        }
        return;
    }
    
    SIMD_VectorSubtract2(A, B, C);
    SIMD_VectorSubtract2(A + 2, B + 2, C + 2);
}

void SIMD_VectorMultiply4(const real_T* A, const real_T* B, real_T* C)
{
    if (!SIMD_IsSupported() || A == NULL || B == NULL || C == NULL) {
        if (A && B && C) {
            for (int i = 0; i < 4; i++) {
                C[i] = A[i] * B[i];
            }
        }
        return;
    }
    
    SIMD_VectorMultiply2(A, B, C);
    SIMD_VectorMultiply2(A + 2, B + 2, C + 2);
}

void SIMD_VectorScalarMultiply4(const real_T* A, real_T k, real_T* C)
{
    if (!SIMD_IsSupported() || A == NULL || C == NULL) {
        if (A && C) {
            for (int i = 0; i < 4; i++) {
                C[i] = A[i] * k;
            }
        }
        return;
    }
    
    SIMD_VectorScalarMultiply2(A, k, C);
    SIMD_VectorScalarMultiply2(A + 2, k, C + 2);
}

/* 8개 double 벡터 연산 */

void SIMD_VectorAdd8(const real_T* A, const real_T* B, real_T* C)
{
    if (!SIMD_IsSupported() || A == NULL || B == NULL || C == NULL) {
        if (A && B && C) {
            for (int i = 0; i < 8; i++) {
                C[i] = A[i] + B[i];
            }
        }
        return;
    }
    
    /* 4개씩 나누어서 처리 */
    SIMD_VectorAdd4(A, B, C);
    SIMD_VectorAdd4(A + 4, B + 4, C + 4);
}

void SIMD_VectorSubtract8(const real_T* A, const real_T* B, real_T* C)
{
    if (!SIMD_IsSupported() || A == NULL || B == NULL || C == NULL) {
        if (A && B && C) {
            for (int i = 0; i < 8; i++) {
                C[i] = A[i] - B[i];
            }
        }
        return;
    }
    
    SIMD_VectorSubtract4(A, B, C);
    SIMD_VectorSubtract4(A + 4, B + 4, C + 4);
}

void SIMD_VectorScalarMultiply8(const real_T* A, real_T k, real_T* C)
{
    if (!SIMD_IsSupported() || A == NULL || C == NULL) {
        if (A && C) {
            for (int i = 0; i < 8; i++) {
                C[i] = A[i] * k;
            }
        }
        return;
    }
    
    SIMD_VectorScalarMultiply4(A, k, C);
    SIMD_VectorScalarMultiply4(A + 4, k, C + 4);
}

/* 벡터 내적 계산 */

real_T SIMD_VectorDotProduct2(const real_T* A, const real_T* B)
{
    if (!SIMD_IsSupported() || A == NULL || B == NULL) {
        if (A && B) {
            return A[0] * B[0] + A[1] * B[1];
        }
        return 0.0;
    }
    
    #ifdef _MSC_VER
        __m128d a = _mm_loadu_pd(A);
        __m128d b = _mm_loadu_pd(B);
        __m128d mul = _mm_mul_pd(a, b);
        real_T result[2];
        _mm_storeu_pd(result, mul);
        return result[0] + result[1];
    #else
        __m128d a = _mm_loadu_pd(A);
        __m128d b = _mm_loadu_pd(B);
        __m128d mul = _mm_mul_pd(a, b);
        real_T result[2];
        _mm_storeu_pd(result, mul);
        return result[0] + result[1];
    #endif
}

real_T SIMD_VectorDotProduct4(const real_T* A, const real_T* B)
{
    if (!SIMD_IsSupported() || A == NULL || B == NULL) {
        if (A && B) {
            real_T sum = 0.0;
            for (int i = 0; i < 4; i++) {
                sum += A[i] * B[i];
            }
            return sum;
        }
        return 0.0;
    }
    
    return SIMD_VectorDotProduct2(A, B) + SIMD_VectorDotProduct2(A + 2, B + 2);
}

real_T SIMD_VectorDotProduct8(const real_T* A, const real_T* B)
{
    if (!SIMD_IsSupported() || A == NULL || B == NULL) {
        if (A && B) {
            real_T sum = 0.0;
            for (int i = 0; i < 8; i++) {
                sum += A[i] * B[i];
            }
            return sum;
        }
        return 0.0;
    }
    
    return SIMD_VectorDotProduct4(A, B) + SIMD_VectorDotProduct4(A + 4, B + 4);
}

/* 기타 벡터 연산 */

real_T SIMD_VectorMax2(const real_T* A)
{
    if (!SIMD_IsSupported() || A == NULL) {
        if (A) {
            return (A[0] > A[1]) ? A[0] : A[1];
        }
        return 0.0;
    }
    
    #ifdef _MSC_VER
        __m128d a = _mm_loadu_pd(A);
        real_T result[2];
        _mm_storeu_pd(result, a);
        return (result[0] > result[1]) ? result[0] : result[1];
    #else
        __m128d a = _mm_loadu_pd(A);
        real_T result[2];
        _mm_storeu_pd(result, a);
        return (result[0] > result[1]) ? result[0] : result[1];
    #endif
}

real_T SIMD_VectorMin2(const real_T* A)
{
    if (!SIMD_IsSupported() || A == NULL) {
        if (A) {
            return (A[0] < A[1]) ? A[0] : A[1];
        }
        return 0.0;
    }
    
    #ifdef _MSC_VER
        __m128d a = _mm_loadu_pd(A);
        real_T result[2];
        _mm_storeu_pd(result, a);
        return (result[0] < result[1]) ? result[0] : result[1];
    #else
        __m128d a = _mm_loadu_pd(A);
        real_T result[2];
        _mm_storeu_pd(result, a);
        return (result[0] < result[1]) ? result[0] : result[1];
    #endif
}

void SIMD_VectorAbs2(const real_T* A, real_T* B)
{
    if (!SIMD_IsSupported() || A == NULL || B == NULL) {
        if (A && B) {
            B[0] = fabs(A[0]);
            B[1] = fabs(A[1]);
        }
        return;
    }
    
    /* SSE2에는 double 절댓값 명령어가 없어서 일반 연산 사용 */
    B[0] = fabs(A[0]);
    B[1] = fabs(A[1]);
}

void SIMD_VectorSqrt2(const real_T* A, real_T* B)
{
    if (!SIMD_IsSupported() || A == NULL || B == NULL) {
        if (A && B) {
            B[0] = sqrt(A[0]);
            B[1] = sqrt(A[1]);
        }
        return;
    }
    
    /* SSE2에는 double 제곱근 명령어가 없어서 일반 연산 사용 */
    B[0] = sqrt(A[0]);
    B[1] = sqrt(A[1]);
}
