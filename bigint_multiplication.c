/**
 * @file bigint_multiplication.c
 * @brief bigint에 대한 곱셈 연산을 하는 함수들을 정의한 소스 파일
 * @date 2024-11-08
 * @author 김남일
 */
#include <stdio.h>
#include "struct.h"
#include "error.h"
#include "bigint_addition.h"
#include "base.h"
#include "bigint_multiplication.h"
#include <stdbool.h>

 /**
  * @brief 두 양의 정수 A와 B의 곱셈을 수행하는 함수
  * @param dst 결과를 저장할 bigint의 포인터
  * @param A 곱셈을 수행할 첫 번째 bigint
  * @param B 곱셈을 수행할 두 번째 bigint
  */

msg bi_mul_AB(OUT bigint** dst, IN word* A, IN word* B)
{
    bigint* C = NULL;
    bi_new(&C, 2);

    // 32비트와 64비트 word 크기에 따른 상수값
    word mask = (sizeof(word) == 4) ? 0xFFFF : 0xFFFFFFFF;

    // A의 상위와 하위 비트 추출
    word A1 = *A >> (sizeof(word) * 4); // 상위 비트 추출
    word A0 = *A & mask;                // 하위 비트 추출

    // B의 상위와 하위 비트 추출
    word B1 = *B >> (sizeof(word) * 4); // 상위 비트 추출
    word B0 = *B & mask;                // 하위 비트 추출

    // A1B0 + A0B1 계산
    word T1 = A1 * B0;
    word T0 = A0 * B1;
    T0 = T1 + T0;              // A1B0 + A0B1 덧셈
    T1 = (T0 < T1) ? 1 : 0;    // 캐리 계산

    // 최종 곱셈 결과 계산
    word C1 = A1 * B1;
    word C0 = A0 * B0;
    word tmp = C0;

    C0 = C0 + (T0 << (sizeof(word) * 4));
    C1 = C1 + (T1 << (sizeof(word) * 4)) + (T0 >> (sizeof(word) * 4)) + (C0 < tmp);

    // C에 C1과 C0 값을 합쳐 설정
    C->start[0] = C0;
    C->start[1] = C1;

    *dst = C;

    return print_success_mul_AB();
}


msg bi_mul_C(OUT bigint** dst, IN bigint* A, IN bigint* B)
{
    bigint* C = NULL;
    bi_new(&C, A->wordlen + B->wordlen);

    for (int i = 0; i < B->wordlen; i++)
    {
        bigint* T0 = NULL;
        bigint* T1 = NULL;
        bi_new(&T0, 2); // T0는 2 워드 크기로 생성
        bi_new(&T1, 2); // T1도 2 워드 크기로 생성

        for (int j = 0; j < (A->wordlen) / 2; j++)
        {
            msg result;
            result = bi_mul_AB(&T0, &(A->start[2 * j]), &(B->start[i]));
            if (result != SUCCESS_MUL_AB) return print_fail_mul_AB();
            bi_word_left_shift(T0, 2 * j);

            result = bi_mul_AB(&T1, &(A->start[(2 * j) + 1]), &(B->start[i]));
            if (result != SUCCESS_MUL_AB) return print_fail_mul_AB();
            bi_word_left_shift(T1, (2 * j) + 1);
        }

        bigint* T = NULL;
        msg result = bi_add(&T, T0, T1);
        if (result != SUCCESS_MUL_AB) return print_fail_mul_AB();
        bi_word_left_shift(T, i);
        result = bi_add(&C, C, T);
        if (result != SUCCESS_MUL_AB) return print_fail_mul_AB();

        // 임시 변수 삭제
        bi_delete(&T0);
        bi_delete(&T1);
        bi_delete(&T);
    }

    bi_refine(C);
    bi_assign(dst, C); // 최종 결과를 dst에 할당
    bi_delete(&C); // C 삭제

    return print_success_mul_C();
}





msg karatsuba_mul(OUT bigint** dst, IN bigint* A, IN bigint* B) {
    // Ensure A and B have the same word length by padding the smaller one with zeros
    if (A->wordlen != B->wordlen) {
        uint32_t max_len = (A->wordlen > B->wordlen) ? A->wordlen : B->wordlen;
        if (A->wordlen < max_len) {
            bigint* padded_A = NULL;
            if (bi_new(&padded_A, max_len) != SUCCESS_MEMORY_ALLOCATION) {
                fprintf(stderr, "Memory allocation error for padded_A\n");
                return MEMORY_ALLOCATION_ERROR;
            }
            for (uint32_t i = 0; i < A->wordlen; i++) {
                padded_A->start[i] = A->start[i];
            }
            for (uint32_t i = A->wordlen; i < max_len; i++) {
                padded_A->start[i] = 0;
            }
            A = padded_A;
        }
        if (B->wordlen < max_len) {
            bigint* padded_B = NULL;
            if (bi_new(&padded_B, max_len) != SUCCESS_MEMORY_ALLOCATION) {
                fprintf(stderr, "Memory allocation error for padded_B\n");
                return MEMORY_ALLOCATION_ERROR;
            }
            for (uint32_t i = 0; i < B->wordlen; i++) {
                padded_B->start[i] = B->start[i];
            }
            for (uint32_t i = B->wordlen; i < max_len; i++) {
                padded_B->start[i] = 0;
            }
            B = padded_B;
        }
    }

    uint32_t n = (A->wordlen > B->wordlen) ? A->wordlen : B->wordlen;
    uint32_t half = (n + 1) / 2;

    
    bigint* A1 = NULL, * A0 = NULL, * B1 = NULL, * B0 = NULL;
    if (bi_new(&A1, half) != SUCCESS_MEMORY_ALLOCATION ||
        bi_new(&A0, half) != SUCCESS_MEMORY_ALLOCATION ||
        bi_new(&B1, half) != SUCCESS_MEMORY_ALLOCATION ||
        bi_new(&B0, half) != SUCCESS_MEMORY_ALLOCATION) {
        fprintf(stderr, "Error initializing memory for splitting A and B\n");
        return MEMORY_ALLOCATION_ERROR;
    }

    for (uint32_t i = 0; i < half; i++) {
        A0->start[i] = A->start[i];
        B0->start[i] = B->start[i];
    }
    for (uint32_t i = half; i < A->wordlen; i++) {
        A1->start[i - half] = A->start[i];
    }
    for (uint32_t i = half; i < B->wordlen; i++) {
        B1->start[i - half] = B->start[i];
    }

    
    bigint* T1 = NULL, * T0 = NULL, * T2 = NULL;
    if (karatsuba_mul(&T1, A1, B1) != SUCCESS_MUL_C) {
        fprintf(stderr, "Error calculating T1\n");
        return FAIL_MUL_AB;
    }
    if (karatsuba_mul(&T0, A0, B0) != SUCCESS_MUL_C) {
        fprintf(stderr, "Error calculating T0\n");
        return FAIL_MUL_AB;
    }

   
    bigint* S1 = NULL, * S2 = NULL;
    if (bi_add(&S1, A1, A0) != SUCCESS_ADD || bi_add(&S2, B1, B0) != SUCCESS_ADD) {
        fprintf(stderr, "Error calculating S1 or S2\n");
        return INVALID_INPUT_ERROR;
    }
    if (karatsuba_mul(&T2, S1, S2) != SUCCESS_MUL_C) {
        fprintf(stderr, "Error calculating T2\n");
        return FAIL_MUL_AB;
    }

   
    if (bi_sub(&T2, T2, T1) != SUCCESS_SUB || bi_sub(&T2, T2, T0) != SUCCESS_SUB) {
        fprintf(stderr, "Error calculating T2 subtraction\n");
        return INVALID_INPUT_ERROR;
    }

    
    if (bi_word_left_shift(&T1, 2 * half) != SUCCESS_SHIFT ||
        bi_word_left_shift(&T2, half) != SUCCESS_SHIFT) {
        fprintf(stderr, "Error during shifting T1 or T2\n");
        return INVALID_INPUT_ERROR;
    }

  
    bigint* result = NULL;
    if (bi_add(&result, T1, T2) != SUCCESS_ADD || bi_add(&result, result, T0) != SUCCESS_ADD) {
        fprintf(stderr, "Error combining results\n");
        return INVALID_INPUT_ERROR;
    }

    *dst = result;
    bi_delete(&A1);
    bi_delete(&A0);
    bi_delete(&B1);
    bi_delete(&B0);
    bi_delete(&T1);
    bi_delete(&T0);
    bi_delete(&T2);
    bi_delete(&S1);
    bi_delete(&S2);

    return SUCCESS_MUL_C;
}
msg bi_test_mul()
{
    fprintf(stdout, "1. 기본 곱셈 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "1.1 두 양의 정수 A와 B의 곱\n");
    // Implement test logic for positive * positive

    fprintf(stdout, "1.2 두 음의 정수 A와 B의 곱\n");

    fprintf(stdout, "1.3 양의 정수 A와 음의 정수 B의 곱\n");
    // Implement test logic for positive * negative

    fprintf(stdout, "1.4 음의 정수 A와 양의 정수 B의 곱\n");
    // Implement test logic for negative * positive

    fprintf(stdout, "\n2. 0과의 곱셈 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "2.1 A * 0 (결과는 0)\n");
    fprintf(stdout, "2.2 0 * B (결과는 0)\n");
    fprintf(stdout, "2.3 0 * 0 (결과는 0)\n");

    fprintf(stdout, "\n3. 자리 올림 발생 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "3.1 자리 올림이 발생하는 경우\n");
    // Implement test case logic for carry

    fprintf(stdout, "3.2 연속적인 자리 올림\n");
    // Implement test case logic for multiple carries
}