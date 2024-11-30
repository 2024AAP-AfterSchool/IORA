/**
 * @file bigint_division.c
 * @brief bigint에 대한 나눗셈 연산을 하는 함수들을 정의한 소스 파일
 * @date 2024-11-22
 * @author 김남일
 */
#include <stdio.h>
#include "base_type.h"
#include "base_error.h"
#include "bigint_addition.h"
#include "bigint_subtraction.h"
#include "bigint_calculation.h"
#include "bigint_multiplication.h"
#include "bigint_division.h"

msg bi_div_bit(OUT bigint** Q, OUT bigint** R, IN bigint* A, IN bigint* B)
{
    if (B == 0)
    {
        return print_zero_div_error();
    }

    bigint* tmp_Q = NULL;
    bigint* tmp_R = NULL;
    bigint* tmp_A = NULL;
    bigint* tmp_B = NULL;
    // Initialize Q and R
    bi_new(&tmp_Q, A->wordlen);
    bi_new(&tmp_R, A->wordlen);  // Increase the size of tmp_R to handle bit shifts properly
    // Loop from the most significant bit to the least significant bit
    for (int i = A->wordlen * sizeof(word) * 8 - 1; i >= 0; i--) {
        // Shift R to the left by 1 bit
        bi_bit_left_shift(&tmp_R, 1);

        // Add the corresponding bit from A to R
        word bit = (A->start[i / (sizeof(word) * 8)] >> (i % (sizeof(word) * 8))) & 1;
        tmp_R->start[0] |= bit;

        // If R >= B, set the corresponding bit in Q and subtract B from R
        if (bi_compare(tmp_R, B) != -1) {
            tmp_Q->start[i / (sizeof(word) * 8)] |= (1 << (i % (sizeof(word) * 8))); // Set the i-th bit of Q
            bi_sub(&tmp_R, tmp_R, B); // R = R - B
        }
    }

    if (A->sign == NEGATIVE)
    {
        bigint* one = NULL;
        bi_new(&one, 1);
        one->start[0] = 1;

        // Add 1 to tmp_Q
        bi_add(&tmp_Q, tmp_Q, one);
        bi_sub(&tmp_R, B, tmp_R);
        tmp_Q->sign = A->sign;
        bi_delete(&one);
    }
    // Assign results to Q and R
    bi_refine(tmp_Q);
    bi_assign(Q, tmp_Q);
    bi_assign(R, tmp_R);

    // Clean up
    bi_delete(&tmp_Q);
    bi_delete(&tmp_R);

    return print_success_div();
}

msg bi_div_word(OUT bigint** Q, IN bigint* A, IN bigint* B)
{
    if (B == NULL || B->start[0] == 0)
    {
        return print_zero_div_error();
    }

    bigint* tmp_Q = NULL;
    bigint* tmp_R = NULL;
    bi_new(&tmp_Q, 1);
    bi_new(&tmp_R, 1);
    if (A->wordlen > 1) {
        tmp_R->start[0] = A->start[1];
    }
    else {
        tmp_R->start[0] = 0;
    }

    for (int i = (sizeof(word) * 8 - 1); i > -1; i--)
    {
        if (tmp_R->start[0] >= (1 << (sizeof(word) * 8 - 1)))
        {
            bigint* one = NULL;
            bi_new(&one, 1);
            one->start[0] = 1 << i;
            bi_add(&tmp_Q, tmp_Q, one);

            bi_bit_left_shift(&tmp_R, 1);
            // Add the corresponding bit from A to R
            word bit = (A->start[0] >> (i)) & 1;
            tmp_R->start[0] |= bit;
            bi_sub(&tmp_R, tmp_R, B);
            bi_delete(&one);
        }
        else
        {
            // Add the corresponding bit from A to R
            word bit = (A->start[0] >> (i)) & 1;
            bi_bit_left_shift(&tmp_R, 1);
            tmp_R->start[0] |= bit;
            if (bi_compare(tmp_R, B) != -1)
            {
                bigint* one = NULL;
                bi_new(&one, 1);
                one->start[0] = 1 << i;
                bi_add(&tmp_Q, tmp_Q, one);
                bi_sub(&tmp_R, tmp_R, B);
                bi_delete(&one);
            }
        }
    }
    // Assign results to Q and R
    bi_refine(tmp_Q);
    bi_assign(Q, tmp_Q);

    // Clean up
    bi_delete(&tmp_Q);
    bi_delete(&tmp_R);

    return print_success_div();
}

msg bi_div_CC(OUT bigint** Q, OUT bigint** R, IN bigint* A, IN bigint* B)
{
    int word_size = sizeof(word) * 8;
    // word 크기에 따른 마스크 설정

    // 32비트 word의 경우
    word mask = (1U << (word_size)) - 1;
    bigint* tmp_A = NULL;
    bigint* tmp_B = NULL;
    bi_refine(A);
    bi_refine(B);
    bi_assign(&tmp_A,A);
    bi_assign(&tmp_B,B);
    bigint* tmp_Q = NULL;
    bigint* tmp_R = NULL;
    if ((tmp_A->wordlen) == (tmp_B->wordlen))
    {
        bi_new(&tmp_Q, 1);
        tmp_Q->start[0] = (tmp_A->start[(tmp_A->wordlen) - 1] / tmp_B->start[(tmp_A->wordlen) - 1]);
    }
    if ((((tmp_A->wordlen) - 1 == (tmp_B->wordlen)) && ((tmp_A->start[(tmp_A->wordlen) - 1])) == (tmp_A->start[(tmp_B->wordlen) - 1])))
    {
        bi_new(&tmp_Q, 1);
        tmp_Q->start[0] = mask;
    }
    if ((((tmp_A->wordlen) - 1 == (tmp_B->wordlen)) && ((tmp_A->start[(tmp_A->wordlen) - 1])) != (tmp_B->start[(tmp_B->wordlen) - 1])))
    {
        //bi_print(A,16);
        //bi_print(B,16);
        bigint* upper_A = NULL;
        bigint* upper_B = NULL;
        bi_new(&upper_A, 2);
        bi_new(&upper_B, 1);
        upper_A->start[0] = tmp_A->start[tmp_A->wordlen - 2];
        //printf("%x\n", A->start[A->wordlen - 2]);
        //printf("%x\n", upper_A->start[0]);
        upper_A->start[1] = tmp_A->start[tmp_A->wordlen - 1];
        upper_B->start[0] = tmp_B->start[tmp_B->wordlen - 1];
        //printf("asdasdasd AAAA\n");
        //bi_print(upper_A,16);
        //printf("asdasdasd BBBB\n");
        //bi_print(upper_B,16);
        bi_div_word(&tmp_Q, upper_A, upper_B);
        // fprintf(stdout, "\n##@4#tem_Q:");
        // bi_print(tmp_Q, 16);
        // fprintf(stdout, "\n##@455#B:");
        // bi_print(B, 16);
        // fprintf(stdout, "\n##@455#temR:");
        // bi_print(tmp_R, 16);
        // fprintf(stdout, "\n##@328723878723784#\n");
        bi_delete(&upper_A);
        bi_delete(&upper_B);
    }

    bi_karatsuba_mul(&tmp_R, tmp_B, tmp_Q);
    // fprintf(stdout, "\n아래가 R임저기!!!!!!!!!##@328723878723784#\n");
    // bi_print(tmp_R, 16);
    // fprintf(stdout, "\n아래가 A임##@328723878723784#\n");
    //bi_print(A, 16);
    //bi_print(tmp_R,16);
    bi_sub(&tmp_R, tmp_A, tmp_R);
    // fprintf(stdout, "\n아래가 계산 R여기!!!!!!!!!##@328723878723784#\n");
    // bi_print(tmp_R, 16);
    // fprintf(stdout, "\n 끝히히##@@@@@@@@@@@@@@328723878723784#\n");

    bigint* one = NULL;
    bi_new(&one, 1);
    one->start[0] = 1;

    while (tmp_R->sign == NEGATIVE)
    {
        bi_sub(&tmp_Q, tmp_Q, one);
        bi_add(&tmp_R, tmp_R, tmp_B);
    }
    bi_assign(Q, tmp_Q);
    bi_assign(R, tmp_R);

    bi_delete(&tmp_A);
    bi_delete(&tmp_B);

    bi_delete(&tmp_Q);
    bi_delete(&tmp_R);
    bi_delete(&one);

    return print_success_div();
}

msg bi_div_C(OUT bigint** Q, OUT bigint** R, IN bigint* A, IN bigint* B)
{
    bigint* tmp_Q = NULL;
    bigint* tmp_R = NULL;
    bigint* q = NULL;
    bi_refine(A);
    bi_refine(B);

    if (bi_compare(A, B) == -1)
    {
        bi_new(&tmp_Q, 1);
        tmp_Q->start[0] = 0;
        bi_assign(&q, tmp_Q);
        bi_assign(&tmp_R, A);
        bi_assign(Q, q);
        bi_assign(R, tmp_R);

        bi_delete(&tmp_Q);

        return print_success_div();
    }


    //bigint* tmp_R = NULL;
    bigint* tmp_A = NULL;
    bigint* tmp_B = NULL;
    bi_assign(&tmp_A, A);
    bi_assign(&tmp_B, B);

    int k = 0;
    word highest_word = B->start[B->wordlen - 1];
    while ((highest_word & (1ULL << (sizeof(word) * 8 - 1))) == 0)
    {
        highest_word <<= 1;
        k++;
    }
    bi_bit_left_shift(&tmp_A, k);
    bi_bit_left_shift(&tmp_B, k);
    bi_div_CC(&tmp_Q, &tmp_R, tmp_A, tmp_B);
    bi_bit_right_shift(&tmp_R, k);
    bi_assign(Q, tmp_Q);
    bi_assign(R, tmp_R);

    bi_delete(&tmp_Q);
    bi_delete(&tmp_R);
    bi_delete(&tmp_A);
    bi_delete(&tmp_B);

    return print_success_div();
}

msg bi_div(OUT bigint** Q, OUT bigint** R, IN bigint* A, IN bigint* B)
{
    bigint* tmp_Q = NULL;
    bigint* Qi = NULL;
    bigint* tmp_R = NULL;

    if (bi_compare(A, B) == -1)
    {
        bi_new(&tmp_Q, 1);
        tmp_Q->start[0] = 0;
        bi_assign(Q, tmp_Q);
        bi_assign(R, A);

        return print_success_div();
    }

    bi_new(&tmp_R, 1);
    tmp_R->start[0] = 0;
    bi_new(&tmp_Q, A->wordlen);
    for (int i = (A->wordlen) - 1; i > -1; i--)
    {
        bi_word_left_shift(&tmp_R, 1);
        tmp_R->start[0] |= A->start[i];
        bi_div_C(&Qi, &tmp_R, tmp_R, B);
        tmp_Q->start[i] = Qi->start[0];
    }

    if (A->sign == NEGATIVE)
    {
        bigint* one = NULL;
        bi_new(&one, 1);
        one->start[0] = 1;
        // Add 1 to tmp_Q
        bi_add(&tmp_Q, tmp_Q, one);
        bi_sub(&tmp_R, B, tmp_R);
        //부호 복사
        tmp_Q->sign = A->sign;
        bi_delete(&one);
    }

    bi_refine(tmp_Q);
    bi_refine(tmp_R);
    bi_assign(Q, tmp_Q);
    bi_assign(R, tmp_R);

    bi_delete(&tmp_Q);
    bi_delete(&tmp_R);
    bi_delete(&Qi);

    return print_success_div();
}