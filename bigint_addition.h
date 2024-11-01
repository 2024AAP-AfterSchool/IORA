#ifndef BIGINT_ADDITION_H
#define BIGINT_ADDITION_H

#include "struct.h"  // bigint 구조체 정의
#include "base.h"    // 기본 함수 및 타입 정의

// 캐리(carry) 타입 정의
typedef uint8_t carry;

// ADD_ABC 함수: A, B, c의 덧셈을 수행하고 C에 저장, 캐리를 c_prime에 저장
void add_ABC(word A, word B, carry c, word* C, carry* c_prime);

// ADDC 함수: 두 bigint A와 B의 덧셈을 수행하고, 결과를 result에 저장
void addC(bigint** result, bigint* A, bigint* B);

// ADD 함수: A와 B의 덧셈을 수행하며, A와 B의 길이에 따라 ADDC를 호출
void add(bigint** result, bigint* a, bigint* b);

#endif // BIGINT_ADDITION_H