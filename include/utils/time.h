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

#define START_TIMER() \
    clock_t start = clock();

#define END_TIMER(result, msg) \
    clock_t end = clock(); \
    result.message = msg; \
    result.time = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;

#endif // IORA_UTILS_TIME_H_