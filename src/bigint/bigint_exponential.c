/**
 * @file bigint_exponential.h
 * @brief bigint에 대한 지수 연산을 하는 함수들을 정의한 헤더 파일
 * @date 2024-11-29
 * @author 송원우, 김남일
 */
#include <stdio.h>
#include "utils/time.h"
#include "base/base_type.h"
#include "base/base_error.h"
#include "bigint/bigint_addition.h"
#include "bigint/bigint_subtraction.h"
#include "bigint/bigint_calculation.h"
#include "bigint/bigint_division.h"
#include "bigint/bigint_multiplication.h"

/**
 * @brief Left-to-right 방식의 거듭제곱 연산을 수행하는 함수
 * @param dst 결과를 저장할 bigint의 포인터
 * @param A 밑이 되는 bigint
 * @param exp 지수가 되는 bigint
 */
res bi_exp_left_to_right(OUT bigint** dst, IN bigint* A, IN bigint* exp)
{   
    res result;
    START_TIMER();

    if (A == NULL || exp == NULL || dst == NULL)
    {
        END_TIMER(result, NULL_POINTER_ERROR);
        return result;
    }

    // 결과 변수 초기화 
    bigint* tmp_result = NULL;
    bi_new(&tmp_result, 1);
    tmp_result->start[0] = 1;

    // 복사된 base와 exp 생성
    bigint* tmp_base = NULL;
    bigint* tmp_exp = NULL;
    bi_assign(&tmp_base, A);
    bi_assign(&tmp_exp, exp);

    // exp의 각 비트를 확인하며 연산 수행
    for (int i = (tmp_exp->wordlen * sizeof(word) * 8) - 1; i >= 0; i--)
    {
        // result = result^2
        res square_verification_result = bi_square_karatsuba(&tmp_result, tmp_result);
        if (square_verification_result.message != SUCCESS_SQUARE_A)
        {
            bi_delete(&tmp_result);
            bi_delete(&tmp_base);
            bi_delete(&tmp_exp);
        }

        // 현재 비트가 1인 경우 result = result * base
        if ((tmp_exp->start[i / (sizeof(word) * 8)] >> (i % (sizeof(word) * 8))) & 1)
        {
            res mul_verification_result = bi_mul(&tmp_result, tmp_result, tmp_base, 1);
            if (mul_verification_result.message != SUCCESS_MUL)
            {
                bi_delete(&tmp_result);
                bi_delete(&tmp_base);
                bi_delete(&tmp_exp);
            }
        }
    }

    // 최종 결과 저장
    bi_assign(dst, tmp_result);

    // 메모리 해제
    bi_delete(&tmp_result);
    bi_delete(&tmp_base);
    bi_delete(&tmp_exp);

    END_TIMER(result, print_success_left_to_right());
    return result;
}

/**
 * @brief Right-to-left 방식의 거듭제곱 연산을 수행하는 함수
 * @param dst 결과를 저장할 bigint의 포인터
 * @param A 밑이 되는 bigint
 * @param exp 지수가 되는 bigint
 */
res bi_exp_right_to_left(OUT bigint** dst, IN bigint* A, IN bigint* exp)
{
    res result;
    START_TIMER();

    if (A == NULL || exp == NULL || dst == NULL)
    {
        END_TIMER(result, NULL_POINTER_ERROR);
        return result;
    }

    // 결과 변수 초기화 (결과 = 1로 시작)
    bigint* tmp_result = NULL;
    bi_new(&tmp_result, 1);
    tmp_result->start[0] = 1;

    // 복사된 base와 exp 생성
    bigint* tmp_base = NULL;
    bigint* tmp_exp = NULL;
    bi_assign(&tmp_base, A);
    bi_assign(&tmp_exp, exp);

    // tmp_exp가 0이 아닐 때까지 반복
    while (!bi_is_zero(tmp_exp))
    {
        // exp의 LSB(가장 낮은 비트)가 1이면 result *= base
        if (tmp_exp->start[0] & 1)
        {
            res mul_verification_result = bi_mul(&tmp_result, tmp_result, tmp_base, false);
            if (mul_verification_result.message != SUCCESS_MUL)
            {
                bi_delete(&tmp_result);
                bi_delete(&tmp_base);
                bi_delete(&tmp_exp);

                END_TIMER(result, mul_verification_result.message);
                return result; // 곱셈 중 에러 발생 시 처리
            }
        }
        // base = base^2
        res square_verification_result = bi_square_C(&tmp_base, tmp_base);
        if (square_verification_result.message != SUCCESS_SQUARE_A) {
            bi_delete(&tmp_result);
            bi_delete(&tmp_base);
            bi_delete(&tmp_exp);

            END_TIMER(result, square_verification_result.message);
            return square_verification_result; // 제곱 중 에러 발생 시 처리
        }
        // exp = exp >> 1 (지수를 오른쪽으로 1비트 이동)
        bi_bit_right_shift(&tmp_exp, 1);
    }
    // 최종 결과 저장
    bi_assign(dst, tmp_result);

    // 메모리 해제
    bi_delete(&tmp_result);
    bi_delete(&tmp_base);
    bi_delete(&tmp_exp);

    END_TIMER(result, print_success_right_to_left());
    return result;
}

/**
 * @brief Multiply-and-Square(Montgomery) 방식의 거듭제곱 연산을 수행하는 함수
 * @param dst 결과를 저장할 bigint의 포인터
 * @param A 밑이 되는 bigint
 * @param exp 지수가 되는 bigint
 */
res bi_exp_montgomery(OUT bigint** dst, IN bigint* A, IN bigint* exp)
{   
    res result;
    START_TIMER();

    bigint* t0 = NULL;
    bigint* t1 = NULL;
    bi_new(&t0, 1);
    t0->start[0] = 1;
    bi_assign(&t1, A);

    for (int i = exp->wordlen * sizeof(word) * 8 - 1; i > -1; i--)
    {
        int expi = (exp->start[i / (sizeof(word) * 8)] >> (i % (sizeof(word) * 8)) & 1);
        if (expi == 0)
        {
            bi_mul_C(&t1, t0, t1);
            bi_square_C(&t0, t0);    //squaring 함수명 확정되면 바꾸기 
        }
        else
        {
            bi_mul_C(&t0, t0, t1);
            bi_square_C(&t1, t1);    //squaring 함수명 확정되면 바꾸기 
        }
    }
    bi_assign(dst, t0);
    bi_delete(&t0);
    bi_delete(&t1);

    END_TIMER(result, print_success_montgomery());
    return result;
}

/**
 * @brief Left-to-right 방식의 거듭제곱 연산을 수행하면서 나머지 연산을 수행하는 함수
 * @param dst 결과를 저장할 bigint의 포인터
 * @param A 밑이 되는 bigint
 * @param exp 지수가 되는 bigint
 * @param mod 나머지가 되는 bigint
 */

res bi_exp_left_to_right_mod(OUT bigint** dst, IN bigint* A, IN bigint* exp, IN bigint* mod)
{
    res result;
    START_TIMER();

    
    if (A == NULL || exp == NULL || dst == NULL || mod == NULL)
    {
        END_TIMER(result, NULL_POINTER_ERROR);
        return result;
    }
    
    // 초기값 설정
    bigint* tmp_result = NULL;
    bigint* tmp_base = NULL;
    bi_new(&tmp_result, 1);  // 결과값 초기화
    tmp_result->start[0] = 1;
    bi_assign(&tmp_base, A); // A를 tmp_base에 복사

    // 지수 연산 수행 (왼쪽에서 오른쪽으로 처리)
    for (int i = (exp->wordlen * word_size) - 1; i >= 0; i--)
    {
        // tmp_result = tmp_result^2 % mod
        bi_square(&tmp_result, tmp_result, 1); // 제곱
        bigint* tmp_q = NULL;
        bigint* tmp_r = NULL;
        bi_div(&tmp_q, &tmp_r, tmp_result, mod); // mod 연산
        bi_assign(&tmp_result, tmp_r); // 나머지를 tmp_result에 저장
        bi_delete(&tmp_q);
        bi_delete(&tmp_r);

        // 지수의 현재 비트가 1인 경우, 곱셈 연산 수행
        if ((exp->start[i / word_size] >> (i % word_size)) & 1)
        {
            // tmp_result = tmp_result * tmp_base % mod
            bi_mul(&tmp_result, tmp_result, tmp_base, 1); // 곱셈
            bi_div(&tmp_q, &tmp_r, tmp_result, mod);      // mod 연산
            bi_assign(&tmp_result, tmp_r); // 나머지를 tmp_result에 저장
            bi_delete(&tmp_q);
            bi_delete(&tmp_r);
        }
    }

    // 최종 결과 저장
    bi_assign(dst, tmp_result);

    // 메모리 해제
    bi_delete(&tmp_result);
    bi_delete(&tmp_base);

    END_TIMER(result, print_success_left_to_right_mod());
    return result;
}

/**
 * @brief Right-to-left 방식의 거듭제곱 연산을 수행하면서 나머지 연산을 수행하는 함수
 * @param dst 결과를 저장할 bigint의 포인터
 * @param A 밑이 되는 bigint
 * @param exp 지수가 되는 bigint
 * @param mod 나머지가 되는 bigint
 */
res bi_exp_right_to_left_mod(OUT bigint** dst, IN bigint* A, IN bigint* exp, IN bigint* mod)
{
    res result;
    START_TIMER();
    
    if (A == NULL || exp == NULL || dst == NULL || mod == NULL)
    {
        END_TIMER(result, NULL_POINTER_ERROR);
        return result;
    }
    
    // 결과 변수 초기화 (결과 = 1로 시작)
    bigint* tmp_result = NULL;
    bi_new(&tmp_result, 1);
    tmp_result->start[0] = 1;

    // 복사된 base와 exp 생성
    bigint* tmp_base = NULL;
    bigint* tmp_exp = NULL;
    bi_assign(&tmp_base, A);
    bi_assign(&tmp_exp, exp);

    // tmp_exp가 0이 아닐 때까지 반복
    while (!bi_is_zero(tmp_exp))
    {
        // exp의 LSB(가장 낮은 비트)가 1이면 result *= base
        if (tmp_exp->start[0] & 1)
        {
            bi_mul(&tmp_result, tmp_result, tmp_base, 1);
            // 곱셈 후 mod 연산 적용
            bigint* tmp_q = NULL;
            bigint* tmp_r = NULL;
            bi_div(&tmp_q, &tmp_r, tmp_result, mod);

            // 결과는 나머지로 업데이트
            bi_assign(&tmp_result, tmp_r);
            // 메모리 해제
            bi_delete(&tmp_q);
            bi_delete(&tmp_r);
        }

        // base = base^2
        bi_square_karatsuba(&tmp_base, tmp_base);
        // 제곱 후 mod 연산 적용
        bigint* tmp_q = NULL;
        bigint* tmp_r = NULL;
        bi_div(&tmp_q, &tmp_r, tmp_base, mod);

        // base 업데이트
        bi_assign(&tmp_base, tmp_r);
        // 메모리 해제
        bi_delete(&tmp_q);
        bi_delete(&tmp_r);
        // exp = exp >> 1 (지수를 오른쪽으로 1비트 이동)
        bi_bit_right_shift(&tmp_exp, 1);
    }
    // 최종 결과 저장
    bi_assign(dst, tmp_result);
    // 메모리 해제
    bi_delete(&tmp_result);
    bi_delete(&tmp_base);
    bi_delete(&tmp_exp);

    END_TIMER(result, print_success_right_to_left_mod());
    return result;
}

/**
 * @brief Multiply-and-Square(Montgomery) 방식의 거듭제곱 연산을 수행하면서 나머지 연산을 수행하는 함수
 * @param dst 결과를 저장할 bigint의 포인터
 * @param A 밑이 되는 bigint
 * @param exp 지수가 되는 bigint
 * @param mod 나머지가 되는 bigint
 */
res bi_exp_montgomery_mod(OUT bigint** dst, IN bigint* A, IN bigint* exp, IN bigint* mod)
{
    res result;
    START_TIMER();

    bigint* t0 = NULL;
    bigint* t1 = NULL;
    bi_new(&t0, 1);
    t0->start[0] = 1;
    bi_assign(&t1, A);

    for (int i = exp->wordlen * word_size - 1; i > -1; i--)
    {
        int ni = (exp->start[i / word_size] >> (i % (word_size)) & 1);
        if (ni == 0)
        {
            bi_mul(&t1, t0, t1,0);
            // 곱셈 후 mod 연산 적용
            bigint* tmp_q = NULL;
            bigint* tmp_r = NULL;
            bi_div(&tmp_q, &tmp_r, t1, mod);

            bi_assign(&t1, tmp_r);
            bi_square(&t0, t0,1);
            // 제곱 후 mod 연산 적용
            bi_div(&tmp_q, &tmp_r, t0, mod);

            bi_assign(&t0, tmp_r);
            // 메모리 해제
            bi_delete(&tmp_q);
            bi_delete(&tmp_r);
        }
        else
        {
            bi_mul(&t0, t0, t1,1);
            // 곱셈 후 mod 연산 적용
            bigint* tmp_q = NULL;
            bigint* tmp_r = NULL;
            bi_div(&tmp_q, &tmp_r, t0, mod);

            bi_assign(&t0, tmp_r);
            bi_square(&t1, t1,1);
            // 제곱 후 mod 연산 적용
            bi_div(&tmp_q, &tmp_r, t1, mod);

            bi_assign(&t1, tmp_r);
            // 메모리 해제
            bi_delete(&tmp_q);
            bi_delete(&tmp_r);
        }
    }
    bi_assign(dst, t0);
    bi_delete(&t0);
    bi_delete(&t1);

    END_TIMER(result, print_success_montgomery_mod());
    return result;
}