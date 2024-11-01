#include <stdio.h>
#include "base/base_type.h"
#include "bigint/bigint_calculation.h"

void bi_add_ABC(IN word A, IN word B, carry c, OUT word* C, carry* c_prime) {
    *c_prime = 0;  // 초기 캐리를 0으로 설정
    *C = A + B;
    if (*C < A) {
        *c_prime = 1;
    }
    *C += c;
    if (*C < c) {
        *c_prime += 1;
    }
}

void bi_add_C(OUT bigint** result, IN bigint* A, IN bigint* B) {
    if (A == NULL || A->start == NULL) {
        printf("Error: A is not properly initialized.\n");
        return;
    }
    if (B == NULL || B->start == NULL) {
        printf("Error: B is not properly initialized.\n");
        return;
    }

    int n = A->wordlen;
    int m = B->wordlen;

    bi_new(result, n + 1);  // 최대 n + 1 자리 (자리 올림 고려)

    carry c = 0;
    word C_word;

    for (int j = 0; j < n; j++) {
        word A_word = A->start[j];
        word B_word;
        if (j < m) {
            B_word = B->start[j];  // B의 길이 내에 있으면 B의 j번째 워드를 할당
        }
        else {
            B_word = 0;        // B의 길이를 넘는 인덱스에서는 0을 할당
        }

        bi_add_ABC(A_word, B_word, c, &C_word, &c);
        (*result)->start[j] = C_word;
    }

    (*result)->start[n] = c;
    if (c == 0) {
        (*result)->wordlen = n;     // 자리 올림이 없으면 n 길이 사용
    }
    else {
        (*result)->wordlen = n + 1; // 자리 올림이 있으면 n + 1 길이 사용
    }
    bi_refine(*result);
}

void bi_add(OUT bigint** result, IN bigint* a, IN bigint* b) {
    // 덧셈만 수행하도록 간단하게 처리
    if (a->wordlen >= b->wordlen) {
        bi_add_C(result, a, b);
    }
    else {
        bi_add_C(result, b, a);
    }
}