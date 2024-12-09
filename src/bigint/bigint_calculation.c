/**
 * @file bigint_calculation.c
 * @brief bigint에 대한 연산을 하는 함수들을 정의한 소스 파일
 * @date 2024-10-18
 * @author 조동후
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "utils/time.h"
#include "utils/string.h"
#include "utils/memory.h"
#include "base/base_type.h"
#include "base/base_error.h"
#include "bigint/bigint_calculation.h"


/**
 * @brief bigint에 무작위 값을 할당하는 함수
 * @create_prime이면 마지막 블럭이 all 0가 되는 것을 방지 = 키 크기 최대한 보장
 * @param dst bigint의 포인터
 * @param sign 부호
 * @param wordlen 배열의 길이
 */


res bi_get_random(OUT bigint** dst, IN uint32_t sign, IN uint32_t wordlen) {   
    res result;
    START_TIMER();

    if (wordlen <= 0) {
        END_TIMER(result, print_invalid_word_length_error());
        return result;
    }

    // 랜덤 시드 초기화 (시간 기반)
    srand((unsigned int)clock());

    // 메모리 할당
    bi_new(dst, wordlen);
    (*dst)->sign = sign;

    for (int i = 0; i < wordlen; i++) {
        (*dst)->start[i] = 0x00;

        for (int j = 1; j < sizeof(word); j++) {   
            (*dst)->start[i] |= rand() & 0xFF; // 랜덤 값 생성
            (*dst)->start[i] <<= 8;
        }
        (*dst)->start[i] |= rand() & 0xFF;
    }
#ifdef create_prameter
    // 마지막 단어가 0이 아닌 값으로 보장
    if ((*dst)->start[wordlen - 1] == 0) {
        do {
            (*dst)->start[wordlen - 1] = rand() & 0xFF;
        } while ((*dst)->start[wordlen - 1] == 0);
    }
#endif

    // 불필요한 0 제거
    bi_refine(*dst);

    END_TIMER(result, print_success_gen_rand());
    return result;
}
/*
res bi_get_random(OUT bigint** dst, IN uint32_t sign, IN uint32_t wordlen)
{   
    res result;
    START_TIMER();

    if (wordlen <= 0)
    {
        END_TIMER(result, print_invalid_word_length_error());
        return result;
    }

    bi_new(dst, wordlen);

    (*dst)->sign = sign;

    for (int i = 0; i < wordlen; i++)
    {
        (*dst)->start[i]=0x00;

        for (int j = 1; j < sizeof(word); j++)
        {   
            (*dst)->start[i] |= rand() & 0xFF;
            (*dst)->start[i] <<= 8;
        }
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

    END_TIMER(result, print_success_gen_rand());
    return result;
}
*/

 /**
  * @brief bigint의 값을 다른 bigint로 할당하는 함수
  * @param dst bigint의 포인터
  */
res bi_delete(OUT bigint** dst)
{
    res result;
    START_TIMER();

    if (is_null_pointer(dst) || is_null_pointer(*dst))
    {
        END_TIMER(result, print_null_pointer_error());
        return result;
    }

    free((*dst)->start);
    free(*dst);
    *dst = NULL;

    END_TIMER(result, print_success_memory_deallocation());
    return result;
}

/**
 * @brief bigint에 arrary를 할당하는 함수
 * @param dst bigint의 포인터
 * @param wordlen 배열의 길이
 */
res bi_new(OUT bigint** dst, IN uint32_t wordlen)
{   
    res result;
    START_TIMER();

    if (wordlen <= 0)
    {   
        END_TIMER(result, print_invalid_word_length_error());
        return result;
    }

    if (!is_null_pointer(*dst))
    {
        if (VERBOSE) fprintf(stdout, "[Warning] Already freed pointer.\n");

        bi_delete(dst);
    }

    *dst = (bigint*)malloc(sizeof(bigint));

    if (is_null_pointer(*dst))
    {
        END_TIMER(result, print_memory_allocation_error());
        return result;
    }
    if (*dst != NULL)
    {
        (*dst)->sign = POSITIVE;
        (*dst)->wordlen = wordlen;
        (*dst)->start = (word*)calloc(wordlen, sizeof(word));
    }
    else
    {
        free(*dst);
        END_TIMER(result, print_memory_allocation_error());
        return result;
    }

    END_TIMER(result, print_success_initialization());
    return result;
}

/**
* @memory 값을 copy하는 함수 추가함
*/
res array_copy(OUT word* dst, IN word* src, IN uint32_t wordlen)
{
    res result;
    START_TIMER();

    if (wordlen <= 0)
    {
        END_TIMER(result, print_invalid_word_length_error());
        return result;
    }

    if (is_null_pointer(dst) || is_null_pointer(src))
    {
        END_TIMER(result, print_null_pointer_error());
        return result;
    }

    memcpy(dst, src, sizeof(word) * wordlen);

    END_TIMER(result, print_success_copy());
    return result;
}

/**
 * @brief arrary를 통해 bigint에 값을 할당하는 함수
 * @param dst bigint의 포인터
 * @param sign 부호
 * @param wordlen 배열의 길이
 * @param src 할당하고자 하는 배열
 */
res bi_set_from_array(OUT bigint** dst, IN uint32_t sign, IN uint32_t wordlen, IN word* src)
{
    res result;
    START_TIMER();

    if (wordlen <= 0)
    {
        END_TIMER(result, print_invalid_word_length_error());
        return result;
    }

    if (is_null_pointer(src))
    {
        END_TIMER(result, print_null_pointer_error());
        return result;
    }

    res verification_result = bi_new(dst, wordlen);
    if (verification_result.message != SUCCESS_INITIALIZATION)
    {   
        END_TIMER(result, verification_result.message);
        return result;
    }

    (*dst)->sign = sign;

    verification_result = array_copy((*dst)->start, src, wordlen);
    if (verification_result.message != SUCCESS_COPY)
    {
        END_TIMER(result, verification_result.message);
        return result;
    }

    END_TIMER(result, print_success_set_from_array());
    return result;
}

/**
 * @brief string을 통해 bigint에 값을 할당하는 함수
 * @param dst bigint의 포인터
 * @param str 할당하고자 하는 문자열
 * @param base 10진수, 16진수 등 어떻게 해석할지 의미함.
 */
res bi_set_from_string(OUT bigint** dst, IN char* str, IN uint32_t base)
{   
    res result;
    START_TIMER();

    // 예외 처리: NULL 포인터 또는 잘못된 진수 값 체크
    if (dst == NULL || str == NULL || base != 16)
    {
        END_TIMER(result, print_null_pointer_error());
        return result;
    }
    // 메모리가 이미 할당된 경우, 중복 할당 방지
    if (*dst == NULL)
    {
        res verification_result = bi_new(dst, 1);  // 최소 크기로 먼저 할당
        if (verification_result.message != SUCCESS_INITIALIZATION)
        {
            END_TIMER(result, verification_result.message);
            return result;
        }
    }

    // 부호 확인
    int start_idx = 0;
    if (str[0] == '-')
    {
        (*dst)->sign = 1;  // 음수로 설정
        start_idx = 1;
    }
    else
    {
        (*dst)->sign = 0;  // 양수로 설정
    }

    // '0x' 접두사 처리 (16진수일 경우)
    if (base == 16)
    {
        if (strlen(str + start_idx) < 2 ||  // "0x" 이후 값이 없음
            !(str[start_idx] == '0' && (str[start_idx + 1] == 'x' || str[start_idx + 1] == 'X')))
        {   
            END_TIMER(result, print_invalid_input_error());
            return result;
        }

        start_idx += 2;  // '0x' 건너뛰기
    }

    // 유효한 숫자 문자열인지 확인
    int str_len = strlen(str) - start_idx;
    if (str_len == 0)
    {  // '0x'만 있고 숫자가 없을 때 예외 처리
        END_TIMER(result, print_invalid_input_error());
        return result;
    }
    for (int i = start_idx; i < strlen(str); i++)
    {
        if (!is_hex_digit(str[i]))
        {  // 16진수 문자인지 직접 체크
            END_TIMER(result, print_invalid_input_error());
            return result;
        }
    }

    // 워드 길이 계산
    int wordlen = (str_len + (sizeof(word) * 2 - 1)) / (sizeof(word) * 2);  // 8자리 16진수를 32비트 워드로 변환
    if (*dst == NULL)
    {
        res verification_result = bi_new(dst, wordlen);
        if (verification_result.message != SUCCESS_INITIALIZATION)
        {
            END_TIMER(result, verification_result.message);
            return result;
        }
    }
    else if ((*dst)->wordlen < wordlen)
    {
        free((*dst)->start);
        (*dst)->start = (word*)calloc(wordlen, sizeof(word));
        if ((*dst)->start == NULL)
        {
            END_TIMER(result, print_memory_allocation_error());
            return result;
        }
        (*dst)->wordlen = wordlen;
    }

    // 문자열을 16진수로 변환하여 정순으로 워드 배열에 저장
    int word_idx = 0;
    word temp_value = 0;
    int shift = 0;

    for (int i = str_len - 1; i >= 0; i--)
    {
        char c = str[start_idx + i];
        int digit_value = 0;

        if (is_digit(c))
        {
            digit_value = c - '0';
        }
        else if (c >= 'a' && c <= 'f')
        {
            digit_value = c - 'a' + 10;
        }
        else if (c >= 'A' && c <= 'F')
        {
            digit_value = c - 'A' + 10;
        }

        // 16진수 문자를 변환하고 적절한 비트 위치에 저장
        temp_value |= ((word)digit_value << shift);
        shift += 4;

        // 워드가 다 채워지면 저장
        if (shift == sizeof(word) * 8)
        {
            (*dst)->start[word_idx++] = temp_value;
            temp_value = 0;
            shift = 0;
        }
    }

    // 남은 값이 있으면 마지막 워드에 저장
    if (shift > 0)
    {
        (*dst)->start[word_idx] = temp_value;
    }

    END_TIMER(result, print_success_set_from_string());
    return result;
}

/**
 * @brief bigint에 무작위 값을 할당하는 함수
 * @create_prime이면 마지막 블럭이 all 0가 되는 것을 방지 = 키 크기 최대한 보장
 * @param dst bigint의 포인터
 * @param sign 부호
 * @param byte 배열의 바이트 길이
 */

/*
res bi_get_random(OUT bigint** dst, IN uint32_t sign, IN uint32_t wordlen)
{   
    res result;
    START_TIMER();

    if (wordlen <= 0)
    {
        END_TIMER(result, print_invalid_word_length_error());
        return result;
    }

    bi_new(dst, wordlen);

    (*dst)->sign = sign;

    for (int i = 0; i < wordlen; i++)
    {
        (*dst)->start[i]=0x00;

        for (int j = 1; j < sizeof(word); j++)
        {   
            (*dst)->start[i] |= rand() & 0xFF;
            (*dst)->start[i] <<= 8;
        }
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

    END_TIMER(result, print_success_gen_rand());
    return result;
}
*/

/**
 * @brief bigint의 마지막 워드가 0인 경우 bigint의 메모리를 재할당하는 함수
 * @param dst bigint의 포인터
 */
res bi_refine(OUT bigint* dst)
{
    res result;
    START_TIMER();

    if (is_null_pointer(dst))
    {
        END_TIMER(result, print_null_pointer_error());
        return result;
    }

    while ((dst->start[dst->wordlen - 1] == 0) && (dst->wordlen > 1))
    {
        dst->wordlen--;
    }

    // 새로운 포인터를 받아 기존 포인터를 그대로 유지하는 방식으로 realloc 사용
    word* new_start = (word*)realloc(dst->start, dst->wordlen * sizeof(word));
    
    if (is_null_pointer(new_start))
    {
        END_TIMER(result, print_memory_allocation_error());
        return result;
    }

    // 성공적으로 할당되었으므로 포인터 업데이트
    dst->start = new_start;

    if ((dst->start[0] == 0) && (dst->wordlen == 1))
    {
        dst->sign = POSITIVE;
    }

    END_TIMER(result, print_success_refine());
    return result;
}

/**
 * @brief bigint의 값을 다른 bigint로 할당하는 함수
 * @param dst bigint의 포인터
 * @param src bigint의 포인터
 */
res bi_assign(OUT bigint** dst, IN bigint* src)
{
    res result;
    START_TIMER();

    if (is_null_pointer(src))
    {
        END_TIMER(result, print_null_pointer_error());
        return result;
    }
    if (*dst != NULL)
    {
        bi_delete(dst);
    }
    res verification_result = bi_new(dst, src->wordlen);
    if (verification_result.message != SUCCESS_INITIALIZATION)
    {
        END_TIMER(result, verification_result.message);
        return result;
    }

    (*dst)->sign = src->sign;

    verification_result = array_copy((*dst)->start, src->start, src->wordlen);
    if (verification_result.message != SUCCESS_COPY)
    {
        free_if_exist((void**)*dst);
        END_TIMER(result, verification_result.message);
        return result;
    }

    END_TIMER(result, print_success_assign());
    return result;
}

/**
 * @brief bigint를 출력하는 함수
 * @param dst bigint의 포인터
 * @param base 10진수, 16진수 등 어떻게 해석할지 의미함.
 */
res bi_print(OUT bigint* dst, IN uint32_t base)
{
    res result;
    START_TIMER();

    if (is_null_pointer(dst))
    {
        END_TIMER(result, print_null_pointer_error());
        return result;
    }

    if (base != 16)
    {   
        END_TIMER(result, fprintf(stdout, "Base %d not supported yet.\n", base));
        return result;
    }

    if (dst->sign == NEGATIVE)
    {
        printf("-");
    }

    printf("0x");
    #if word_size == 8
    {
        for (int i = dst->wordlen - 1; i >= 0; i--)
        {
            printf("%02X", dst->start[i]);
        }
    }
    #elif (word_size == 32)
    {
        for (int i = dst->wordlen - 1; i >= 0; i--)
        {
            printf("%08X", dst->start[i]);
        }
    }
    #elif (word_size == 64)
    {
        for (int i = dst->wordlen - 1; i >= 0; i--)
        {
            printf("%016llX", dst->start[i]);
        }
    }
#endif
    printf("\n");

    END_TIMER(result, print_success_print());
    return result;
}

/**
 * @brief bigint가 0인지 확인하는 함수
 * @param dst bigint의 포인터
 * @return 0이면 1, 아니면 0
 */
bool bi_is_zero(OUT bigint* dst)
{
    if (is_null_pointer(dst))
    {
        return print_null_pointer_error();
    }

    if (dst->sign == NEGATIVE)
    {
        return false;
    }

    for (int i = dst->wordlen - 1; i >= 0; i--)
    {
        if (dst->start[i] != 0)
        {
            return false;
        }
    }

    return true;
}

/**
 * @brief bigint가 1인지 확인하는 함수
 * @param dst bigint의 포인터
 * @return 1이면 1, 아니면 0
 */
bool bi_is_one(OUT bigint* dst)
{
    if (dst == NULL) return false;
    if (dst->start[0] != 1) return false;
    for (int i = 1; i < (dst->wordlen); i++)
    {
        if (dst->start[i] != 0) return false;
    }
    return true;
}

/**
 * @brief bigint가 0으로 초기화하는 함수
 * @param dst bigint의 포인터
 */
void bi_set_zero(OUT bigint** dst)
{
    bi_new(dst, 1);
    (*dst)->sign = POSITIVE;
    (*dst)->start[0] = 0;
}

/**
 * @brief bigint A와 B의 절대 값 비교 함수
 * @param dst_A bigint의 포인터
 * @param dst_B bigint의 포인터
 */
int8_t bi_compare_ABS(IN bigint* dst_A, IN bigint* dst_B)
{
    bi_refine(dst_A);
    bi_refine(dst_B);
    int n = dst_A->wordlen;
    int m = dst_B->wordlen;

    // Case: A > B
    if (n > m)
        return 1;

    // Case: A < B
    if (n < m)
        return -1;

    for (int i = n - 1; i >= 0; i--) {

        if (dst_A->start[i] > dst_B->start[i])
            return 1;

        if (dst_A->start[i] < dst_B->start[i])
            return -1;

    }

    return 0;
}

/**
 * @brief bigint A와 B의 값 대소 비교 함수
 * @param dst_A bigint의 포인터
 * @param dst_B bigint의 포인터
 */
int8_t bi_compare(IN bigint* dst_A, IN bigint* dst_B)
{
    if (dst_A->sign == POSITIVE && dst_B->sign == NEGATIVE)
        return 1;

    if (dst_A->sign == NEGATIVE && dst_B->sign == POSITIVE)
        return -1;

    int res = bi_compare_ABS(dst_A, dst_B);

    if (dst_A->sign == POSITIVE)
        return res;

    return res * (-1);
}

/**
 * @brief word를 왼쪽으로 shift하는 함수
 * @param dst bigint의 포인터
 * @param k shift 하고싶은 word 사이즈
 */
res bi_word_left_shift(OUT bigint** dst, IN uint32_t k)
{   
    res result;
    START_TIMER();

    bigint* tmp = NULL ;
    bi_new(&tmp, (*dst)->wordlen + k);
    tmp->sign = (*dst)->sign;

    for (int i = k; i < tmp->wordlen; i++)
    {
        tmp->start[i] = (*dst)->start[i - k];
    }
    //bi_refine(tmp); // dst가 refine된 값이라면 할 필요 x
    // dst를 NULL로 설정하여 bi_assign이 동작하도록 준비

    // bi_assign을 사용하여 tmp의 값을 dst에 복사
    bi_assign(dst, tmp);
    // tmp 및 기존 dst의 메모리를 해제
    bi_delete(&tmp);

    END_TIMER(result, print_success_shift());
    return result;
}

/**
 * @brief bit를 왼쪽으로 shift하는 함수
 * @param dst bigint의 포인터
 * @param k shift 하고싶은 bit 사이즈
 */
res bi_bit_left_shift(OUT bigint** dst, IN uint32_t k)
{
    res result;
    START_TIMER();

    uint32_t word_shift = k / (sizeof(word) * 8);  // 워드 단위 이동
    uint32_t bit_shift = k % (sizeof(word) * 8);   // 워드 내 비트 이동
    bigint* tmp = NULL;

    // 새로운 bigint 생성 (추가 워드 고려)
    bi_new(&tmp, (*dst)->wordlen + word_shift + (bit_shift > 0 ? 1 : 0));
    tmp->sign = (*dst)->sign;

    // 비트 이동 수행
    for (uint32_t i = 0; i < (*dst)->wordlen; i++) {
        // 현재 워드를 이동하고 새로운 위치에 배치
        tmp->start[i + word_shift] |= (*dst)->start[i] << bit_shift;

        // 상위 비트의 초과분을 다음 워드로 전달
        if (bit_shift > 0 && (i ) < (*dst)->wordlen) {
            tmp->start[i + word_shift + 1] |= (*dst)->start[i] >> (sizeof(word) * 8 - bit_shift);
        }
    }
    bi_refine(tmp);
    // 기존 dst를 tmp로 교체
    bi_assign(dst, tmp);
    bi_delete(&tmp);

    END_TIMER(result, print_success_shift());
    return result;
}

/**
 * @brief word를 오른쪽으로 shift하는 함수
 * @param dst bigint의 포인터
 * @param k shift 하고싶은 word 사이즈
 */


/**
 * @brief bit를 오른쪽으로 shift하는 함수
 * @param dst bigint의 포인터
 * @param k shift 하고싶은 bit 사이즈
 */

res bi_word_right_shift(OUT bigint** dst, IN uint32_t k) // k는 shift하고싶은 word 사이즈 의미
{
    res result;
    START_TIMER();

    bigint* tmp = NULL;
    
    if (k >= (*dst)->wordlen)
    {
        bi_new(&tmp, 1);
        tmp->start[0] = 0;
        tmp->sign = POSITIVE;
        bi_assign(dst, tmp);
        bi_delete(&tmp);
        return result;
    }
    else
    {
        bi_new(&tmp, (*dst)->wordlen - k);
    }
    tmp->sign = (*dst)->sign;

    for (int i = 0; i < tmp->wordlen; i++)
    {
        tmp->start[i] = (*dst)->start[i + k];
    }

    //bi_refine(tmp); // dst가 refine된 값이라면 할 필요 x
    // dst를 NULL로 설정하여 bi_assign이 동작하도록 준비

    // bi_assign을 사용하여 tmp의 값을 dst에 복사
    bi_assign(dst, tmp);

    // tmp 및 기존 dst의 메모리를 해제
    bi_delete(&tmp);

    END_TIMER(result, print_success_shift());
    return result;
}


/**
 * @brief bit를 오른쪽으로 shift하는 함수
 * @param dst bigint의 포인터
 * @param k shift 하고싶은 bit 사이즈
 */
res bi_bit_right_shift(OUT bigint** dst, IN uint32_t k)
{
    res result;
    START_TIMER();

    uint32_t word_shift = k / (sizeof(word) * 8);  // 워드 단위 이동
    uint32_t bit_shift = k % (sizeof(word) * 8);   // 워드 내 비트 이동
    bigint* tmp = NULL;

    // 새로운 bigint 생성 (크기를 줄임)
    if (word_shift >= (*dst)->wordlen) {
        bi_new(&tmp, 1);
        tmp->start[0] = 0;
        tmp->sign = POSITIVE;
        bi_assign(dst, tmp);
        bi_delete(&tmp);
    }
    else
    {
        bi_new(&tmp, (*dst)->wordlen - word_shift);
        tmp->sign = (*dst)->sign;
    }

    // 비트 이동 수행
    for (uint32_t i = word_shift; i < (*dst)->wordlen; i++) {
        // 현재 워드를 이동하고 새로운 위치에 배치
        tmp->start[i - word_shift] |= (*dst)->start[i] >> bit_shift;

        // 하위 비트의 초과분을 이전 워드로 전달
        if (bit_shift > 0 && (i > word_shift)) {
            tmp->start[i - word_shift - 1] |= (*dst)->start[i] << (sizeof(word) * 8 - bit_shift);
        }
    }

    // 남은 값이 0인지 확인
    bool is_all_zero = true;
    for (uint32_t i = 0; i < tmp->wordlen; i++) {
        if (tmp->start[i] != 0) {
            is_all_zero = false;
            break;
        }
    }

    // 남은 값이 0이면 wordlen을 1로 하고 start[0]을 0으로 설정
    if (is_all_zero) {
        bi_delete(&tmp); // 기존 tmp 삭제
        bi_new(&tmp, 1); // 새로 1개의 워드로 생성
        tmp->start[0] = 0;
        tmp->sign = POSITIVE; // 0의 부호는 양수로 설정
    }
    else {
        bi_refine(tmp); // 일반적인 refine 수행
    }

    // 기존 dst를 tmp로 교체
    bi_assign(dst, tmp);
    bi_delete(&tmp);

    END_TIMER(result, print_success_shift());
    return result;
}