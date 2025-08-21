/*
 * lookup_table.h
 * 
 * Lookup Table 모듈
 * 배터리 OCV(Open Circuit Voltage) 및 dOCV/dSOC 데이터 처리를 위한 모듈
 * 
 * 주요 기능:
 * - 1차원 Lookup Table 검색
 * - 이진 검색 (Binary Search)
 * - 선형 보간 (Linear Interpolation)
 * - 범위 외 값 처리 (Extrapolation)
 */

#ifndef LOOKUP_TABLE_H
#define LOOKUP_TABLE_H

#include "rtwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Lookup Table 구조체 */
typedef struct {
    real_T* breakpoints;            /* 중단점 배열 (x축) */
    real_T* table_data;             /* 테이블 데이터 배열 (y축) */
    uint32_T num_points;            /* 데이터 포인트 개수 */
    boolean_T initialized;          /* 초기화 완료 플래그 */
} LookupTable_T;

/* Lookup Table 파라미터 구조체 */
typedef struct {
    uint32_T max_points;            /* 최대 데이터 포인트 개수 */
    boolean_T use_binary_search;    /* 이진 검색 사용 여부 */
    boolean_T enable_extrapolation; /* 외삽 허용 여부 */
} LookupTable_Params_T;

/* 함수 선언 */

/**
 * @brief Lookup Table 초기화
 * @param table Lookup Table 구조체 포인터
 * @param params Lookup Table 파라미터
 * @param breakpoints 중단점 배열
 * @param table_data 테이블 데이터 배열
 * @param num_points 데이터 포인트 개수
 * @return 초기화 성공 여부
 */
boolean_T LookupTable_Initialize(LookupTable_T* table, 
                                const LookupTable_Params_T* params,
                                const real_T* breakpoints, 
                                const real_T* table_data, 
                                uint32_T num_points);

/**
 * @brief Lookup Table 해제 (메모리 정리)
 * @param table Lookup Table 구조체 포인터
 */
void LookupTable_Cleanup(LookupTable_T* table);

/**
 * @brief Lookup Table 검색 및 보간
 * @param table Lookup Table 구조체 포인터
 * @param x 검색할 x 값
 * @return 보간된 y 값
 */
real_T LookupTable_Interpolate(const LookupTable_T* table, real_T x);

/**
 * @brief Lookup Table 검색 및 보간 (고급 옵션)
 * @param table Lookup Table 구조체 포인터
 * @param x 검색할 x 값
 * @param index 출력: 사용된 인덱스
 * @param fraction 출력: 보간 비율
 * @return 보간된 y 값
 */
real_T LookupTable_InterpolateAdvanced(const LookupTable_T* table, real_T x, 
                                      uint32_T* index, real_T* fraction);

/**
 * @brief 이진 검색을 사용한 인덱스 찾기
 * @param table Lookup Table 구조체 포인터
 * @param x 검색할 x 값
 * @return 찾은 인덱스
 */
uint32_T LookupTable_BinarySearch(const LookupTable_T* table, real_T x);

/**
 * @brief 선형 검색을 사용한 인덱스 찾기
 * @param table Lookup Table 구조체 포인터
 * @param x 검색할 x 값
 * @return 찾은 인덱스
 */
uint32_T LookupTable_LinearSearch(const LookupTable_T* table, real_T x);

/**
 * @brief Lookup Table 유효성 검사
 * @param table Lookup Table 구조체 포인터
 * @return 유효성 여부
 */
boolean_T LookupTable_Validate(const LookupTable_T* table);

/**
 * @brief Lookup Table 정보 출력 (디버깅용)
 * @param table Lookup Table 구조체 포인터
 */
void LookupTable_PrintInfo(const LookupTable_T* table);

/* 기존 코드와의 호환성을 위한 함수 */
real_T look1_binlxpw(real_T u0, const real_T bp0[], const real_T table[],
                     uint32_T maxIndex);

#ifdef __cplusplus
}
#endif

#endif /* LOOKUP_TABLE_H */
