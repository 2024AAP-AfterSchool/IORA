/**
 * @file memory.c
 * @brief 메모리 할당, 해제 등 메모리와 관련된 함수들을 정의한 소스 파일
 * @date 2024-10-17
 * @author 조동후
 */

#include <stdio.h>
#include <stdlib.h>
#include "utils/memory.h"
#include "base/base_type.h"

/**
 * @brief 포인터가 NULL인지 확인하는 함수
 * @param ptr 포인터
 */
bool is_null_pointer(void* ptr)
{
    return ptr == NULL;
}

/**
 * @brief 포인터의 메모리가 할당되어 있으면 해제하는 함수
 * @param ptr 포인터
 */
void free_if_exist(void** ptr)
{   
    if (!is_null_pointer(ptr) && !is_null_pointer(*ptr))
    {
        printf("Freeing memory...\n");
        printf("ptr: %p\n", ptr);
        free(*ptr);
        *ptr = NULL;
    }
}