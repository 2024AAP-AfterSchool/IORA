#ifndef BIGINT_SUBTRACTION_H_
#define BIGINT_SUBTRACTION_H_

#include "base/base_type.h"   // 기본 함수 및 타입 정의

void bi_sub_ABb(IN word* A, IN word* B, byte* b, OUT word* C);

void bi_sub_Ab(IN word* A, byte* b, word* C);

void bi_sub_C(IN bigint* A, IN bigint* B, OUT bigint** z);	

void bi_sub(IN bigint* x, IN bigint* y, OUT bigint** z);

#endif // BIGINT_SUBTRACTION_H_