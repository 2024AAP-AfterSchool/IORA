/**
 * @file string.c
 * @brief 문자열과 관련된 함수들을 정의한 소스 파일
 * @date 2024-10-17
 * @author 조동후
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils/string.h"
#include "utils/memory.h"
#include "base/base_type.h"

/**
 * @brief 문자열을 숫자로 변환하는 함수
 * @param str 문자열
 * @param base 10진수, 16진수 등 어떻게 해석할지 의미함.
 */
uint32_t str_to_hex(word** dst, char* str, int base)
{
    char* end = NULL;

    uint32_t length = strlen(str);
    uint32_t chunk_size = 8; 

    // free_if_exist(dst);

    for (int i = 0; i < length; i += chunk_size)
    {
        char* chunk = str + i;
        printf("chunk: %s\n", chunk);
        word num_of_chunk = strtol(chunk, &end, base);
        printf("num_from_str: %d\n", num_of_chunk);
        uint32_t is_valid = !(strcmp(end, ""));

        if (is_valid)
        {
            *(*dst) = num_of_chunk;
            (*dst)++;
        }
        
        if (!is_valid)
        {
            return -1;
        }
    }
}