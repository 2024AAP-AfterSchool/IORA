/**
 * @file bigint_exponential.h
 * @brief bigint에 대한 지수 연산을 하는 함수들을 정의한 헤더 파일
 * @date 2024-11-29
 * @author 송원우, 김남일
 */
#ifndef BIGINT_EXPONENTIAL_H_
#define BIGINT_EXPONENTIAL_H_

#include "base/base_type.h"   // 기본 함수 및 타입 정의

// LEFT_TO_RIGHT 함수: Left-to-right 방식의 거듭제곱 연산을 수행하는 함수
res bi_exp_left_to_right(OUT bigint** dst, IN bigint* A, IN bigint* exp);

// RIGHT_TO_LEFT 함수: Right-to-left 방식의 거듭제곱 연산을 수행하는 함수
res bi_exp_right_to_left(OUT bigint** dst, IN bigint* A, IN bigint* exp);

// EXP_MON 함수: Multiply-and-Square(Montgomery) 방식의 거듭제곱 연산을 수행하는 함수
res bi_exp_montgomery(OUT bigint** dst, IN bigint* A, IN bigint* exp);

// LEFT_TO_RIGHT_MOD 함수: Left-to-right 방식의 거듭제곱 연산을 수행하면서 나머지 연산을 수행하는 함수
res bi_exp_left_to_right_mod(OUT bigint** dst, IN bigint* A, IN bigint* exp, IN bigint* mod);

// RIGHT_TO_LEFT_MOD 함수: Right-to-left 방식의 거듭제곱 연산을 수행하면서 나머지 연산을 수행하는 함수
res bi_exp_right_to_left_mod(OUT bigint** dst, IN bigint* A, IN bigint* exp, IN bigint* mod);

// EXP_MON_MOD 함수: Multiply-and-Square(Montgomery) 방식의 거듭제곱 연산을 수행하면서 나머지 연산을 수행하는 함수
res bi_exp_montgomery_mod(OUT bigint** dst, IN bigint* A, IN bigint* exp, IN bigint* mod);

#endif // BIGINT_EXPONENTIAL_H_