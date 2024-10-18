/**
 * @file memory.h
 * @brief 메모리 할당, 해제 등 메모리와 관련된 함수들을 정의한 헤더 파일
 * @date 2024-10-17
 * @author 조동후
 */
 
#ifndef IORA_UTILS_MEMORY_H_
#define IORA_UTILS_MEMORY_H_

#include <stdbool.h>
#include "base/base_type.h"

/**
 * @brief 포인터가 NULL인지 확인하는 함수
 * @param ptr 포인터
 */
bool is_null_pointer(void* ptr);

/**
 * @brief 포인터의 메모리가 할당되어 있으면 해제하는 함수
 * @param ptr 포인터
 */
void free_if_exist(void* ptr);

#endif // IORA_UTILS_MEMORY_H_