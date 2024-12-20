/**
 * @file bigint_calculation.h
 * @brief bigint에 대한 연산을 하는 함수들을 정의한 헤더 파일
 * @date 2024-10-18
 * @author 조동후
 */
#ifndef BIGINT_CALCULATION_H_
#define BIGINT_CALCULATION_H_

#include "base/base_type.h"

// bigint의 메모리를 할당하는 함수
res bi_new(OUT bigint** dst, IN uint32_t wordlen);

// bigint를 삭제하는 함수
// 삭제 시에는 값을 모두 0으로 초기화하는 ZERORIZE를 사용함.
res bi_delete(OUT bigint** dst);

// bigint에 arrary를 할당하는 함수
// sign은 부호, wordlen은 배열의 길이, a는 배열을 의미함.
res bi_set_from_array(OUT bigint** dst, IN uint32_t sign, IN uint32_t wordlen, IN word* src);

// bigint에 string을 할당하는 함수
// int_str은 문자열, base는 10진수, 16진수 등 어떻게 해석할지 의미함.
// string이 올바르지 않은 경우를 예외처리 해야함.
res bi_set_from_string(OUT bigint** dst, IN char* str, IN uint32_t base);

// random 값으로 생성한 배열의 가장 마지막 인덱스는 0이 되어서는 안됨.
// 0일 경우 wordlen보다 작은 값이 나옴.
res bi_get_random(OUT bigint** dst, IN uint32_t sign, IN uint32_t byte);

// bigint의 마지막 워드가 0인 경우 bigint의 메모리를 재할당하는 함수
res bi_refine(OUT bigint* dst);

// bigint의 값을 다른 bigint로 할당하는 함수
res bi_assign(OUT bigint** dst, IN bigint* src);

// bigint를 출력하는 함수
// 구조체 포인터를 사용하는 이유는 구조체를 복사하는 것보다 포인터를 사용하는 것이 더 메모리를 절약할 수 있음.
// base는 10진수, 16진수 등 어떻게 해석할지 의미함.
// 나눗셈까지 구현을 해야 print를 구현할 수 있음.
res bi_print(OUT bigint* dst, IN uint32_t base);

res array_copy(OUT word* dst, IN word* src, IN uint32_t wordlen);

//value가 0인지 확인하는 함수
bool bi_is_one(OUT bigint* dst);

//value가 1인지 확인하는 함수
bool bi_is_zero(OUT bigint* dst);

// set zero 확인하는 함수
void bi_set_zero(OUT bigint** dst);

// 두 큰수 비교하는 함수
int8_t bi_compare_ABS(IN bigint* dst_A, IN bigint* dst_B);
int8_t bi_compare(IN bigint* dst_A, IN bigint* dst_B);

// bigint를 shift하는 함수
res bi_word_left_shift(OUT bigint** dst, IN uint32_t k);
res bi_word_right_shift(OUT bigint** dst, IN uint32_t k);

res bi_bit_left_shift(OUT bigint** dst, IN uint32_t k);
res bi_bit_right_shift(OUT bigint** dst, IN uint32_t k);

#endif // BIGINT_CALCULATION_H_