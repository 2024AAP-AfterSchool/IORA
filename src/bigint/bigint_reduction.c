/**
 * @file bigint_reduction.c
 * @brief bigint에 대한 나머지 연산을 하는 함수들을 정의한 소스 파일
 * @date 2024-12-01
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

/**
 * @brief bigint A를 bigint n으로 나눈 나머지를 구하는 함수(Barrett Reduction)
 * @param dst bigint A를 bigint n으로 나눈 나머지를 저장할 bigint 구조체의 이중 포인터
 * @param A 나눌 대상이 되는 bigint 구조체
 * @param n 나누는 수가 되는 bigint 구조체
 * @param T Barrett Reduction을 위한 T 값이 되는 bigint 구조체(사전에 계산된 값)
 */
res bi_mod(OUT bigint** dst, IN bigint* A, IN bigint* n, IN bigint* T)
{   
    res result;
    START_TIMER();

    bigint* tmp_A = NULL;
    bigint* tmp_Q = NULL;
    
    bi_assign(&tmp_A, A);
    bi_assign(&tmp_Q, tmp_A);

    if (A->wordlen != n->wordlen * 2)
    {
        bi_word_right_shift(&tmp_Q, (tmp_A->wordlen) / 2);
        bi_mul_C(&tmp_Q, tmp_Q, T);
        bi_word_right_shift(&tmp_Q, (tmp_A->wordlen) / 2 + 1);
    }
    else
    {
        bi_word_right_shift(&tmp_Q, (tmp_A->wordlen) / 2 - 1);
        bi_mul_C(&tmp_Q, tmp_Q, T);
        bi_word_right_shift(&tmp_Q, (tmp_A->wordlen) / 2 + 1);
    }
    
    bi_mul(&tmp_Q, tmp_Q, n, 0);
    bi_sub(&tmp_Q, tmp_A, tmp_Q);

    while (bi_compare(tmp_Q, n)!=-1)
    {   
        bi_sub(&tmp_Q, tmp_Q, n);
    }
    bi_assign(dst, tmp_Q);
    bi_delete(&tmp_A);

    END_TIMER(result, print_success_mod());
    return result;
}