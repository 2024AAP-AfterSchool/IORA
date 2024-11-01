#include <stdio.h>
#include "struct.h"
#include "base.h"
typedef uint8_t carry;

void add_ABC(word A, word B, carry c, word* C, carry* c_prime) {
    *c_prime = 0;  // �ʱ� ĳ���� 0���� ����
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

    bi_new(result, n + 1);  // �ִ� n + 1 �ڸ� (�ڸ� �ø� ���)

    carry c = 0;
    word C_word;

    for (int j = 0; j < n; j++) {
        word A_word = A->a[j];
        word B_word;
        if (j < m) {
            B_word = B->a[j];  // B�� ���� ���� ������ B�� j��° ���带 �Ҵ�
        }
        else {
            B_word = 0;        // B�� ���̸� �Ѵ� �ε��������� 0�� �Ҵ�
        }

        add_ABC(A_word, B_word, c, &C_word, &c);
        (*result)->a[j] = C_word;
    }

    (*result)->a[n] = c;
    if (c == 0) {
        (*result)->word_len = n;     // �ڸ� �ø��� ������ n ���� ���
    }
    else {
        (*result)->word_len = n + 1; // �ڸ� �ø��� ������ n + 1 ���� ���
    }
    bi_refine(*result);
}

void add(bigint** result, bigint* a, bigint* b) {
    // ������ �����ϵ��� �����ϰ� ó��
    if (a->word_len >= b->word_len) {
        addC(result, a, b);
    }
    else {
        addC(result, b, a);
    }
}

