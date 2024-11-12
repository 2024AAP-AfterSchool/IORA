/**
 * @file bigint_multiplication.c
 * @brief bigint에 대한 곱셈 연산을 하는 함수들을 정의한 소스 파일
 * @date 2024-11-08
 * @author 김남일
 */
#include <stdio.h>
#include "../../include/base/base_type.h"
#include "../../include/base/base_error.h"
#include "../../include/bigint/bigint_addition.h"
#include "../../include/bigint/bigint_calculation.h"
#include "../../include/bigint/bigint_multiplication.h"

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
    T0 = T1 + T0; // A1B0 + A0B1 모듈러 덧셈
    T1 = (T0 < T1) ? 1 : 0; // 캐리 계산

    word C1 = A1 * B1;
    word C0 = A0 * B0;
    word tmp = C0;
    C0 = C0 + (T0 << (sizeof(word) * 4));
    C1 = C1 + (T1 << (sizeof(word) * 4)) + (T0 >> (sizeof(word) * 4)) + (C0 < tmp);
    C->start[1] = C1;
    C->start[0] = C0;

    return print_success_mul_AB();
}

/**
 * @brief 두 bigint A와 B의 곱셈을 수행하는 함수
 * @param dst 결과를 저장할 bigint의 포인터
 * @param A 곱셈을 수행할 첫 번째 bigint
 * @param B 곱셈을 수행할 두 번째 bigint
 */
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