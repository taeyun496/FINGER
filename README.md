# SoC 추정 시스템 (State of Charge Estimator)

## 개요

이 프로젝트는 MATLAB/Simulink에서 자동 생성된 배터리 State of Charge (SoC) 추정 코드를 **가독성 향상, 모듈화, 임베디드 최적화**를 위해 리팩터링한 것입니다.

## 주요 기능

- **EKF (Extended Kalman Filter)**: 배터리 상태 추정
- **RLS (Recursive Least Squares)**: 파라미터 추정
- **Lookup Table**: OCV 및 dOCV/dSOC 데이터 처리
- **SIMD 최적화**: Intel SSE2 명령어를 사용한 벡터 연산
- **행렬 연산**: 2x2, 3x3 행렬 연산 최적화

## 프로젝트 구조

```
SoC_esti/
├── include/                 # 헤더 파일들
│   ├── core/               # 핵심 알고리즘 헤더
│   │   ├── ekf.h          # EKF 모듈
│   │   ├── rls.h          # RLS 모듈
│   │   └── lookup_table.h # Lookup Table 모듈
│   └── math/               # 수학 연산 헤더
│       ├── matrix_ops.h    # 행렬 연산
│       └── simd_ops.h      # SIMD 최적화
├── src/                    # 소스 코드
│   ├── core/               # 핵심 알고리즘 구현
│   │   ├── ekf.c          # EKF 구현
│   │   ├── rls.c          # RLS 구현
│   │   └── lookup_table.c # Lookup Table 구현
│   ├── math/               # 수학 연산 구현
│   │   ├── matrix_ops.c   # 행렬 연산 구현
│   │   └── simd_ops.c     # SIMD 최적화 구현
│   └── main.c              # 메인 모듈 통합
├── Makefile                # 빌드 시스템
├── README.md               # 이 파일
└── [기존 파일들]           # 원본 MATLAB/Simulink 코드
```

## 빌드 방법

### 필수 요구사항

- GCC 컴파일러 (4.9 이상)
- Make 유틸리티
- Intel SSE2 지원 CPU (SIMD 최적화용)

### 기본 빌드

```bash
# 기본 빌드
make

# 디버그 빌드
make debug

# 릴리즈 빌드 (최적화)
make release
```

### 테스트 실행

```bash
# 테스트 모드로 빌드 및 실행
make test
```

### 정리

```bash
# 빌드 파일 정리
make clean
```

## 사용법

### 기본 사용법

```c
#include "core/ekf.h"
#include "core/rls.h"
#include "core/lookup_table.h"

// EKF 초기화
EKF_T ekf;
EKF_Params_T params;
// ... 파라미터 설정 ...
EKF_Initialize(&ekf, &params);

// RLS 초기화
RLS_T rls;
RLS_Params_T rls_params;
// ... 파라미터 설정 ...
RLS_Initialize(&rls, &rls_params, 3);

// 시스템 실행
real_T soc = EKF_GetSoC(&ekf);
EKF_Step(&ekf, voltage, current, dt);
```

### 기존 코드와의 호환성

기존 MATLAB/Simulink 코드와 완전히 호환됩니다:

```c
// 기존 함수들 그대로 사용 가능
SoCesti_initialize();
SoCesti_step();
SoCesti_terminate();
```

## 모듈 설명

### 1. EKF 모듈 (`core/ekf`)

Extended Kalman Filter를 구현한 모듈입니다.

- **상태 예측**: 전류 기반 SoC 예측
- **측정 업데이트**: 전압 측정을 통한 상태 보정
- **공분산 관리**: 추정 오차 공분산 업데이트

### 2. RLS 모듈 (`core/rls`)

Recursive Least Squares를 구현한 모듈입니다.

- **파라미터 추정**: 배터리 모델 파라미터 실시간 추정
- **망각 인자**: 시간에 따른 파라미터 변화 추적
- **수치적 안정성**: 공분산 행렬의 수치적 안정성 보장

### 3. Lookup Table 모듈 (`core/lookup_table`)

OCV 및 dOCV/dSOC 데이터 처리를 위한 모듈입니다.

- **이진 검색**: 빠른 데이터 검색
- **선형 보간**: 연속적인 값 추정
- **범위 처리**: 범위 외 값에 대한 적절한 처리

### 4. 행렬 연산 모듈 (`math/matrix_ops`)

2x2, 3x3 행렬 연산을 최적화한 모듈입니다.

- **행렬 곱셈**: 최적화된 행렬 곱셈
- **역행렬 계산**: 수치적으로 안정적인 역행렬 계산
- **행렬식 계산**: 효율적인 행렬식 계산

### 5. SIMD 최적화 모듈 (`math/simd_ops`)

Intel SSE2 명령어를 사용한 벡터 연산 최적화 모듈입니다.

- **벡터 연산**: 2, 4, 8개 double 동시 처리
- **플랫폼 독립성**: Windows/Linux/macOS 지원
- **자동 폴백**: SIMD 미지원 시 일반 연산으로 자동 전환

## 성능 최적화

### 1. 메모리 최적화

- **정적 할당**: 힙 사용 최소화
- **캐시 친화적**: 메모리 접근 패턴 최적화
- **SIMD 정렬**: 16바이트 경계 정렬

### 2. 연산 최적화

- **SIMD 벡터화**: SSE2 명령어 활용
- **행렬 연산 최적화**: 2x2, 3x3 전용 함수
- **루프 언롤링**: 작은 크기 배열에 대한 최적화

### 3. 임베디드 최적화

- **고정소수점 지원**: 부동소수점 연산 최소화
- **메모리 풀**: 동적 할당 최소화
- **인라인 함수**: 함수 호출 오버헤드 감소

## 테스트 및 검증

### 단위 테스트

각 모듈별로 독립적인 테스트가 가능합니다:

```bash
# EKF 모듈 테스트
make test_ekf

# RLS 모듈 테스트  
make test_rls

# 전체 시스템 테스트
make test
```

### 성능 테스트

```bash
# 성능 벤치마크 실행
make benchmark

# 메모리 사용량 분석
make profile
```

## 라이센스

이 프로젝트는 **Trial License**로 제공됩니다. 평가 목적으로만 사용 가능하며, 상업적 사용을 위해서는 별도 라이센스가 필요합니다.

## 기여 방법

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 문의사항

프로젝트에 대한 문의사항이나 버그 리포트는 이슈 트래커를 통해 제출해 주세요.

## 변경 이력

### v2.0.0 (2025-08-21)
- **완전한 모듈화**: 기존 단일 파일을 기능별 모듈로 분리
- **한글 주석**: 가독성 향상을 위한 한글 주석 추가
- **SIMD 최적화**: Intel SSE2 기반 벡터 연산 최적화
- **행렬 연산 모듈**: 2x2, 3x3 행렬 연산 전용 모듈
- **빌드 시스템**: Makefile 기반 자동화된 빌드 시스템

### v1.4.0 (2025-08-21)
- **원본 코드**: MATLAB/Simulink 자동 생성 코드
- **기본 기능**: EKF, RLS, Lookup Table 기반 SoC 추정

## 감사의 말

이 프로젝트는 MATLAB/Simulink의 강력한 코드 생성 기능과 C 언어의 효율성을 결합하여 만들어졌습니다. 모든 기여자들에게 감사드립니다.
