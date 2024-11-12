<<<<<<< HEAD
/**
 * @file bigint_calculation.c
 * @brief bigint에 대한 연산을 하는 함수들을 정의한 소스 파일
 * @date 2024-10-18
 * @author 조동후
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/utils/string.h"
#include "../../include/utils/memory.h"
#include "../../include/base/base_type.h"
#include "../../include/base/base_error.h"
#include "../../include/bigint/bigint_calculation.h"

//#define create_prameter

/**
 * @brief bigint의 값을 다른 bigint로 할당하는 함수
 * @param dst bigint의 포인터
 */
msg bi_delete(OUT bigint** dst)
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
msg bi_new(OUT bigint** dst, IN uint32_t wordlen)
{   
    if (wordlen <= 0)
    {
        return print_invalid_word_length_error();
    }

    if (!is_null_pointer(*dst))
    {
        if (VERBOSE) fprintf(stdout, "[Warning] Already freed pointer.\n");
        
        bi_delete(dst);
        // return print_already_freed_error();
    }

    *dst = (bigint*)malloc(sizeof(bigint));
    (*dst)->sign = POSITIVE;
    (*dst)->wordlen = wordlen;
    (*dst)->start = (word*)calloc(wordlen, sizeof(word));
}
/**
* @memory 값을 copy하는 함수 추가함
*/
msg array_copy(OUT word* dst, IN word* src, IN uint32_t wordlen)
{   
    if (wordlen <= 0)
    {
        return print_invalid_word_length_error();
    }

    if (is_null_pointer(dst) || is_null_pointer(src))
    {
        return print_null_pointer_error();
    }

    memcpy(dst, src, sizeof(word) * wordlen);
}
/**
 * @brief arrary를 통해 bigint에 값을 할당하는 함수
 * @param dst bigint의 포인터
 * @param sign 부호
 * @param wordlen 배열의 길이
 * @param src 할당하고자 하는 배열
 */
msg bi_set_from_array(OUT bigint** dst, IN uint32_t sign, IN uint32_t wordlen, IN word* src)
{   
    if (wordlen <= 0)
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
msg bi_set_from_string(OUT bigint** dst, IN char* str, IN uint32_t base)
{
    if (is_null_pointer(dst))
    {
        return;
    }
    // 메모리가 이미 할당된 경우, 중복 할당 방지
    if (*dst == NULL) {
        msg result = bi_new(dst, 1);  // 최소 크기로 먼저 할당
        if (result != SUCCESS_INITIALIZATION) {
            return result;
        }
    }
    
    // 부호 확인
    int start_idx = 0;
    if (str[0] == '-') {
        (*dst)->sign = 1;  // 음수로 설정
        start_idx = 1;
    }
    else {
        (*dst)->sign = 0;  // 양수로 설정
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
msg bi_get_random(OUT bigint** dst, IN uint32_t sign, IN uint32_t wordlen)
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
msg bi_refine(OUT bigint* dst)
{   
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
msg bi_assign(OUT bigint** dst, IN bigint* src)
{
    if(is_null_pointer(src))
    {
        return;
    }

    bi_new(dst, src->wordlen);
    (*dst)->sign = src->sign;
    
    result = array_copy((*dst)->start, src->start, src->wordlen);
    if (result != SUCCESS_COPY)
    {
        free_if_exist((void**)*dst);
        return result;
    }

    return print_success_assign();
}

/**
 * @brief bigint를 출력하는 함수
 * @param dst bigint의 포인터
 * @param base 10진수, 16진수 등 어떻게 해석할지 의미함.
 */
msg bi_print(OUT bigint* dst, IN uint32_t base)
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

void bi_set_zero(bigint** dst)
{
    bi_new(dst, 1);
    (*dst)->sign = POSITIVE;
    (*dst)->start[0] = 0x00;
}

/**
 * @brief bigint가 0인지 확인하는 함수
 * @param dst bigint의 포인터
 * @return 0이면 1, 아니면 0
 */
int8_t is_zero(OUT bigint* dst)
{
    if(is_null_pointer(dst))
    {
        return print_null_pointer_error();
    }

    if(dst->sign == NEGATIVE)
    {   
        return false;
    }

    for (int i = dst->wordlen; i >= 0 ; i--)
    {
        if(dst->start[i] != 0)
        {
            return false;
        }
    }

    return true;
}

void bi_set_zero(OUT bigint** x)
{
    bi_new(x, 1);
    (*x)->sign = POSITIVE;
    (*x)->start[0] = 0;
}

int8_t bi_compare_ABS(IN bigint* x, IN bigint* y)
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

int8_t bi_compare(IN bigint* x, IN bigint* y)
{
    if (x->sign == POSITIVE && y->sign == NEGATIVE)
        return 1;

    if (x->sign == NEGATIVE && y->sign == POSITIVE)
        return -1;

    int ret = bi_compare_ABS(x, y);

    if (x->sign == POSITIVE)
        return ret;

    return ret * (-1);
}

/**
 * @brief word를 왼쪽으로 shift하는 함수
 * @param dst bigint의 포인터
 * @param k shift 하고싶은 word 사이즈
 */
msg bi_word_left_shift(OUT bigint* dst, IN byte k)
{
    bigint* tmp = NULL;
    bi_new(&tmp, dst->wordlen + k);
    tmp->sign = dst->sign;

    for (int i = k; i < tmp->wordlen; i++)
    {
        tmp->start[i] = dst->start[i - k];
    }
    //bi_refine(tmp); // dst가 refine된 값이라면 할 필요 x
    // dst를 NULL로 설정하여 bi_assign이 동작하도록 준비
    bi_delete(&dst);  // 기존 dst의 메모리를 해제
    dst = NULL;

    // bi_assign을 사용하여 tmp의 값을 dst에 복사
    bi_assign(&dst, tmp);

    // tmp 삭제
    bi_delete(&tmp);

    return print_success_shift();
}

/**
 * @brief word를 오른쪽으로 shift하는 함수
 * @param dst bigint의 포인터
 * @param k shift 하고싶은 word 사이즈
 */
msg bi_word_right_shift(OUT bigint* dst, IN byte k) // k는 shift하고싶은 word 사이즈 의미
{
    bigint* tmp = NULL;
    bi_new(&tmp, dst->wordlen - k);
    tmp->sign = dst->sign;

    for (int i = k; i < tmp->wordlen; i++)
    {
        tmp->start[i] = dst->start[i + k];
    }
    
    //bi_refine(tmp); // dst가 refine된 값이라면 할 필요 x
    // dst를 NULL로 설정하여 bi_assign이 동작하도록 준비
    bi_delete(&dst);  // 기존 dst의 메모리를 해제
    dst = NULL;

    // bi_assign을 사용하여 tmp의 값을 dst에 복사
    bi_assign(&dst, tmp);

    // tmp 삭제
    bi_delete(&tmp);

    return print_success_shift();
}