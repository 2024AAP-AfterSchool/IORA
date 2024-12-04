/**
 * @file bigint_multiplication.c
 * @brief bigint에 대한 곱셈 연산을 하는 함수들을 정의한 소스 파일
 * @date 2024-11-08
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
 * @brief 두 양의 정수 A와 B의 곱셈을 수행하는 함수
 * @param dst 결과를 저장할 bigint의 포인터
 * @param A 곱셈을 수행할 첫 번째 word
 * @param B 곱셈을 수행할 두 번째 word
 */
res bi_mul_AB(OUT bigint** dst, IN word* A, IN word* B)
{   
    res result;
    START_TIMER();

    bigint* C = NULL;
    bi_new(&C, 2);

    word mask = (word_size == 8) ? 0xF :
                (word_size == 32) ? 0xFFFF :
                 0xFFFFFFFF;

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

    END_TIMER(result, print_success_mul_AB());
    return result;
}

/**
 * @brief 두 bigint A와 B의 곱셈을 수행하는 함수
 * @param dst 결과를 저장할 bigint의 포인터
 * @param A 곱셈을 수행할 첫 번째 bigint
 * @param B 곱셈을 수행할 두 번째 bigint
 */
res bi_mul_C(OUT bigint** dst, IN bigint* A, IN bigint* B)
{   
    res result;
    START_TIMER();

    bigint* C = NULL;
    bigint* T = NULL;
    bi_new(&C, A->wordlen + B->wordlen);
    bi_new(&C, A->wordlen + 1);
    bigint* tmp_A = NULL;

    // A가 홀수 길이인 경우 확장
    if (A->wordlen % 2 == 1)
    {
        bi_new(&tmp_A, A->wordlen + 1);
        for (int i = 0; i < A->wordlen; i++)
        {
            tmp_A->start[i] = A->start[i];
        }
        tmp_A->start[A->wordlen] = 0;
    }
    else
    {
        bi_assign(&tmp_A, A);
    }

    // 곱셈 및 결과 누적
    for (int i = 0; i < B->wordlen; i++)
    {
        bigint* T0 = NULL;
        bigint* T1 = NULL;
        bigint* tmp_T0 = NULL;
        bigint* tmp_T1 = NULL;
        bi_new(&T0, 2); // T0는 2 워드 크기로 생성
        bi_new(&T1, 2); // T1도 2 워드 크기로 생성
        bi_new(&tmp_T0, (tmp_A->wordlen));
        bi_new(&tmp_T1, (tmp_A->wordlen) + 1);
        for (int j = 0; j < (tmp_A->wordlen / 2); j++)
        {
            res verification_result;
            verification_result = bi_mul_AB(&T0, &(tmp_A->start[2 * j]), &(B->start[i]));
            if (verification_result.message != SUCCESS_MUL_AB)
            {
                END_TIMER(result, print_fail_mul_AB());
                return result;
            }
            tmp_T0->start[2 * j] = T0->start[0];
            tmp_T0->start[(2 * j) + 1] = T0->start[1];
            verification_result = bi_mul_AB(&T1, &(tmp_A->start[(2 * j) + 1]), &(B->start[i]));
            if (verification_result.message != SUCCESS_MUL_AB)
            {
                END_TIMER(result, print_fail_mul_AB());
                return result;
            }
            tmp_T1->start[(2 * j) + 1] = T1->start[0];
            tmp_T1->start[(2 * (j + 1))] = T1->start[1];
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

    END_TIMER(result, print_success_mul_C());
    return result;
}

/**
 * @brief 두 bigint A와 B의 곱셈을 수행하는 함수(카라추바 알고리즘 사용)
 * @param dst 결과를 저장할 bigint의 포인터
 * @param A 곱셈을 수행할 첫 번째 bigint
 * @param B 곱셈을 수행할 두 번째 bigint
 */
res bi_mul_karatsuba(OUT bigint** dst, IN bigint* A, IN bigint* B)
{
    res result;
    START_TIMER();
    bigint* tmp_A = NULL;
    bigint* tmp_B = NULL;
    bi_assign(&tmp_A, A);
    bi_assign(&tmp_B, B);
    uint32_t n = (tmp_A->wordlen > B->wordlen) ? tmp_A->wordlen : tmp_B->wordlen;
    if (n <= 1)
    {
        return bi_mul(dst, tmp_A, tmp_B, 0);
    }
    uint32_t half = (n + 1) / 2;
    bigint* A1 = NULL, * A0 = NULL, * B1 = NULL, * B0 = NULL;
    bi_new(&A1, n - half);
    bi_new(&A0, half);
    for (uint32_t i = 0; i < half; i++)
    {
        A0->start[i] = (i < tmp_A->wordlen) ? tmp_A->start[i] : 0;
    }
    for (uint32_t i = half; i < n; i++)
    {
        A1->start[i - half] = (i < tmp_A->wordlen) ? tmp_A->start[i] : 0;
    }
    bi_new(&B1, n - half);
    bi_new(&B0, half);
    for (uint32_t i = 0; i < half; i++)
    {
        B0->start[i] = (i < tmp_B->wordlen) ? tmp_B->start[i] : 0;
    }
    for (uint32_t i = half; i < n; i++)
    {
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
    bi_word_left_shift(&T2,  half);
    bigint* tmp_result = NULL;
    bi_add(&tmp_result, T1, T2);
    bi_add(&tmp_result, tmp_result, T0);
    tmp_result->sign = A->sign ^ B->sign;
    // Assign result to dst
    bi_assign(dst, tmp_result);
    bi_delete(&A1); bi_delete(&A0);
    bi_delete(&B1); bi_delete(&B0);
    bi_delete(&T1); bi_delete(&T0); bi_delete(&T2);
    bi_delete(&S1); bi_delete(&S2);
    bi_delete(&tmp_result);
    END_TIMER(result, SUCCESS_MUL_C);
    return result;
}

/**
 * @brief 두 bigint A와 B의 곱셈을 수행하는 함수
 * @param dst 결과를 저장할 bigint의 포인터
 * @param A 곱셈을 수행할 첫 번째 bigint
 * @param B 곱셈을 수행할 두 번째 bigint
 * @param is_karatsuba 카라추바 곱셈을 사용할지 여부
 */
res bi_mul(OUT bigint** dst, IN bigint* A, IN bigint* B, IN bool is_karatsuba)
{
    res result;
    START_TIMER();
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

        END_TIMER(result, print_success_mul());
        return result;
    }
    if (bi_is_one(tmp_A))
    {
        bi_assign(dst, tmp_B);
        (*dst)->sign = (tmp_A->sign ^ tmp_B->sign);
        bi_delete(&tmp_A);
        bi_delete(&tmp_B);

        END_TIMER(result, print_success_mul());
        return result;
    }
    if (bi_is_one(tmp_B))
    {
        bi_assign(dst, tmp_A);
        (*dst)->sign = (tmp_A->sign ^ tmp_B->sign);
        bi_delete(&tmp_A);
        bi_delete(&tmp_B);

        END_TIMER(result, print_success_mul());
        return result;
    }

    //카라추바에 양수 곱셈 넣고 부호는 따로 저장하기 위함
    byte A_sign = tmp_A->sign;
    byte B_sign = tmp_B->sign;
    tmp_A->sign = POSITIVE;

    if (is_karatsuba) bi_mul_karatsuba(dst, tmp_A, tmp_B);
    if (!is_karatsuba) bi_mul_C(dst, tmp_A, tmp_B);

    (*dst)->sign = A_sign ^ B_sign;
    bi_delete(&tmp_A);
    bi_delete(&tmp_B);

    END_TIMER(result, print_success_mul());
    return result;
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

/**
 * @brief bigint의 제곱을 수행하는 함수
 * @param dst 결과를 저장할 bigint의 포인터
 * @param A 제곱을 수행할 bigint
 */
res bi_square_AA(OUT bigint** dst, IN bigint* A)
{   
    res result;
    START_TIMER();

    bigint* C = NULL;
    bi_new(&C, 2); // 충분한 크기의 bigint 생성
    int half_word_size = sizeof(word) * 4;

    word mask = (word_size == 8) ? 0xF :
                (word_size == 32) ? 0xFFFF :
                 0xFFFFFFFF;

    // A의 상위와 하위 비트 추출
    word A1 = *A->start >> half_word_size; // A의 상위 비트
    word A0 = *A->start & mask;            // A의 하위 비트

    // C1 = A1^2, C0 = A0^2
    bigint* C1 = NULL;
    bi_new(&C1, 1);
    C1->start[0] = (A1 * A1);
    bigint* C0 = NULL;
    bi_new(&C0, 1);
    C0->start[0] = (A0 * A0);

    // C = (C1 << w) + C0
    bi_word_left_shift(&C1, 1); // C1 << w
    
    bi_add(&C, C0,  C1); // C = C1 + C0

    // T = A0 * A1
    bigint* T = NULL;
    bi_new(&T, 1);
    word cross = A0 * A1;
    T->start[0] = cross;

    // T = T << (w/2 + 1)
    bi_bit_left_shift(&T, half_word_size + 1);
   
    // C = C + T
    bi_add(&C, C, T);
    bi_refine(C);

    // 결과를 dst에 복사
    bi_assign(dst, C);

    // 메모리 해제
    bi_delete(&C);
    bi_delete(&C1);
    bi_delete(&C0);
    bi_delete(&T);

    END_TIMER(result, SUCCESS_SQUARE_A);
    return result;
}

/**
 * @brief bigint의 제곱을 수행하는 함수
 * @param dst 결과를 저장할 bigint의 포인터
 * @param X 제곱을 수행할 bigint
 */
res bi_square_C(OUT bigint** dst, IN bigint* X)
{   
    res result;
    START_TIMER();

    bigint* C1 = NULL;
    bigint* C2 = NULL;
    bigint* T1 = NULL;
    bigint* T2 = NULL;

    // C1, C2 초기화
    bi_new(&C1, X->wordlen * 2);
    bi_new(&C2, X->wordlen * 2);

    // C1 계산
    for (int j = 0; j < X->wordlen; j++)
    {
        // T1 = x[j]^2
        bigint* temp = NULL;
        bi_new(&temp, 1);
        temp->start[0] = X->start[j];
        bi_square_AA(&T1, temp);  
        bi_delete(&temp);
        // T1 <<= 2 * j (워드 단위 이동)
        bi_word_left_shift(&T1, 2 * j);
        // C1 += T1
        bi_add(&C1, C1, T1);
        bi_delete(&T1);
    }

    // C2 계산
    for (int j = 0; j < X->wordlen; j++)
    {
        for (int i = j + 1; i < X->wordlen; i++)
        {
            // T2 = x[j] * x[i]
            bigint* temp1 = NULL;
            bigint* temp2 = NULL;
            bi_new(&temp1, 1);
            bi_new(&temp2, 1);
            temp1->start[0] = X->start[j];
            temp2->start[0] = X->start[i];
            bi_mul_AB(&T2, &(temp1->start[0]), &(temp2->start[0]));
            bi_delete(&temp1);
            bi_delete(&temp2);
            // T2 <<= (i + j)
            bi_word_left_shift(&T2, i + j);
            // C2 += T2
            bi_add(&C2, C2, T2);
            bi_delete(&T2);
        }
    }

    // C2 <<= 1
    bi_bit_left_shift(&C2, 1);

    // C = C1 + C2
    bi_add(dst, C1, C2);

    // 메모리 해제
    bi_delete(&C1);
    bi_delete(&C2);

    END_TIMER(result, SUCCESS_SQUARE_A);
    return result;
}

/**
 * @brief bigint A의 제곱을 수행하는 함수(karatuba 사용)
 * @param dst 결과를 저장할 bigint의 포인터
 * @param A 제곱을 수행할 bigint
 */
res bi_square_karatsuba(OUT bigint** dst, IN bigint* A)
{   
    res result;
    START_TIMER();

    bigint* tmp_A = NULL;
    bi_assign(&tmp_A, A);

    if (tmp_A == NULL || tmp_A->wordlen == 0)
    {
        bi_new(dst, 1); // A가 NULL이거나 비어있을 경우 0 반환
        (*dst)->start[0] = 0;

        END_TIMER(result, SUCCESS_SQUARE_A);
        return result;
    }
    if (A->wordlen == 1)
    {
        // 단일 워드 제곱 처리
        res tmp_result = bi_square_C(dst, tmp_A);
        END_TIMER(result, tmp_result.message);
        return tmp_result;
    }

    // 워드 분할 길이
    uint32_t l = (tmp_A->wordlen + 1) / 2; // 절반 길이
    bigint* A1 = NULL;
    bigint* A0 = NULL;
    bi_new(&A1, tmp_A->wordlen - l);
    bi_new(&A0, l);
    
    // 상위 워드와 하위 워드 분리
    for (uint32_t i = 0; i < l; i++) \
    {
        A0->start[i] = tmp_A->start[i]; // 하위 워드 복사
    }
    for (uint32_t i = l; i < tmp_A->wordlen; i++)
    {
        A1->start[i - l] = tmp_A->start[i]; // 상위 워드 복사
    }

    // 중간 계산 값 초기화
    bigint* T1 = NULL;
    bigint* T0 = NULL;
    bigint* R = NULL;
    bigint* S = NULL;

    // 재귀적으로 A1^2, A0^2 계산
    bi_square_karatsuba(&T1, A1); // T1 = A1^2
    bi_refine(T1);
    bi_square_karatsuba(&T0, A0); // T0 = A0^2

    // R = (T1 << 2l) + T0
    bi_refine(T0);
    bi_word_left_shift(&T1, 2 * l);
    bi_add(&R, T1, T0); // R = T1 + T0

    // S = A1 * A0
    bi_mul_karatsuba(&S, A1, A0); // S = A1 * A0
    // S <<= (l + 1)
    
    //bi_word_left_shift(&S,l);
    bi_word_left_shift(&S, l);
    bi_bit_left_shift(&S, 1);
    
    // C = R + S
    bigint* C = NULL;
    bi_add(&C, R, S);

    // 결과를 dst에 복사
    bi_assign(dst, C);
    
    // 메모리 해제
    bi_delete(&tmp_A);
    bi_delete(&A1);
    bi_delete(&A0);
    bi_delete(&T1);
    bi_delete(&T0);
    bi_delete(&R);
    bi_delete(&S);
    bi_delete(&C);

    END_TIMER(result, SUCCESS_SQUARE_A);
    return result;
}

/**
 * @brief bigint A의 제곱을 수행하는 함수
 * @param dst 결과를 저장할 bigint의 포인터
 * @param A 제곱을 수행할 bigint
 * @param is_karatsuba 카라추바 제곱을 사용할지 여부
 */
res bi_square(OUT bigint** dst, IN bigint* A, IN bool is_karatsuba)
{   
    res result;
    START_TIMER();

    if ( is_karatsuba) bi_square_karatsuba(dst, A);
    if (!is_karatsuba) bi_square_C(dst, A);

    END_TIMER(result, SUCCESS_SQUARE_A);
    return result;
}