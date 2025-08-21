# Makefile for SoC 추정 시스템
# 모듈화된 구조로 리팩터링된 배터리 State of Charge 추정 시스템

# 컴파일러 설정
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -fPIC
DEBUG_CFLAGS = -Wall -Wextra -std=c99 -g -O0 -fPIC -DDEBUG

# 플랫폼별 설정
ifeq ($(OS),Windows_NT)
    # Windows
    PLATFORM = WIN
    EXT = .exe
    RM = del /Q
    MKDIR = mkdir
    RMDIR = rmdir /S /Q
else
    # Linux/macOS
    PLATFORM = UNIX
    EXT = 
    RM = rm -f
    MKDIR = mkdir -p
    RMDIR = rm -rf
endif

# SIMD 최적화 설정
ifeq ($(PLATFORM),WIN)
    # Windows MSVC
    SIMD_CFLAGS = -msse2
else
    # Linux/macOS GCC/Clang
    SIMD_CFLAGS = -msse2 -march=native
endif

# 디렉토리 설정
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
LIB_DIR = lib
TEST_DIR = test

# 소스 파일들
CORE_SOURCES = $(SRC_DIR)/core/ekf.c \
               $(SRC_DIR)/core/rls.c \
               $(SRC_DIR)/core/lookup_table.c

MATH_SOURCES = $(SRC_DIR)/math/matrix_ops.c \
               $(SRC_DIR)/math/simd_ops.c

MAIN_SOURCE = $(SRC_DIR)/main.c

# 기존 코드 소스 (호환성을 위해)
LEGACY_SOURCES = SoCesti_data.c \
                 rt_nonfinite.c

# 모든 소스 파일
ALL_SOURCES = $(CORE_SOURCES) $(MATH_SOURCES) $(MAIN_SOURCE) $(LEGACY_SOURCES)

# 오브젝트 파일들
CORE_OBJECTS = $(CORE_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
MATH_OBJECTS = $(MATH_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
MAIN_OBJECT = $(BUILD_DIR)/main.o
LEGACY_OBJECTS = $(LEGACY_SOURCES:.c=.o)

# 헤더 파일들
INCLUDES = -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/core -I$(INCLUDE_DIR)/math -I.

# 라이브러리 이름
LIB_NAME = libsoc_estimator
STATIC_LIB = $(LIB_DIR)/$(LIB_NAME).a
SHARED_LIB = $(LIB_DIR)/$(LIB_NAME).so

# 실행 파일 이름
EXEC_NAME = soc_estimator$(EXT)

# 기본 타겟
all: directories $(STATIC_LIB) $(SHARED_LIB) $(EXEC_NAME)

# 디버그 빌드
debug: CFLAGS = $(DEBUG_CFLAGS)
debug: all

# 릴리즈 빌드
release: CFLAGS += -DNDEBUG -O3
release: all

# 디렉토리 생성
directories:
	@$(MKDIR) $(BUILD_DIR)
	@$(MKDIR) $(BUILD_DIR)/core
	@$(MKDIR) $(BUILD_DIR)/math
	@$(MKDIR) $(LIB_DIR)
	@$(MKDIR) $(TEST_DIR)

# 정적 라이브러리 빌드
$(STATIC_LIB): $(CORE_OBJECTS) $(MATH_OBJECTS)
	@echo "정적 라이브러리 빌드 중: $@"
	@$(MKDIR) $(LIB_DIR)
	ar rcs $@ $^

# 공유 라이브러리 빌드
$(SHARED_LIB): $(CORE_OBJECTS) $(MATH_OBJECTS)
	@echo "공유 라이브러리 빌드 중: $@"
	@$(MKDIR) $(LIB_DIR)
	$(CC) -shared -o $@ $^ $(CFLAGS)

# 실행 파일 빌드
$(EXEC_NAME): $(MAIN_OBJECT) $(LEGACY_OBJECTS) $(STATIC_LIB)
	@echo "실행 파일 빌드 중: $@"
	$(CC) -o $@ $^ $(CFLAGS) -L$(LIB_DIR) -lsoc_estimator

# 코어 모듈 오브젝트 파일들
$(BUILD_DIR)/core/%.o: $(SRC_DIR)/core/%.c
	@echo "컴파일 중: $<"
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $(SIMD_CFLAGS) $(INCLUDES) -c $< -o $@

# 수학 모듈 오브젝트 파일들
$(BUILD_DIR)/math/%.o: $(SRC_DIR)/math/%.c
	@echo "컴파일 중: $<"
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $(SIMD_CFLAGS) $(INCLUDES) -c $< -o $@

# 메인 오브젝트 파일
$(BUILD_DIR)/main.o: $(MAIN_SOURCE)
	@echo "컴파일 중: $<"
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $(SIMD_CFLAGS) $(INCLUDES) -c $< -o $@

# 기존 코드 오브젝트 파일들
%.o: %.c
	@echo "컴파일 중: $<"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 테스트 빌드
test: CFLAGS += -DTEST_MODE
test: $(EXEC_NAME)
	@echo "테스트 실행 중..."
	./$(EXEC_NAME)

# 정리
clean:
	@echo "빌드 파일 정리 중..."
	$(RM) $(EXEC_NAME)
	$(RMDIR) $(BUILD_DIR)
	$(RMDIR) $(LIB_DIR)
	$(RM) *.o

# 설치
install: $(STATIC_LIB) $(SHARED_LIB)
	@echo "라이브러리 설치 중..."
	@$(MKDIR) /usr/local/lib
	@$(MKDIR) /usr/local/include
	cp $(STATIC_LIB) /usr/local/lib/
	cp $(SHARED_LIB) /usr/local/lib/
	cp $(INCLUDE_DIR)/core/*.h /usr/local/include/
	cp $(INCLUDE_DIR)/math/*.h /usr/local/include/

# 의존성 정보
depend: $(ALL_SOURCES)
	@echo "의존성 분석 중..."
	$(CC) -MM $(INCLUDES) $^ > Makefile.dep

# 도움말
help:
	@echo "사용 가능한 타겟:"
	@echo "  all       - 기본 빌드 (정적/공유 라이브러리 + 실행 파일)"
	@echo "  debug     - 디버그 정보 포함 빌드"
	@echo "  release   - 최적화된 릴리즈 빌드"
	@echo "  test      - 테스트 모드로 빌드 및 실행"
	@echo "  clean     - 빌드 파일 정리"
	@echo "  install   - 시스템에 라이브러리 설치"
	@echo "  depend    - 의존성 분석"
	@echo "  help      - 이 도움말 표시"

# 의존성 파일 포함
-include Makefile.dep

# 가상 타겟
.PHONY: all debug release directories clean install depend help test
