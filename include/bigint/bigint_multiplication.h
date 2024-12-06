#ifndef BIGINT_MULTIPLICATION_H_
#define BIGINT_MULTIPLICATION_H_

#include "base/base_type.h"   // 기본 함수 및 타입 정의

// MUL_AB 함수: 두 word A와 B의 곱셈을 수행하고, 결과를 C에 저장
res bi_mul_AB(OUT bigint** dst, IN word* A, IN word* B);

// MUL_C 함수: 두 bigint A와 B의 곱셈을 수행하고, 결과를 result에 저장
res bi_mul_C(OUT bigint** dst, IN bigint* A, IN bigint* B);

// MUL_KARATSUBA 함수: 카라추바 곱셈을 수행하는 함수
res bi_mul_karatsuba(OUT bigint** dst, IN bigint* A, IN bigint* B);

// MUL 함수: 두 bigint A와 B의 곱셈을 수행하는 함수
res bi_mul(OUT bigint** dst, IN bigint* A, IN bigint* B, IN bool is_karatsuba);

// MUL test 함수: bigint 곱셈 테스트 함수
void bi_test_mul();

// SQUARE_A 함수: bigint A의 제곱을 수행하고, 결과를 dst에 저장
res bi_square_AA(OUT bigint** dst, IN bigint* A);

// SQUARE_C 함수: bigint X의 제곱을 수행하고, 결과를 dst에 저장(내부적으로 SQUARE_A 함수 호출)
res bi_square_C(OUT bigint** dst, IN bigint* X); 

// SQUARE_A 함수: bigint A의 제곱을 수행하고, 결과를 dst에 저장(재귀적으로 호출)
res bi_square_karatsuba(OUT bigint** dst, IN bigint* A);

// SQUARE 함수: bigint A의 제곱을 수행하는 함수
res bi_square(OUT bigint** dst, IN bigint* A, IN bool is_karatsuba);

#endif // BIGINT_MULTIPLICATION_H_