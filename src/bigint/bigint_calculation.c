/**
 * @file bigint_calculation.c
 * @brief bigint에 대한 연산을 하는 함수들을 정의한 소스 파일
 * @date 2024-10-18
 * @author 조동후
 */
#include <stdio.h>
#include <string.h>
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

    *dst = (bigint*)malloc(sizeof(bigint));
    (*dst)->sign = POSITIVE;
    (*dst)->wordlen = wordlen;
    (*dst)->start = (word*)calloc(wordlen, sizeof(word));
}
/**
* @memory 값을 copy하는 함수 추가함
*/
void array_copy(word* dst, word* src, uint32_t wordlen)                     // Copy array
{
    memcpy(dst, src, sizeof(word) * wordlen);
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
    if (is_null_pointer(dst))
    {
        return;
    }

    if (is_null_pointer(src))
    {
        return;
    }
    //수정함
    bi_new(dst, wordlen);
    (*dst)->sign = sign;
    array_copy((*dst)->start, src, wordlen);
}

/**
 * @brief string을 통해 bigint에 값을 할당하는 함수
 * @param dst bigint의 포인터
 * @param str 할당하고자 하는 문자열
 * @param base 10진수, 16진수 등 어떻게 해석할지 의미함.
 */
void bi_set_from_string(bigint** dst, char* str, uint32_t base)
{
    if (is_null_pointer(dst))
    {
        return;
    }

    if (is_null_pointer(str))
    {
        return;
    }

    uint32_t sign = *str == '-' ? NEGATIVE : POSITIVE;
    if (sign == NEGATIVE)
    {
        str++;
    }

    if (*str == '0' && (*(str + 1) == 'x' || *(str + 1) == 'X'))
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
    bi_refine(*dst);
}

/**
 * @brief bigint의 마지막 워드가 0인 경우 bigint의 메모리를 재할당하는 함수
 * @param dst bigint의 포인터
 */
void bi_refine(bigint* dst)
{
    uint32_t origin_wordlen = dst->wordlen;

    if (is_null_pointer(dst))
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

    // 새로운 포인터를 받아 기존 포인터를 그대로 유지하는 방식으로 realloc 사용
    word* new_start = (word*)realloc(dst->start, dst->wordlen * sizeof(word));

    // realloc 실패 처리
    if (new_start == NULL)
    {
        return;
    }

    // 성공적으로 할당되었으므로 포인터 업데이트
    dst->start = new_start;


    if ((dst->start[0] == 0) && (dst->wordlen == 1))
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
    if (!is_null_pointer(dst))
    {
        return;
    }

    if (is_null_pointer(src))
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
    if (is_null_pointer(dst))
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
/*
void bi_set_zero(bigint** dst)
{
    bi_new(dst, 1);
    (*dst)->sign = POSITIVE;
    (*dst)->start[0] = 0x00;
}
*/
bool is_zero(bigint* dst)
{
    if (dst->a[0] != 0)
        return false;
    for (int i = 1; i < (dst->wordlen); i++)
    {
        if (dst->a[i] != 0)
            return false;
    }
}
int compare_ABS(bigint* x, bigint* y)
{
    int n = x->wordlen;
    int m = y->wordlen;

    // Case: A > B
    if (n > m)
        return 1;

    // Case: A < B
    if (n < m)
        return -1;

    for (int i = n - 1; i >= 0; i--) {

        if (x->start[i] > y->start[i])
            return 1;

        if (x->start[i] < y->start[i])
            return -1;

    }

    return 0;
}
int compare(bigint* x, bigint* y)
{
    if (x->sign == POSITIVE && y->sign == NEGATIVE)
        return 1;

    if (x->sign == NEGATIVE && y->sign == POSITIVE)
        return -1;

    int ret = compare_ABS(x, y);

    if (x->sign == POSITIVE)
        return ret;

    return ret * (-1);
}
word_left_shift(bigint* dst, byte k) // k는 shift하고싶은 word 사이즈 의미
{
    bigint* tmp = NULL;
    bi_new(&tmp, k + dst->wordlen);
    tmp->sign = dst->sign;

    for (int i = k; i < tmp->wordlen; i++)
    {
        tmp->x[i] = dst->x[i - k];
    }
   //bi_refine(tmp); // dst가 refine된 값이라면 할 필요 x
    // dst를 NULL로 설정하여 bi_assign이 동작하도록 준비
    bi_delete(&dst);  // 기존 dst의 메모리를 해제
    dst = NULL;

    // bi_assign을 사용하여 tmp의 값을 dst에 복사
    bi_assign(&dst, tmp);

    // tmp 삭제
    bi_delete(&tmp);
    return;
}