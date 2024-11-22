/**
 * @file bigint_multiplication.c
 * @brief bigint에 대한 곱셈 연산을 하는 함수들을 정의한 소스 파일
 * @date 2024-11-08
 * @author 김남일
 */
#include <stdio.h>
#include "base/base_type.h"
#include "base/base_error.h"
#include "bigint/bigint_addition.h"
#include "bigint/bigint_subtraction.h"
#include "bigint/bigint_calculation.h"
#include "bigint/bigint_multiplication.h"

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

    word mask = (sizeof(word) == 4) ? 0xFFFF : 0xFFFFFFFF;

    // A의 상위와 하위 비트 추출
    word A1 = *A >> (sizeof(word) * 4); // 상위 비트 추출
    word A0 = *A & mask;                // 하위 비트 추출

    // B의 상위와 하위 비트 추출
    word B1 = *B >> (sizeof(word) * 4); // 상위 비트 추출
    word B0 = *B & mask;                // 하위 비트 추출
    
    // A1B0+A0B1 계산하는 부분
    word T1 = A1 * B0;
    word T0 = A0 * B1;
    T0 = T1 + T0;                //A1B0+A0B1 모듈러덧셈
    T1 = (T0 < T1) ? 1 : 0;      //carry

    word C1 = A1 * B1;
    word C0 = A0 * B0;
    word tmp = C0;

    C0 = C0 + (T0 << (sizeof(word) * 4));
    C1 = C1 + (T1 << (sizeof(word) * 4)) + (T0 >> (sizeof(word) * 4)) + (C0 < tmp);
    C->start[0] = C0;
    C->start[1] = C1;

    // 초기화 함수
    bi_assign(dst, C);
    bi_delete(&C);

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
    bigint* T = NULL;
    bi_new(&C, A->wordlen + B->wordlen);
    bi_new(&C, A->wordlen + 1);
    bigint* tmp_A = NULL;

    // A가 홀수 길이인 경우 확장
    if (A->wordlen % 2 == 1) {
        bi_new(&tmp_A, A->wordlen + 1);
        for (int i = 0; i < A->wordlen; i++) {
            tmp_A->start[i] = A->start[i];
        }
        tmp_A->start[A->wordlen] = 0;
    }
    else {
        bi_assign(&tmp_A, A);
    }

    // 곱셈 및 결과 누적
    for (int i = 0; i < B->wordlen; i++) {
        bigint* T0 = NULL;
        bigint* T1 = NULL;
        bigint* tmp_T0 = NULL;
        bigint* tmp_T1 = NULL;
        bi_new(&T0, 2); // T0는 2 워드 크기로 생성
        bi_new(&T1, 2); // T1도 2 워드 크기로 생성
        bi_new(&tmp_T0, (tmp_A->wordlen));
        bi_new(&tmp_T1, (tmp_A->wordlen)+1);

        for (int j = 0; j < (tmp_A->wordlen / 2); j++) {
            msg result;

            result = bi_mul_AB(&T0, &(tmp_A->start[2 * j]), &(B->start[i]));
            if (result != SUCCESS_MUL_AB) return print_fail_mul_AB();
            tmp_T0->start[2 * j] = T0->start[0];
            tmp_T0->start[(2 * j)+1] = T0->start[1];
            
            result = bi_mul_AB(&T1, &(tmp_A->start[(2 * j) + 1]), &(B->start[i]));
            if (result != SUCCESS_MUL_AB) return print_fail_mul_AB();
            tmp_T1->start[(2 * j)+1] = T1->start[0];
            tmp_T1->start[(2 * (j+1))] = T1->start[1];
        }
        bi_add(&T, tmp_T1, tmp_T0);
        // i번째 워드에 대한 시프트 처리 후 누적 합 산
        bi_word_left_shift(&T, i);
        bi_add(&C, C, T);

        // 임시 변수 삭제
        bi_delete(&T0);
        bi_delete(&T1); 
        bi_delete(&tmp_T0);
        bi_delete(&tmp_T1);
    }

    // 최종 결과 할당
    bi_refine(C);
    bi_assign(dst, C);
    bi_delete(&C);
    bi_delete(&tmp_A);

    return print_success_mul_C();
}

msg bi_mul_karatsuba(OUT bigint** dst, IN bigint* A, IN bigint* B) {

    bigint* tmp_A = NULL;
    bigint* tmp_B = NULL;
  

    bi_assign(&tmp_A, A);
    bi_assign(&tmp_B, B);
 
    uint32_t n = (tmp_A->wordlen > B->wordlen) ? tmp_A->wordlen : tmp_B->wordlen;


    if (n == 1) {
        return bi_mul_C(dst, tmp_A, tmp_B);
    }

    uint32_t half = (n + 1) / 2;

  
    bigint* A1 = NULL, * A0 = NULL, * B1 = NULL, * B0 = NULL;

   
    bi_new(&A1, n - half);
    bi_new(&A0, half);
    for (uint32_t i = 0; i < half; i++) {
        A0->start[i] = (i < tmp_A->wordlen) ? tmp_A->start[i] : 0;
    }
    for (uint32_t i = half; i < n; i++) {
        A1->start[i - half] = (i < tmp_A->wordlen) ? tmp_A->start[i] : 0;
    }

  
    bi_new(&B1, n - half);
    bi_new(&B0, half);
    for (uint32_t i = 0; i < half; i++) {
        B0->start[i] = (i < tmp_B->wordlen) ? tmp_B->start[i] : 0;
    }
    for (uint32_t i = half; i < n; i++) {
        B1->start[i - half] = (i < tmp_B->wordlen) ? tmp_B->start[i] : 0;
    }

    
    bigint* T1 = NULL, * T0 = NULL, * T2 = NULL;
    bi_mul_karatsuba(&T1, A1, B1); // T1 = A1 * B1
    T1->sign = A1->sign ^ B1->sign;
    bi_mul_karatsuba(&T0, A0, B0); // T0 = A0 * B0
    T0->sign = A0->sign ^ B0->sign;

    //S1 = A0 - A1, S2 = B1 - B0
    bigint* S1 = NULL, * S2 = NULL;
    bi_sub(&S1, A0, A1);
    bi_sub(&S2, B1, B0);

    //T2 = S1 * S2
    bi_mul_karatsuba(&T2, S1, S2);
    T2->sign = S1->sign ^ S2->sign;
    // T2: T2 = T2 + T1 + T0
    bi_add(&T2, T2, T1);
    bi_add(&T2, T2, T0);

   
    bi_word_left_shift(&T1, 2 * half);
    bi_word_left_shift(&T2, half);

    bigint* result = NULL;
    bi_add(&result, T1, T2);
    bi_add(&result, result, T0);

    result->sign = A->sign ^ B->sign;

    // Assign result to dst
    bi_refine(result);
    bi_assign(dst, result);


    bi_delete(&A1); bi_delete(&A0);
    bi_delete(&B1); bi_delete(&B0);
    bi_delete(&T1); bi_delete(&T0); bi_delete(&T2);
    bi_delete(&S1); bi_delete(&S2);
    bi_delete(&result);

    return SUCCESS_MUL_C;
}

/**
 * @brief 두 bigint A와 B의 곱셈을 수행하는 함수
 * @param dst 결과를 저장할 bigint의 포인터
 * @param A 곱셈을 수행할 첫 번째 bigint
 * @param B 곱셈을 수행할 두 번째 bigint
 * @param is_karatsuba 카라추바 곱셈을 사용할지 여부
 */
msg bi_mul(OUT bigint** dst, IN bigint* A, IN bigint* B, IN bool is_karatsuba)
{
    bigint* tmp_A = NULL;
    bigint* tmp_B = NULL;
    
    bi_assign(&tmp_A, A);
    bi_assign(&tmp_B, B);

    if (bi_is_zero(tmp_A) || bi_is_zero(tmp_B))
    {
        bi_new(dst, 1);
        (*dst)->start[0] = 0;
        
        bi_delete(&tmp_A);
        bi_delete(&tmp_B);

        return print_success_mul();
    }
    if (bi_is_one(tmp_A))
    {
        bi_assign(dst, tmp_B);
        (*dst)->sign = (tmp_A->sign ^ tmp_B->sign);
        
        bi_delete(&tmp_A);
        bi_delete(&tmp_B);

        return print_success_mul();
    }
    if (bi_is_one(tmp_B))
    {
        bi_assign(dst, tmp_A);
        (*dst)->sign = (tmp_A->sign ^ tmp_B->sign);
        
        bi_delete(&tmp_A);
        bi_delete(&tmp_B);

        return print_success_mul();
    }

    //카라추바에 양수 곱셈 넣고 부호는 따로 저장하기 위함
    byte A_sign = tmp_A->sign;
    byte B_sign = tmp_B->sign;
    tmp_A->sign = POSITIVE;

    if ( is_karatsuba) bi_mul_karatsuba(dst, tmp_A, tmp_B);
    if (!is_karatsuba) bi_mul_C(dst, tmp_A, tmp_B); 

    (*dst)->sign = A_sign ^ B_sign;
    bi_delete(&tmp_A);
    bi_delete(&tmp_B);

    return print_success_mul();
}

void bi_test_mul()
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