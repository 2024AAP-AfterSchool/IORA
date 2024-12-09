/**
 * @file bigint_division.h
 * @brief bigint에 대한 나눗셈 연산을 하는 함수들을 정의한 헤더 파일
 * @date 2024-11-22
 * @author 김남일
 */
#ifndef BIGINT_DIVISION_H_
#define BIGINT_DIVISION_H_

#include "base/base_type.h"

res bi_div_bit(OUT bigint** Q, OUT bigint** R, IN bigint* A, IN bigint* B);

res bi_div_word(OUT bigint** Q, IN bigint* A, IN bigint* B);

res bi_div_C(OUT bigint** Q, OUT bigint** R, IN bigint* A, IN bigint* B);

res bi_div_CC(OUT bigint** Q, OUT bigint** R, IN bigint* A, IN bigint* B);

res bi_div(OUT bigint** Q, OUT bigint** R, IN bigint* A, IN bigint* B);

#endif // BIGINT_MULTIPLICATION_H_