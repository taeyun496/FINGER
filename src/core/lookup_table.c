/*
 * lookup_table.c
 * 
 * Lookup Table 모듈 구현
 * 배터리 OCV(Open Circuit Voltage) 및 dOCV/dSOC 데이터 처리를 위한 모듈
 */

#include "lookup_table.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* 상수 정의 */
#define LOOKUP_MIN_POINTS      2       /* 최소 데이터 포인트 개수 */
#define LOOKUP_MAX_POINTS      10000   /* 최대 데이터 포인트 개수 */
#define LOOKUP_MIN_FRACTION    0.0     /* 최소 보간 비율 */
#define LOOKUP_MAX_FRACTION    1.0     /* 최대 보간 비율 */

/**
 * @brief Lookup Table 초기화
 */
boolean_T LookupTable_Initialize(LookupTable_T* table, 
                                const LookupTable_Params_T* params,
                                const real_T* breakpoints, 
                                const real_T* table_data, 
                                uint32_T num_points)
{
    if (table == NULL || params == NULL || breakpoints == NULL || 
        table_data == NULL || num_points < LOOKUP_MIN_POINTS || 
        num_points > LOOKUP_MAX_POINTS) {
        return false;
    }
    
    /* 파라미터 검증 */
    if (num_points > params->max_points) {
        return false;
    }
    
    /* 중단점이 오름차순으로 정렬되어 있는지 확인 */
    for (uint32_T i = 1; i < num_points; i++) {
        if (breakpoints[i] <= breakpoints[i-1]) {
            return false;
        }
    }
    
    /* 구조체 초기화 */
    table->num_points = num_points;
    table->initialized = false;
    
    /* 메모리 할당 */
    table->breakpoints = (real_T*)malloc(num_points * sizeof(real_T));
    table->table_data = (real_T*)malloc(num_points * sizeof(real_T));
    
    if (!table->breakpoints || !table->table_data) {
        LookupTable_Cleanup(table);
        return false;
    }
    
    /* 데이터 복사 */
    memcpy(table->breakpoints, breakpoints, num_points * sizeof(real_T));
    memcpy(table->table_data, table_data, num_points * sizeof(real_T));
    
    /* 초기화 완료 */
    table->initialized = true;
    
    return true;
}

/**
 * @brief Lookup Table 해제 (메모리 정리)
 */
void LookupTable_Cleanup(LookupTable_T* table)
{
    if (table == NULL) {
        return;
    }
    
    if (table->breakpoints) {
        free(table->breakpoints);
        table->breakpoints = NULL;
    }
    
    if (table->table_data) {
        free(table->table_data);
        table->table_data = NULL;
    }
    
    table->num_points = 0;
    table->initialized = false;
}

/**
 * @brief Lookup Table 검색 및 보간
 */
real_T LookupTable_Interpolate(const LookupTable_T* table, real_T x)
{
    uint32_T index;
    real_T fraction;
    
    return LookupTable_InterpolateAdvanced(table, x, &index, &fraction);
}

/**
 * @brief Lookup Table 검색 및 보간 (고급 옵션)
 */
real_T LookupTable_InterpolateAdvanced(const LookupTable_T* table, real_T x, 
                                      uint32_T* index, real_T* fraction)
{
    if (!table->initialized || index == NULL || fraction == NULL) {
        return 0.0;
    }
    
    /* 인덱스 찾기 */
    *index = LookupTable_BinarySearch(table, x);
    
    /* 보간 비율 계산 */
    if (*index >= table->num_points - 1) {
        *index = table->num_points - 2;
        *fraction = 1.0;
    } else if (*index == 0 && x <= table->breakpoints[0]) {
        *fraction = 0.0;
    } else {
        real_T x1 = table->breakpoints[*index];
        real_T x2 = table->breakpoints[*index + 1];
        
        if (fabs(x2 - x1) > 1e-10) {
            *fraction = (x - x1) / (x2 - x1);
        } else {
            *fraction = 0.0;
        }
        
        /* 보간 비율 범위 제한 */
        if (*fraction < LOOKUP_MIN_FRACTION) {
            *fraction = LOOKUP_MIN_FRACTION;
        } else if (*fraction > LOOKUP_MAX_FRACTION) {
            *fraction = LOOKUP_MAX_FRACTION;
        }
    }
    
    /* 선형 보간 */
    real_T y1 = table->table_data[*index];
    real_T y2 = table->table_data[*index + 1];
    
    return y1 + *fraction * (y2 - y1);
}

/**
 * @brief 이진 검색을 사용한 인덱스 찾기
 */
uint32_T LookupTable_BinarySearch(const LookupTable_T* table, real_T x)
{
    if (!table->initialized) {
        return 0;
    }
    
    uint32_T left = 0;
    uint32_T right = table->num_points - 1;
    
    /* 범위 외 값 처리 */
    if (x <= table->breakpoints[0]) {
        return 0;
    }
    if (x >= table->breakpoints[right]) {
        return right - 1;
    }
    
    /* 이진 검색 */
    while (right - left > 1) {
        uint32_T mid = (left + right) / 2;
        
        if (x < table->breakpoints[mid]) {
            right = mid;
        } else {
            left = mid;
        }
    }
    
    return left;
}

/**
 * @brief 선형 검색을 사용한 인덱스 찾기
 */
uint32_T LookupTable_LinearSearch(const LookupTable_T* table, real_T x)
{
    if (!table->initialized) {
        return 0;
    }
    
    /* 범위 외 값 처리 */
    if (x <= table->breakpoints[0]) {
        return 0;
    }
    if (x >= table->breakpoints[table->num_points - 1]) {
        return table->num_points - 2;
    }
    
    /* 선형 검색 */
    for (uint32_T i = 0; i < table->num_points - 1; i++) {
        if (x >= table->breakpoints[i] && x <= table->breakpoints[i + 1]) {
            return i;
        }
    }
    
    return table->num_points - 2;
}

/**
 * @brief Lookup Table 유효성 검사
 */
boolean_T LookupTable_Validate(const LookupTable_T* table)
{
    if (!table->initialized) {
        return false;
    }
    
    if (table->num_points < LOOKUP_MIN_POINTS) {
        return false;
    }
    
    /* 중단점이 오름차순으로 정렬되어 있는지 확인 */
    for (uint32_T i = 1; i < table->num_points; i++) {
        if (table->breakpoints[i] <= table->breakpoints[i-1]) {
            return false;
        }
    }
    
    return true;
}

/**
 * @brief Lookup Table 정보 출력 (디버깅용)
 */
void LookupTable_PrintInfo(const LookupTable_T* table)
{
    if (!table->initialized) {
        return;
    }
    
    /* 현재는 빈 함수로 구현 - 필요시 printf 등으로 확장 */
    (void)table; /* 컴파일러 경고 방지 */
}

/**
 * @brief 기존 코드와의 호환성을 위한 함수
 * MATLAB/Simulink에서 생성된 코드와 동일한 인터페이스
 */
real_T look1_binlxpw(real_T u0, const real_T bp0[], const real_T table[],
                     uint32_T maxIndex)
{
    /* 간단한 구현 - 실제로는 더 복잡한 로직이 필요할 수 있음 */
    if (maxIndex < 1) {
        return 0.0;
    }
    
    /* 범위 외 값 처리 */
    if (u0 <= bp0[0]) {
        return table[0];
    }
    if (u0 >= bp0[maxIndex]) {
        return table[maxIndex];
    }
    
    /* 이진 검색으로 인덱스 찾기 */
    uint32_T left = 0;
    uint32_T right = maxIndex;
    
    while (right - left > 1) {
        uint32_T mid = (left + right) / 2;
        
        if (u0 < bp0[mid]) {
            right = mid;
        } else {
            left = mid;
        }
    }
    
    /* 선형 보간 */
    real_T x1 = bp0[left];
    real_T x2 = bp0[left + 1];
    real_T y1 = table[left];
    real_T y2 = table[left + 1];
    
    if (fabs(x2 - x1) > 1e-10) {
        real_T fraction = (u0 - x1) / (x2 - x1);
        return y1 + fraction * (y2 - y1);
    } else {
        return y1;
    }
}
