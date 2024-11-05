# CMake의 최소 요구 버전을 설정합니다. 여기서는 3.10을 사용합니다.
cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름과 버전을 정의합니다.
project(IORA_Project VERSION 1.0)

# C 표준을 C99로 설정합니다.
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

if(APPLE)
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif()

# 빌드 출력 파일의 위치를 지정합니다. (build 디렉토리 하위에 운영체제 별로 파일을 생성)
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/build/linux)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build/linux)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/build/windows)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build/windows)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")  # MacOS
    set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/build/mac)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build/mac)
endif()

# 빌드할 소스 디렉토리와 헤더 파일이 있는 디렉토리를 포함 경로로 추가합니다.
include_directories(${PROJECT_SOURCE_DIR}/include)

# src 디렉토리의 모든 .c 파일들을 찾습니다.
file(GLOB SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.c
    ${PROJECT_SOURCE_DIR}/src/*/*.c
)

# 라이브러라 파일을 빌드합니다.
add_library(IORA_LIB SHARED ${SOURCE_FILES})
set_target_properties(IORA_LIB PROPERTIES PREFIX "" OUTPUT_NAME "IORA"
                        LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIRECTORY})

# 실행 파일을 정의합니다. IORA라는 이름으로 소스 파일들을 빌드합니다.
add_executable(IORA ${SOURCE_FILES})

# 메모리 테스트 AddressSanitizer 설정
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

enable_testing()

add_test(NAME AddressSanitizerTest
    COMMAND $<TARGET_FILE:IORA>
)