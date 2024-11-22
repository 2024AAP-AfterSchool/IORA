/**
 * @file bigint_division.c
 * @brief bigint에 대한 나눗셈 연산을 하는 함수들을 정의한 소스 파일
 * @date 2024-11-22
 * @author 김남일
 */
#include <stdio.h>
#include "base/base_type.h"
#include "base/base_error.h"
#include "bigint/bigint_addition.h"
#include "bigint/bigint_subtraction.h"
#include "bigint/bigint_calculation.h"
#include "bigint/bigint_division.h"

msg bi_div_bit(OUT bigint** Q, OUT bigint** R, IN bigint* A, IN bigint* B)
{
    if (B == 0)
    {
        return print_fail_div_bit();
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