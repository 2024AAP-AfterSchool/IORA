cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_C_COMPILER "C:/MinGW/bin/gcc.exe")
endif()

project(IORA LANGUAGES C VERSION 1.0)

# C 표준을 C99로 설정
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
project(IORA LANGUAGES C VERSION 1.0)

if(APPLE)
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif()

# 빌드 출력 파일의 위치를 지정 (build 디렉토리 하위에 운영체제 별로 파일을 생성)
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

# 컴파일 옵션 분기
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Linux 환경 설정
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -O3 -fprofile-arcs")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs ")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # MacOS 환경 설정
    if(CMAKE_C_COMPILER_ID STREQUAL "AppleClang" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -O3 -fprofile-arcs")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs")
    endif()
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # Windows 환경 설정
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -O3 -fprofile-arcs")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs")
endif()

# 빌드할 소스 디렉토리와 헤더 파일이 있는 디렉토리를 포함 경로로 추가
include_directories(${PROJECT_SOURCE_DIR}/include)

# src 디렉토리의 모든 .c 파일 탐색
file(GLOB SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.c
    ${PROJECT_SOURCE_DIR}/src/*/*.c
)

# 실행 파일 빌드
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_executable(IORA ${SOURCE_FILES})
    set_target_properties(IORA PROPERTIES OUTPUT_NAME "IORA"
                        RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
endif()

# main 함수는 라이브러리 만들 때 제외
list(REMOVE_ITEM SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/main.c)

# 동적 라이브러리 파일 빌드
add_library(IORA_LIB_SHARED SHARED ${SOURCE_FILES})
set_target_properties(IORA_LIB_SHARED PROPERTIES PREFIX "" OUTPUT_NAME "IORA"
                        LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIRECTORY}
                        RUNTIME_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIRECTORY})

# 정적 라이브러리 파일 빌드
add_library(IORA_LIB_STATIC STATIC ${SOURCE_FILES})
set_target_properties(IORA_LIB_STATIC PROPERTIES PREFIX "" OUTPUT_NAME "IORA"
                        ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIRECTORY})

# 메모리 테스트 AddressSanitizer 설정
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    if(MEMORY_CHECK)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

        enable_testing()

        add_test(NAME AddressSanitizerTest
            COMMAND $<TARGET_FILE:IORA>
        )
    endif()
endif()