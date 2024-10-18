/**
 * @file bigint_calculation.c
 * @brief bigint에 대한 연산을 하는 함수들을 정의한 소스 파일
 * @date 2024-10-18
 * @author 조동후
 */

#include <stdio.h>
#include <stdlib.h>
#include "utils/string.h"
#include "utils/memory.h"
#include "base/base_type.h"
#include "bigint/bigint_calculation.h"

/**
 * @brief bigint의 값을 다른 bigint로 할당하는 함수
 * @param dst bigint의 포인터
 */
void bi_delete(bigint** dst)
{
    if (is_null_pointer(*dst))
    {
        return;
    }

    // free_if_exist((*dst)->start);
    free_if_exist(*dst);
}

/**
 * @brief bigint에 arrary를 할당하는 함수
 * @param dst bigint의 포인터
 * @param wordlen 배열의 길이
 */
void bi_new(bigint** dst, uint32_t wordlen)
{   
    bi_delete(dst);

    *dst = (bigint*)calloc(1, sizeof(bigint));
    (*dst)->sign = POSITIVE;
    (*dst)->wordlen = wordlen;
    (*dst)->start = (word*)calloc(wordlen, sizeof(word));
}

/**
 * @brief arrary를 통해 bigint에 값을 할당하는 함수
 * @param dst bigint의 포인터
 * @param sign 부호
 * @param wordlen 배열의 길이
 * @param src 할당하고자 하는 배열
 */
void bi_set_from_array(bigint** dst, uint32_t sign, uint32_t wordlen, word* src)
{   
    if(is_null_pointer(dst))
    {
        return;
    }

    if(is_null_pointer(src))
    {
        return;
    }

    (*dst)->sign = sign;
    (*dst)->wordlen = wordlen;
    (*dst)->start = src;
}

/**
 * @brief string을 통해 bigint에 값을 할당하는 함수
 * @param dst bigint의 포인터
 * @param str 할당하고자 하는 문자열
 * @param base 10진수, 16진수 등 어떻게 해석할지 의미함.
 */
void bi_set_from_string(bigint** dst, char* str, uint32_t base)
{
    if(is_null_pointer(dst))
    {
        return;
    }

    if(is_null_pointer(str))
    {
        return;
    }

    uint32_t sign = *str == '-' ? NEGATIVE : POSITIVE;
    if(sign == NEGATIVE)
    {
        str++;
    }

    if (*str == '0' && (*(str + 1) == 'x' || *(str +1 ) == 'X'))
    {
        str += 2;
    }

    uint32_t zero_count = 0;
    char* temp_str = str;
    while (*temp_str == '0')
    {
        zero_count++;
        temp_str++;
    }

    printf("%s\n", str);
    word target_num = str_to_hex((*dst)->start, str, base);
    printf("target_num: %d\n", target_num);
    if (target_num == -1)
    {
        return;
    }

    (*dst)->sign = sign;
    (*dst)->wordlen = sizeof((*dst)->start) / sizeof(word) + zero_count;
}

/**
 * @brief bigint에 무작위 값을 할당하는 함수
 * @param dst bigint의 포인터
 * @param sign 부호
 * @param wordlen 배열의 길이
 */
void bi_get_random(bigint** dst, uint32_t sign, uint32_t wordlen)
{
    bi_new(dst, wordlen);

    (*dst)->sign = sign;
    for (int i = 0; i < wordlen; i++)
    {
        (*dst)->start[i] = rand() & 0xFF;
    }
}

/**
 * @brief bigint의 마지막 워드가 0인 경우 bigint의 메모리를 재할당하는 함수
 * @param dst bigint의 포인터
 */
void bi_refine(bigint* dst)
{   
    uint32_t origin_wordlen = dst->wordlen;

    if(is_null_pointer(dst))
    {
        return;
    }

    while ((dst->start[dst->wordlen - 1] == 0) && (dst->wordlen > 1))
    {
        dst->wordlen--;
    }

    if (origin_wordlen == dst->wordlen)
    {
        return;
    }

    dst->start = (word*)realloc(dst->start, dst->wordlen * sizeof(word));

    if((dst->start[0] == 0) && (dst->wordlen == 1))
    {
        dst->sign = POSITIVE;
    }
}

/**
 * @brief bigint의 값을 다른 bigint로 할당하는 함수
 * @param dst bigint의 포인터
 * @param src bigint의 포인터
 */
void bi_assign(bigint** dst, bigint* src)
{
    if(!is_null_pointer(dst))
    {
        return;
    }

    if(is_null_pointer(src))
    {
        return;
    }

    bi_new(dst, src->wordlen);
    (*dst)->sign = src->sign;
    (*dst)->start = src->start;
}

/**
 * @brief bigint를 출력하는 함수
 * @param dst bigint의 포인터
 * @param base 10진수, 16진수 등 어떻게 해석할지 의미함.
 */
void bi_print(bigint* dst, uint32_t base)
{
    if(is_null_pointer(dst))
    {
        return;
    }
    
    if (dst->sign == NEGATIVE)
    {
        printf("-");
    }

    printf("0x");
    for (int i = dst->wordlen - 1; i >= 0; i--)
    {
        printf("%08X", dst->start[i]);
        // TODO: base에 따라 출력 형식을 변경할 수 있도록 구현
    }
    printf("\n");
}