#ifndef BIGINT_ADDITION_H_
#define BIGINT_ADDITION_H_

#include "base/base_type.h"   // 기본 함수 및 타입 정의

// ADD_ABC 함수: A, B, c의 덧셈을 수행하고 C에 저장, 캐리를 c_prime에 저장
void bi_add_ABC(IN word A, IN word B, carry c, OUT word* C, carry* c_prime);

// ADD_C 함수: 두 bigint A와 B의 덧셈을 수행하고, 결과를 result에 저장
void bi_add_C(OUT bigint** result, IN bigint* A, IN bigint* B);

// ADD 함수: A와 B의 덧셈을 수행하며, A와 B의 길이에 따라 ADDC를 호출
void bi_add(OUT bigint** result, IN bigint* a, IN bigint* b);

#endif // BIGINT_ADDITION_H_