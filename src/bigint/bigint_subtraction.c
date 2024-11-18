/**
 * @file bigint_subtraction.c
 * @brief bigint에 대한 뺄셈 연산을 하는 함수들을 정의한 소스 파일
 * @date 2024-11-01
 * @author 김남일
 */
#include <stdio.h>
#include "base/base_type.h"
#include "base/base_error.h"
#include "bigint/bigint_addition.h"
#include "bigint/bigint_calculation.h"

/**
 * @brief A - B - borrow 연산을 수행하는 함수
 * @param dst 결과값을 저장할 변수
 * @param A 피연산자 A
 * @param B 피연산자 B
 * @param borrow 이전 연산에서 발생한 borrow
 */
msg bi_sub_ABb(OUT word* dst, IN word* A, IN word* B, IN byte* borrow)
{
	byte tmp = 0;
	*dst = *A - *borrow;
	if (*A < *borrow)
		tmp = 1;
	if (*dst < *B)
		tmp = 1;
	*dst = *dst - *B;
	*borrow = tmp;

	return print_success_sub_AbB();
}

/**
 * @brief A - borrow 연산을 수행하는 함수
 * @param dst 결과값을 저장할 변수
 * @param A 피연산자 A
 * @param borrow 이전 연산에서 발생한 borrow
 */
msg bi_sub_Ab(OUT word* dst, IN word* A, IN byte* borrow)
{
	byte tmp = 0;
	*dst = *A - *borrow;
	if (*A < *borrow)
		tmp = 1;
	*borrow = tmp;

	return print_success_sub_Ab();
}

/**
 * @brief 두 bigint의 뺄셈을 수행하는 함수.
 * @param dst 결과값을 저장할 변수
 * @param A 피연산자 A
 * @param B 피연산자 B
 */
msg bi_sub_C(OUT bigint** dst, IN bigint* A, IN bigint* B)
{
    byte b = 0;
    word C = 0;
    bigint* bi_sub = NULL;
    bi_new(&bi_sub, A->wordlen);

    for (int j = 0; j < B->wordlen; j++) 
	{
        bi_sub_ABb(&C, &(A->start[j]), &(B->start[j]), &b);
        bi_sub->start[j] = C;
        C = 0;
    }
	for (int j = B->wordlen; j < A->wordlen; j++) {
		bi_sub_Ab(&C, &(A->start[j]), &b);
		bi_sub->start[j] = C;
		C = 0;
	}
    bi_refine(bi_sub);
    bi_refine(B);
    bi_assign(dst, bi_sub);
    bi_delete(&bi_sub);

	return print_success_sub_C();
}

/**
 * @brief 두 bigint의 뺄셈을 수행하는 함수
 * @param dst 결과값을 저장할 변수
 * @param A 피연산자 A
 * @param B 피연산자 B
 */
msg bi_sub(OUT bigint** dst, IN bigint* A, IN bigint* B)
{
	bigint* tmp_A = NULL;
	bigint* tmp_B = NULL;

	bi_assign(&tmp_A, A);
	bi_assign(&tmp_B, B);

	if ((tmp_A->sign == POSITIVE) && (tmp_B->sign == POSITIVE) && (bi_compare(tmp_A, tmp_B) != -1))
	{
		bi_sub_C(dst, tmp_A, tmp_B);
		(*dst)->sign = POSITIVE;
		bi_delete(&tmp_A);
		bi_delete(&tmp_B);
		
		return print_success_sub();
	}
	if ((tmp_A->sign == POSITIVE) && (tmp_B->sign == POSITIVE) && (bi_compare(tmp_A, tmp_B) == -1))
	{
		bi_sub_C(dst, tmp_B, tmp_A);
		(*dst)->sign = NEGATIVE;
		bi_delete(&tmp_A);
		bi_delete(&tmp_B);
		
		return print_success_sub();
	}
	if ((tmp_A->sign == NEGATIVE) && (tmp_B->sign == NEGATIVE) && (bi_compare(tmp_A, tmp_B) != -1))
	{
		tmp_A->sign = POSITIVE;
		tmp_B->sign = POSITIVE;
		bi_sub_C(dst, tmp_B, tmp_A);
		bi_delete(&tmp_A);
		bi_delete(&tmp_B);
		
		return print_success_sub();
	}
	if ((tmp_A->sign == NEGATIVE) && (tmp_B->sign == NEGATIVE) && (bi_compare(tmp_A, tmp_B) == -1))
	{
		tmp_A->sign = POSITIVE;
		tmp_B->sign = POSITIVE;
		bi_sub_C(dst, tmp_A, tmp_B);
		(*dst)->sign = NEGATIVE;
		bi_delete(&tmp_A);
		bi_delete(&tmp_B);
		
		return print_success_sub();
	}
	if ((tmp_A->sign == POSITIVE) && (tmp_B->sign == NEGATIVE))
	{
		tmp_B->sign = POSITIVE;
		bi_add(dst, tmp_A, tmp_B);
		(*dst)->sign = POSITIVE;
		bi_delete(&tmp_A);
		bi_delete(&tmp_B);
		
		return print_success_sub();
	}
	if ((tmp_A->sign == NEGATIVE) && (tmp_B->sign == POSITIVE))
	{
		tmp_A->sign = POSITIVE;
		bi_add(dst, tmp_A, tmp_B);
		(*dst)->sign = NEGATIVE;
		bi_delete(&tmp_A);
		bi_delete(&tmp_B);
		
		return print_success_sub();
	}

	return print_success_sub();
}

/**
 * @brief bigint 뺄셈 테스트 함수
 */
void bi_test_sub()
{
	fprintf(stdout, "===========================\n");
    fprintf(stdout, "BigInt 뺄셈 테스트 시작\n");
    fprintf(stdout, "===========================\n\n");

    fprintf(stdout, "1. 일반적인 뺄셈 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "1.1 두 양수 A와 B를 뺄 때 (A > B)\n");
    // Implement test case logic for positive A > B

    fprintf(stdout, "1.2 두 양수 A와 B를 뺄 때 (A < B)\n");
    // Implement test case logic for positive A < B

    fprintf(stdout, "1.3 두 음수 A와 B를 뺄 때 (|A| > |B|)\n");
    // Implement test case logic for negative subtraction |A| > |B|

    fprintf(stdout, "1.4 두 음수 A와 B를 뺄 때 (|A| < |B|)\n");
    // Implement test case logic for negative subtraction |A| < |B|

    fprintf(stdout, "\n2. 양수와 음수의 조합 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "2.1 양의 정수 A에서 음의 정수 B를 뺄 때 (A - (-B))\n");
    // Implement test case logic for A - (-B)

    fprintf(stdout, "2.2 음의 정수 A에서 양의 정수 B를 뺄 때 (-A - B)\n");
    // Implement test case logic for -A - B

    fprintf(stdout, "\n3. 0과의 연산 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "3.1 A - 0 (결과는 A)\n");
    // Implement test case logic for A - 0

    fprintf(stdout, "3.2 0 - A (결과는 -A)\n");
    // Implement test case logic for 0 - A

    fprintf(stdout, "3.3 0 - 0 (결과는 0)\n");
    // Implement test case logic for 0 - 0

    fprintf(stdout, "\n4. 동일한 값의 뺄셈\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "4.1 A - A (결과는 0)\n");
    // Implement test case logic for A - A

    fprintf(stdout, "4.2 -A - (-A) (결과는 0)\n");
    // Implement test case logic for -A - (-A)

    fprintf(stdout, "\n5. 자리 내림 발생 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "5.1 자리 내림이 필요한 경우\n");
    // Implement test case logic for borrow

    fprintf(stdout, "5.2 연속적인 자리 내림\n");
    // Implement test case logic for consecutive borrows

    fprintf(stdout, "\n6. 다른 길이의 bigint 뺄셈 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "6.1 A의 길이가 B보다 긴 경우\n");
    // Implement test case logic for A longer than B

    fprintf(stdout, "6.2 A의 길이가 B보다 짧은 경우\n");
    // Implement test case logic for A shorter than B

    fprintf(stdout, "6.3 A와 B의 길이가 같은 경우\n");
    // Implement test case logic for equal length

    fprintf(stdout, "\n7. 경계 조건 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "7.1 A 또는 B가 NULL인 경우\n");
    // Implement test case logic for NULL pointers

    fprintf(stdout, "7.2 A와 B 중 하나 또는 둘 다 길이가 0인 경우\n");
    // Implement test case logic for zero-length

    fprintf(stdout, "7.3 A와 B가 모두 1 비트로 구성된 경우\n");
    // Implement test case logic for single-bit bigint

    fprintf(stdout, "\n8. 메모리 관련 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "8.1 메모리 할당 실패\n");
    // Implement test case logic for memory allocation failure

    fprintf(stdout, "8.2 dst가 비어 있거나 초기화되지 않은 경우\n");
    // Implement test case logic for uninitialized dst

    fprintf(stdout, "\n9. 임의의 값 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "9.1 A와 B에 큰 정수 값 할당\n");
    // Implement test case logic for large values

    fprintf(stdout, "9.2 A와 B에 작은 값 할당 (예: 1, -1)\n");
    // Implement test case logic for small values

    fprintf(stdout, "\n10. 부호가 다른 경우\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "10.1 A가 양수이고 B가 음수인 경우 (A - (-B))\n");
    // Implement test case logic for A - (-B)

    fprintf(stdout, "10.2 A가 음수이고 B가 양수인 경우 (-A - B)\n");
    // Implement test case logic for -A - B

    fprintf(stdout, "\n11. 최대 및 최소 값 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "11.1 A가 최대 정수 값이고 B가 1인 경우\n");
    // Implement test case logic for max int - 1

    fprintf(stdout, "11.2 A가 최소 정수 값이고 B가 음수인 경우\n");
    // Implement test case logic for min int - negative

    fprintf(stdout, "\n12. 단일 워드 뺄셈 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "12.1 단일 워드로 구성된 A와 B의 뺄셈\n");
    // Implement test case logic for single-word subtraction

    fprintf(stdout, "12.2 A < B일 때 단일 워드 뺄셈 (음수 결과)\n");
    // Implement test case logic for negative result

        fprintf(stdout, "\n13. 중첩된 뺄셈 테스트\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "13.1 여러 번 중첩된 bi_sub 호출로 연속된 뺄셈 수행 ((((A - B) - C) - D) 형태)\n");
    // Implement test case logic for deep nested subtraction
    bigint *result = NULL;
    // bigint *A = bigint_create_example(7); // Example function to create a sample bigint
    // bigint *B = bigint_create_example(5);
    // bigint *C = bigint_create_example(3);
    // bigint *D = bigint_create_example(1);
    
    // fprintf(stdout, "Initial A: ");
    // print_bigint(*A);
    // fprintf(stdout, ", B: ");
    // print_bigint(*B);
    // fprintf(stdout, ", C: ");
    // print_bigint(*C);
    // fprintf(stdout, ", D: ");
    // print_bigint(*D);
    // fprintf(stdout, "\n");

    // bi_sub(&result, A, B);
    // fprintf(stdout, "Step 1 result (A - B): ");
    // print_bigint(*result);
    // fprintf(stdout, "\n");
    
    // bi_sub(&result, result, C);
    // fprintf(stdout, "Step 2 result ((A - B) - C): ");
    // print_bigint(*result);
    // fprintf(stdout, "\n");

    // bi_sub(&result, result, D);
    // fprintf(stdout, "Final result ((((A - B) - C) - D)): ");
    // print_bigint(*result);
    // fprintf(stdout, "\n");

    // // Clean up
    // bi_delete(&A);
    // bi_delete(&B);
    // bi_delete(&C);
    // bi_delete(&D);
    // bi_delete(&result);

    // fprintf(stdout, "\n14. 정수 비교에 따른 연산\n");
    // fprintf(stdout, "---------------------------\n");
    // fprintf(stdout, "14.1 bi_compare 함수의 결과에 따른 양수, 음수, 같은 값 테스트\n");
    // bigint *X = bigint_create_example(6);
    // bigint *Y = bigint_create_example(6);

    // int cmp_result = bi_compare(X, Y);
    // fprintf(stdout, "Comparing X and Y: ");
    // print_bigint(*X);
    // fprintf(stdout, " vs ");
    // print_bigint(*Y);
    // fprintf(stdout, "\n");
    // if (cmp_result > 0) {
    //     fprintf(stdout, "Result: X > Y\n");
    // } else if (cmp_result < 0) {
    //     fprintf(stdout, "Result: X < Y\n");
    // } else {
    //     fprintf(stdout, "Result: X == Y\n");
    // }

    // bi_delete(&X);
    // bi_delete(&Y);

    // fprintf(stdout, "\n15. bi_refine 결과 확인\n");
    // fprintf(stdout, "---------------------------\n");
    // fprintf(stdout, "15.1 결과의 상위 워드가 0일 때, bi_refine 후 wordlen 조정 확인\n");
    // bigint *Z = bigint_create_example_with_trailing_zeros();
    // fprintf(stdout, "Initial Z: ");
    // print_bigint(*Z);
    // fprintf(stdout, "\n");

    // bi_refine(Z);
    // fprintf(stdout, "Refined Z: ");
    // print_bigint(*Z);
    // fprintf(stdout, "\n");
    // fprintf(stdout, "Z wordlen after refine: %d\n", Z->wordlen);

    // bi_delete(&Z);

    // fprintf(stdout, "15.2 wordlen이 감소하거나 유지되는지 확인\n");
    // bigint *W = bigint_create_example_no_trailing_zeros();
    // fprintf(stdout, "Initial W: ");
    // print_bigint(*W);
    // fprintf(stdout, "\n");

    // bi_refine(W);
    // fprintf(stdout, "Refined W: ");
    // print_bigint(*W);
    // fprintf(stdout, "\n");
    // fprintf(stdout, "W wordlen after refine: %d\n", W->wordlen);

    // bi_delete(&W);

    fprintf(stdout, "\n===========================\n");
    fprintf(stdout, "BigInt 뺄셈 테스트 완료\n");
    fprintf(stdout, "===========================\n");
}