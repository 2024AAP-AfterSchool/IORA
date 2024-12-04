/**
 * @file time.h
 * @brief 시간과 관련된 함수들을 정의한 헤더 파일
 * @date 2024-11-29
 * @author 조동후
 */
#ifndef IORA_UTILS_TIME_H_
#define IORA_UTILS_TIME_H_

#include <time.h>
#include "base/base_type.h"

// 고해상도 타이머 시작
#define START_TIMER() \
    struct timespec start; \
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

// 고해상도 타이머 종료
#define END_TIMER(result, msg) \
    struct timespec end; \
    clock_gettime(CLOCK_MONOTONIC_RAW, &end); \
    result.message = msg; \
    result.time = ((end.tv_sec - start.tv_sec) * 1000.0) + ((end.tv_nsec - start.tv_nsec) / 1e6);

#endif // IORA_UTILS_TIME_H_