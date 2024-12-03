/**
 * @file bigint_division.c
 * @brief bigint에 대한 나눗셈 연산을 하는 함수들을 정의한 소스 파일
 * @date 2024-11-22
 * @author 김남일
 */
#include <stdio.h>
#include "utils/time.h"
#include "base/base_type.h"
#include "base/base_error.h"
#include "bigint/bigint_addition.h"
#include "bigint/bigint_subtraction.h"
#include "bigint/bigint_calculation.h"
#include "bigint/bigint_multiplication.h"
#include "bigint/bigint_division.h"

/**
 * @brief 비트 단위로 나눗셈을 수행하는 함수
 * @param Q 몫을 저장할 bigint의 포인터
 * @param R 나머지를 저장할 bigint의 포인터
 * @param A 피연산자 A
 * @param B 피연산자 B
 */
res bi_div_bit(OUT bigint** Q, OUT bigint** R, IN bigint* A, IN bigint* B)
{   
    res result;
    START_TIMER();

    if (B == 0)
    {   
        END_TIMER(result, print_zero_div_error());
        return result;
    }

    bigint* tmp_Q = NULL;
    bigint* tmp_R = NULL;

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
        if (bi_compare(tmp_R, B) != -1)
        {
            tmp_Q->start[i / (sizeof(word) * 8)] |= (1 << (i % (sizeof(word) * 8))); // Set the i-th bit of Q
            bi_sub_C(&tmp_R, tmp_R, B); // R = R - B
        }
    }

    if (A->sign == NEGATIVE)
    {
        bigint* one = NULL;
        bi_new(&one, 1);
        one->start[0] = 1;

        // Add 1 to tmp_Q
        bi_add(&tmp_Q, tmp_Q, one);
        bi_sub_C(&tmp_R, B, tmp_R);
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

    END_TIMER(result, print_success_div());
    return result;
}

/**
 * @brief word 단위로 나눗셈을 수행하는 함수
 * @param Q 몫을 저장할 bigint의 포인터
 * @param A 피연산자 A
 * @param B 피연산자 B
 */
res bi_div_word(OUT bigint** Q, IN bigint* A, IN bigint* B)
{   
    res result;
    START_TIMER();

    if (B == 0)
    {   
        END_TIMER(result, print_zero_div_error());
        return result;
    }

    bigint* tmp_Q = NULL;
    bigint* tmp_R = NULL;
    bi_new(&tmp_Q, 1);
    bi_new(&tmp_R, 1);

    if (A->wordlen > 1)
    {
        tmp_R->start[0] = A->start[1];
    }
    else
    {
        tmp_R->start[0] = 0;
    }

    for (int i = (sizeof(word) * 8 - 1); i > -1; i--)
    {
        if (tmp_R->start[0] >= (1 << (sizeof(word) * 8 - 1)))
        {
            bigint* one = NULL;
            bi_new(&one, 1);
            one->start[0] = 1 << i;
            bi_add_C(&tmp_Q, tmp_Q, one);

            bi_bit_left_shift(&tmp_R, 1);
            // Add the corresponding bit from A to R
            word bit = (A->start[0] >> (i)) & 1;
            tmp_R->start[0] |= bit;
            bi_sub_C(&tmp_R, tmp_R, B);
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
                bi_add_C(&tmp_Q, tmp_Q, one);
                bi_sub_C(&tmp_R, tmp_R, B);
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

    END_TIMER(result, print_success_div());
    return result;
}

/**
 * @brief 두 bigint의 나눗셈을 수행하는 함수(Core of Core)
 * @param Q 몫을 저장할 bigint의 포인터
 * @param R 나머지를 저장할 bigint의 포인터
 * @param A 피연산자 A
 * @param B 피연산자 B
 */
res bi_div_CC(OUT bigint** Q, OUT bigint** R, IN bigint* A, IN bigint* B)
{   
    res result;
    START_TIMER();

    int word_size = sizeof(word) * 8;
    // word 크기에 따른 마스크 설정
#if (word_size==32)
    // 32비트 word의 경우
    word mask = (1U << (word_size)) - 1;
#else
    // 64비트 word의 경우
    word mask = (1ULL << (word_size)) - 1;
#endif
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
        bigint* upper_A = NULL;
        bigint* upper_B = NULL;
        bi_new(&upper_A, 2);
        bi_new(&upper_B, 1);

        upper_A->start[0] = tmp_A->start[tmp_A->wordlen - 2];
        upper_A->start[1] = tmp_A->start[tmp_A->wordlen - 1];
        upper_B->start[0] = tmp_B->start[tmp_B->wordlen - 1];

        bi_div_word(&tmp_Q, upper_A, upper_B);
        bi_delete(&upper_A);
        bi_delete(&upper_B);
    }
    bi_mul_C(&tmp_R, tmp_B, tmp_Q);
    bi_sub(&tmp_R, tmp_A, tmp_R);

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

    END_TIMER(result, print_success_div());
    return result;
}

/**
 * @brief 두 bigint의 나눗셈을 수행하는 함수(Core)
 * @param Q 몫을 저장할 bigint의 포인터
 * @param R 나머지를 저장할 bigint의 포인터
 * @param A 피연산자 A
 * @param B 피연산자 B
 */
res bi_div_C(OUT bigint** Q, OUT bigint** R, IN bigint* A, IN bigint* B)
{   
    res result;
    START_TIMER();

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

        END_TIMER(result, print_success_div());
        return result;
    }

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

    END_TIMER(result, print_success_div());
    return result;
}

/**
 * @brief 두 bigint의 나눗셈을 수행하는 함수
 * @param Q 몫을 저장할 bigint의 포인터
 * @param R 나머지를 저장할 bigint의 포인터
 * @param A 피연산자 A
 * @param B 피연산자 B
 */
res bi_div(OUT bigint** Q, OUT bigint** R, IN bigint* A, IN bigint* B)
{   
    res result;
    START_TIMER();

    bigint* tmp_Q = NULL;
    bigint* Qi = NULL;
    bigint* tmp_R = NULL;

    if (bi_compare(A, B) == -1)
    {
        bi_new(&tmp_Q, 1);
        tmp_Q->start[0] = 0;
        bi_assign(Q, tmp_Q);
        bi_assign(R, A);

        END_TIMER(result, print_success_div());
        return result;
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
        bi_sub_C(&tmp_R, B, tmp_R);
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

    END_TIMER(result, print_success_div());
    return result;
}