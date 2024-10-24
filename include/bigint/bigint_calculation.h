#ifndef BIGINT_CALCULATION_H_
#define BIGINT_CALCULATION_H_

#include "base/base_type.h"

// bigint의 메모리를 할당하는 함수
msg bi_new(bigint** dst, uint32_t wordlen);

// bigint를 삭제하는 함수
// 삭제 시에는 값을 모두 0으로 초기화하는 ZERORIZE를 사용함.
msg bi_delete(bigint** dst);

// bigint에 arrary를 할당하는 함수
// sign은 부호, wordlen은 배열의 길이, a는 배열을 의미함.
msg bi_set_from_array(bigint** dst, uint32_t sign, uint32_t wordlen, word* src);

// bigint에 string을 할당하는 함수
// int_str은 문자열, base는 10진수, 16진수 등 어떻게 해석할지 의미함.
// string이 올바르지 않은 경우를 예외처리 해야함.
msg bi_set_from_string(bigint** dst, char* str, uint32_t base);

// random 값으로 생성한 배열의 가장 마지막 인덱스는 0이 되어서는 안됨.
// 0일 경우 wordlen보다 작은 값이 나옴.
msg bi_get_random(bigint** dst, uint32_t sign, uint32_t wordlen);

// bigint의 마지막 워드가 0인 경우 bigint의 메모리를 재할당하는 함수
msg bi_refine(bigint* dst);

// bigint의 값을 다른 bigint로 할당하는 함수
msg bi_assign(bigint** dst, bigint* src);

// bigint를 출력하는 함수
// 구조체 포인터를 사용하는 이유는 구조체를 복사하는 것보다 포인터를 사용하는 것이 더 메모리를 절약할 수 있음.
// base는 10진수, 16진수 등 어떻게 해석할지 의미함.
// 나눗셈까지 구현을 해야 print를 구현할 수 있음.
msg bi_print(bigint* dst, uint32_t base);

msg array_copy(word* dst, word* src, uint32_t wordlen);

#endif // BIGINT_CALCULATION_H_