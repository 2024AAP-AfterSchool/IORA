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
//#define create_prameter

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

    free_if_exist((*dst)->start);
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

    *dst = (bigint*)malloc(1, sizeof(bigint));
    (*dst)->sign = POSITIVE;
    (*dst)->wordlen = wordlen;
    (*dst)->start = (word*)calloc(wordlen, sizeof(word));
}
/**
* @memory 값을 copy하는 함수 추가함
*/
void array_copy(word* dst, word* src, int wordlen)                     // Copy array
{
    memcpy_s(dst, sizeof(word) * wordlen, src, sizeof(word) * wordlen);
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
    //수정함
    bi_new(dst, wordlen)
    (*dst)->sign = sign;
    array_copy(*dst->start, src, wordlen)
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
 * @create_prime이면 마지막 블럭이 all 0가 되는 것을 방지 = 키 크기 최대한 보장
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
#ifdef create_prameter
    if ((*dst)->start[wordlen - 1] == 0)
    {
        (*dst)->start[wordlen - 1] = rand() & 0xFF;
        while ((*dst)->start[wordlen - 1] == 0)
        {
            (*dst)->start[wordlen - 1] = rand() & 0xFF; // 0이 아닌 값을 보장
        }
    }
#endif
    bi_refine(*x);
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
    array_copy((*dst)->start, src->start, src->wordlen);
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