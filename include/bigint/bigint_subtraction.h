#ifndef BIGINT_SUBTRACTION_H_
#define BIGINT_SUBTRACTION_H_

#include "base/base_type.h"   // 기본 함수 및 타입 정의

// SUB_ABC 함수: A, B, borrow의 뺄셈을 수행하고 C에 저장, borrow를 borrow_prime에 저장
res bi_sub_ABb(OUT word* dst, IN word* A, IN word* B, byte* borrow);

// SUB_Ab 함수: A, borrow의 뺄셈을 수행하고 C에 저장, borrow를 borrow_prime에 저장
res bi_sub_Ab(OUT word* dst, IN word* A, IN byte* borrow);

// SUB 함수: 두 bigint A와 B의 뺄셈을 수행하고, 결과를 dst에 저장
res bi_sub_C(OUT bigint** dst, IN bigint* A, IN bigint* B);	

// SUB 함수: 두 bigint A와 B의 뺄셈을 수행하며, 길이에 따라 SUBC를 호출
res bi_sub(OUT bigint** dst, IN bigint* A, IN bigint* B);

// SUB 테스트 함수: bigint 뺄셈 테스트 함수
void bi_test_sub();

#endif // BIGINT_SUBTRACTION_H_