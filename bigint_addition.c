#include <stdio.h>
#include "struct.h"
#include "base.h"
typedef uint8_t carry;

void add_ABC(word A, word B, carry c, word* C, carry* c_prime) {
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

void addC(bigint** result, bigint* A, bigint* B) {
    if (A == NULL || A->a == NULL) {
        printf("Error: A is not properly initialized.\n");
        return;
    }
    if (B == NULL || B->a == NULL) {
        printf("Error: B is not properly initialized.\n");
        return;
    }

    int n = A->word_len;
    int m = B->word_len;

    bi_new(result, n + 1);  // 최대 n + 1 자리 (자리 올림 고려)

    carry c = 0;
    word C_word;

    for (int j = 0; j < n; j++) {
        word A_word = A->a[j];
        word B_word;
        if (j < m) {
            B_word = B->a[j];  // B의 길이 내에 있으면 B의 j번째 워드를 할당
        }
        else {
            B_word = 0;        // B의 길이를 넘는 인덱스에서는 0을 할당
        }

        add_ABC(A_word, B_word, c, &C_word, &c);
        (*result)->a[j] = C_word;
    }

    (*result)->a[n] = c;
    if (c == 0) {
        (*result)->word_len = n;     // 자리 올림이 없으면 n 길이 사용
    }
    else {
        (*result)->word_len = n + 1; // 자리 올림이 있으면 n + 1 길이 사용
    }
    bi_refine(*result);
}

void add(bigint** result, bigint* a, bigint* b) {
    // 덧셈만 수행하도록 간단하게 처리
    if (a->word_len >= b->word_len) {
        addC(result, a, b);
    }
    else {
        addC(result, b, a);
    }
}

