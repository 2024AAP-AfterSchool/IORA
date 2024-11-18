/**
 * @file bigint_addition.c
 * @brief bigint에 대한 덧셈 연산을 하는 함수들을 정의한 소스 파일
 * @date 2024-11-01
 * @author 송원우
 */
#include <stdio.h>
#include "base/base_type.h"
#include "base/base_error.h"
#include "bigint/bigint_subtraction.h"
#include "bigint/bigint_calculation.h"

/**
 * @brief A + B + c 연산을 수행하는 함수
 * @param C 결과값을 저장할 변수
 * @param A 피연산자 A
 * @param B 피연산자 B
 * @param c 이전 연산에서 발생한 carry
 */
msg bi_add_ABC(OUT word* C, IN word A, IN word B, IN carry c, IN carry* c_prime)
{
    *c_prime = 0;  // 초기 캐리를 0으로 설정
    *C = A + B;
    if (*C < A)
    {
        *c_prime = 1;
    }
    *C += c;
    if (*C < c)
    {
        *c_prime += 1;
    }

    return print_success_add_ABC();
}

/**
 * @brief A + B 연산을 수행하는 함수
 * @param C 결과값을 저장할 변수
 * @param A 피연산자 A
 * @param B 피연산자 B
 */
msg bi_add_C(OUT bigint** dst, IN bigint* A, IN bigint* B)
{
    if (A == NULL || A->start == NULL)
    {
        return NULL_POINTER_ERROR;
    }
    if (B == NULL || B->start == NULL)
    {
        return NULL_POINTER_ERROR;
    }

    int n = A->wordlen;
    int m = B->wordlen;

    // 결과를 저장할 임시 bigint 생성
    bigint* temp = NULL;

    bi_new(&temp, n + 1);  // 최대 n + 1 자리 (자리 올림 고려)
    
    carry c = 0;
    word C_word;

    // A와 B의 각 워드 덧셈 수행
    for (int j = 0; j < n; j++)
    {
        word A_word = A->start[j];
        word B_word = (j < m) ? B->start[j] : 0;

        bi_add_ABC(&C_word, A_word, B_word, c, &c);

        temp->start[j] = C_word;
    }

    // 최종 자리 올림(carry)을 저장
    temp->start[n] = c;
    temp->wordlen = (c == 0) ? n : n + 1;

    bi_refine(temp);

    bi_assign(dst, temp);
    
    bi_delete(&temp);

    return print_success_add_C();
}

/**
 * @brief 두 bigint의 덧셈을 수행하는 함수
 * @param result 결과값을 저장할 변수
 * @param a 피연산자 a
 * @param b 피연산자 b
 */
msg bi_add(OUT bigint** dst, IN bigint* A, IN bigint* B)
{
    if (A == NULL || B == NULL)
    {
        return NULL_POINTER_ERROR;
    }

    // 임시 변수 temp 생성
    bigint* temp = NULL;
    bi_new(&temp, A->wordlen + 1); // A->wordlen + 1 크기 할당

    // A와 B의 부호에 따라 덧셈 또는 뺄셈 연산 수행
    if ((A->sign == 0) && (B->sign == 1))
    {
        bi_sub(&temp, A, B); 
    }
    else if ((B->sign == 0) && (A->sign == 1))
    {
        bi_sub(&temp, B, A);
    }
    else if ((B->sign == 1) && (A->sign == 1))
    {
        if (A->wordlen >= B->wordlen)
        {
            bi_add_C(&temp, A, B); 
            temp->sign = 1; 
        }
        else
        {
            bi_add_C(&temp, B, A); 
            temp->sign = 1; 
        }
    }
    else
    { 
        if (A->wordlen >= B->wordlen)
        {
            bi_add_C(&temp, A, B); 
        }
        else
        {
            bi_add_C(&temp, B, A);
        }
    }
    
    bi_refine(temp);
    bi_assign(dst, temp);
    bi_delete(&temp); 

    return SUCCESS_ADD;
}

/**
 * @brief bigint 덧셈 테스트 함수
 */
void bi_test_add()
{   

    fprintf(stdout, "===========================\n");
    fprintf(stdout, "BigInt 덧셈 테스트 시작\n");
    fprintf(stdout, "===========================\n");

    fprintf(stdout, "\n1. 일반적인 덧셈 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "1.1 두 양수 A와 B를 더하는 경우\n");
    bigint* A = NULL;
    bigint* B = NULL;
    bigint* dst = NULL;

    bi_get_random(&A, POSITIVE, 3);
    bi_get_random(&B, POSITIVE, 3);
    bi_add(&dst, A, B);

    fprintf(stdout, "1.2 두 음수 A와 B를 더하는 경우\n");
    // Implement test case logic for negative addition

    fprintf(stdout, "1.3 하나의 양수와 하나의 음수 덧셈 (결과가 양수)\n");
    // Implement test case logic for mixed sign (positive result)

    fprintf(stdout, "1.4 하나의 양수와 하나의 음수 덧셈 (결과가 음수)\n");
    // Implement test case logic for mixed sign (negative result)

    fprintf(stdout, "\n2. 자리 올림 발생 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "2.1 자리 올림이 발생하는 경우\n");
    // Implement test case logic for carry

    fprintf(stdout, "2.2 연속적인 자리 올림\n");
    // Implement test case logic for consecutive carries

    fprintf(stdout, "\n3. 다른 길이의 bigint 덧셈 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "3.1 A의 길이가 B보다 긴 경우\n");
    // Implement test case logic for A longer than B

    fprintf(stdout, "3.2 A의 길이가 B보다 짧은 경우\n");
    // Implement test case logic for B longer than A

    fprintf(stdout, "3.3 A와 B의 길이가 같은 경우\n");
    // Implement test case logic for equal length

    fprintf(stdout, "\n4. 경계 조건 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "4.1 A 또는 B가 NULL인 경우\n");
    // Implement test case logic for NULL pointers

    fprintf(stdout, "4.2 A와 B 중 하나 또는 둘 다 길이가 0인 경우\n");
    // Implement test case logic for zero-length

    fprintf(stdout, "4.3 A와 B가 모두 0인 경우\n");
    // Implement test case logic for both zero

    fprintf(stdout, "4.4 A 또는 B가 0x0으로 구성된 경우\n");
    // Implement test case logic for single 0 word

    fprintf(stdout, "\n5. 메모리 관련 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "5.1 메모리 할당 실패\n");
    // Implement test case logic for memory allocation failure

    fprintf(stdout, "5.2 dst가 비어 있거나 초기화되지 않은 경우\n");
    // Implement test case logic for uninitialized dst

    fprintf(stdout, "\n6. 임의의 값 및 캐리 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "6.1 임의의 값\n");
    // Implement test case logic for arbitrary values

    fprintf(stdout, "6.2 이전 캐리가 1인 상태에서 덧셈\n");
    // Implement test case logic for carry 1

    fprintf(stdout, "\n7. bi_refine 결과 확인\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "7.1 상위 워드가 0일 때, bi_refine 후 wordlen 확인\n");
    // Implement test case logic for bi_refine

    fprintf(stdout, "7.2 자리 올림으로 인해 wordlen 증가 확인\n");
    // Implement test case logic for wordlen increase

    fprintf(stdout, "\n8. 이진 덧셈 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "8.1 1비트 차이의 이진수 덧셈\n");
    // Implement test case logic for binary addition

    fprintf(stdout, "\n9. 특수 케이스\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "9.1 A가 최대 정수 값이고 B가 1인 경우\n");
    // Implement test case logic for max int + 1

    fprintf(stdout, "9.2 A가 최소 정수 값이고 B가 음수인 경우\n");
    // Implement test case logic for min int + negative

    fprintf(stdout, "\n10. 단일 워드 덧셈 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "10.1 단일 워드로 구성된 A와 B의 덧셈\n");
    // Implement test case logic for single-word addition

    fprintf(stdout, "10.2 c가 0인 경우와 1인 경우\n");
    // Implement test case logic for carry 0 and 1

    fprintf(stdout, "\n11. 중첩된 덧셈 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "11.1 여러 번 중첩된 bi_add 호출\n");
    // Implement test case logic for nested addition

    fprintf(stdout, "\n===========================\n");
    fprintf(stdout, "BigInt 덧셈 테스트 완료\n");
    fprintf(stdout, "===========================\n");
}