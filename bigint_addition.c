#include <stdio.h>
#include "struct.h"
#include "base.h"
typedef uint8_t carry;

void bi_add_ABC(word A, word B, carry c, word* C, carry* c_prime) {
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

void bi_addC(bigint** result, bigint* A, bigint* B) {
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

void bi_add(bigint** result, bigint* a, bigint* b) {

    if ((a->sign == 0) && (b->sign == 1)) {
        bi_sub(result,a,b);
    }
    if ((b->sign == 0) && (a->sign == 1)) {
        bi_sub(result, b, a);
    }
   if ((b->sign == 1) && (a->sign == 1)) {

        if (a->wordlen >= b->wordlen) {
           
            bi_addC(result, a, b);
             (*result)->sign = 1;
        }
        else {
            
            bi_addC(result, b, a);
             (*result)->sign = 1;
        }
    }
    else{
        
        if (a->wordlen >= b->wordlen) {
            bi_addC(result, a, b);
        }
        else {
            bi_addC(result, b, a);
        }
    }
}

