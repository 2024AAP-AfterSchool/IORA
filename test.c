#include <stdio.h>
#include "struct.h"
#include "base.h"
#include "error.h"
#include <time.h>
# define NON_NEGATIVE 0 

/*
int main() {
    // 랜덤 시드 설정
    srand((unsigned int)time(NULL));  // 현재 시간을 기반으로 시드 설정

    // 1. bi_new 및 bi_delete 테스트
    printf("=== Testing bi_new and bi_delete ===\n");
    bigint* num1 = NULL;
    msg result = bi_new(&num1, 5);

    if (result == SUCCESS_INITIALIZATION) {
        printf("bi_new: SUCCESS\n");
    }
    else {
        printf("bi_new: FAILED\n");
    }

    result = bi_delete(&num1);
    if (result == SUCCESS_MEMORY_DEALLOCATION) {
        printf("bi_delete: SUCCESS\n");
    }
    else {
        printf("bi_delete: FAILED\n");
    }
    printf("\n");

    // 2. bi_gen_random 테스트
    printf("=== Testing bi_gen_random ===\n");
    bigint* num2 = NULL;
    result = bi_gen_random(&num2, NON_NEGATIVE, 5);

    if (result == SUCCESS_INITIALIZATION) {
        printf("bi_gen_random: SUCCESS\n");
    }
    else {
        printf("bi_gen_random: FAILED\n");
    }

    // 생성된 bigint 출력
    if (num2 != NULL) {
        bi_print(num2, 16);
        printf("\n");
    }

    result = bi_delete(&num2);
    if (result == SUCCESS_MEMORY_DEALLOCATION) {
        printf("bi_delete: SUCCESS\n");
    }
    else {
        printf("bi_delete: FAILED\n");
    }
    printf("\n");

    // 3. bi_set_from_array 테스트
    printf("=== Testing bi_set_from_array ===\n");
    word arr[5] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
    bigint* num3 = NULL;
    result = bi_set_from_array(&num3, NON_NEGATIVE, 5, arr);

    if (result == SUCCESS_INITIALIZATION) {
        printf("bi_set_from_array: SUCCESS\n");
    }
    else {
        printf("bi_set_from_array: FAILED\n");
    }

    // 생성된 bigint 출력
    if (num3 != NULL) {
        bi_print(num3, 16);
        printf("\n");
    }

    result = bi_delete(&num3);
    if (result == SUCCESS_MEMORY_DEALLOCATION) {
        printf("bi_delete: SUCCESS\n");
    }
    else {
        printf("bi_delete: FAILED\n");
    }
    printf("\n");

    // 4. bi_assign 테스트
    printf("=== Testing bi_assign ===\n");
    bigint* src = NULL;
    bigint* dst = NULL;

    bi_gen_random(&src, NON_NEGATIVE, 5);
    result = bi_assign(&dst, src);

    if (result == SUCCESS_INITIALIZATION) {
        printf("bi_assign: SUCCESS\n");
    }
    else {
        printf("bi_assign: FAILED\n");
    }

    // src 및 dst bigint 출력하여 비교
    if (src != NULL) {
        printf("Source bigint: ");
        bi_print(src, 16);
        printf("\n");
    }

    if (dst != NULL) {
        printf("Assigned bigint: ");
        bi_print(dst, 16);
        printf("\n");
    }

    // 메모리 해제
    bi_delete(&src);
    bi_delete(&dst);
    printf("\n");

    // 5. bi_refine 테스트
    printf("=== Testing bi_refine ===\n");
    bigint* num4 = NULL;
    bi_gen_random(&num4, NON_NEGATIVE, 5);

    // Bigint 출력 전
    printf("Before refinement: ");
    bi_print(num4, 16);
    printf("\n");

    // 정제 후
    result = bi_refine(num4);
    if (result == SUCCESS_REFINEMENT) {
        printf("bi_refine: SUCCESS\n");
    }
    else {
        printf("bi_refine: FAILED\n");
    }

    // 정제된 bigint 출력
    printf("After refinement: ");
    bi_print(num4, 16);
    printf("\n");

    bi_delete(&num4);

    return 0;
}

int main() {
    // 랜덤 시드 설정
    srand((unsigned int)time(NULL));  // 현재 시간을 기반으로 시드 설정

    // 5. bi_refine 테스트 - 상위 비트에 0이 있는 값
    printf("=== Testing bi_refine (with leading zeros) ===\n");
    bigint* num4 = NULL;

    // 상위 비트에 0이 포함된 배열 생성
    word arr_with_zeros[5] = { 0x00000000, 0x00000000, 0x00000003, 0x00000000, 0x00000000 };
    bi_set_from_array(&num4, NON_NEGATIVE, 5, arr_with_zeros);

    // 정제 전 출력
    printf("Before refinement: ");
    bi_print(num4, 16);
    printf("\n");

    // 정제 후
    msg result = bi_refine(num4);
    if (result == SUCCESS_REFINEMENT) {
        printf("bi_refine: SUCCESS\n");
    }
    else {
        printf("bi_refine: FAILED\n");
    }

    // 정제 후 출력
    printf("After refinement: ");
    bi_print(num4, 16);
    printf("\n");

    bi_delete(&num4);

    return 0;
}
*/
/*
int main() {
    bigint* test_bigint = NULL;  // 초기화를 NULL로 설정
    msg result;

    // 케이스 1: 배열의 마지막 원소가 0이 아닌 경우 (정제가 일어나지 않아야 함)
    printf("Test Case 1: No refinement needed\n");
    result = bi_new(&test_bigint, 3);  // word_len을 3으로 설정
    if (result != SUCCESS_INITIALIZATION || test_bigint == NULL || test_bigint->a == NULL) {
        printf("Error: Failed to initialize bigint.\n");
        return -1;
    }

    test_bigint->a[0] = 0x12345678;
    test_bigint->a[1] = 0x87654321;
    test_bigint->a[2] = 0x11111111;  // 마지막 원소가 0이 아닌 경우
    test_bigint->sign = NON_NEGATIVE;

    printf("Before refinement: word_len = %d\n", test_bigint->word_len);
    result = bi_refine(test_bigint);
    if (result != SUCCESS_REFINE) {
        printf("Error: Refinement failed.\n");
        bi_delete(&test_bigint);
        return -1;
    }
    printf("After refinement: word_len = %d\n", test_bigint->word_len);
    bi_print(test_bigint, 16);

    bi_delete(&test_bigint);  // 메모리 해제

    // 케이스 2: 배열의 마지막 원소가 0인 경우 (정제가 발생해야 함)
    printf("\nTest Case 2: Refinement needed\n");
    result = bi_new(&test_bigint, 4);  // word_len을 4로 설정
    if (result != SUCCESS_INITIALIZATION || test_bigint == NULL || test_bigint->a == NULL) {
        printf("Error: Failed to initialize bigint.\n");
        return -1;
    }

    test_bigint->a[0] = 0x12345678;
    test_bigint->a[1] = 0x87654321;
    test_bigint->a[2] = 0x00000000;  // 마지막 원소가 0인 경우
    test_bigint->a[3] = 0x00000000;  // 또 다른 0
    test_bigint->sign = NON_NEGATIVE;

    printf("Before refinement: word_len = %d\n", test_bigint->word_len);
    result = bi_refine(test_bigint);
    if (result != SUCCESS_REFINE) {
        printf("Error: Refinement failed.\n");
        bi_delete(&test_bigint);
        return -1;
    }
    printf("After refinement: word_len = %d\n", test_bigint->word_len);
    bi_print(test_bigint, 16);

    bi_delete(&test_bigint);  // 메모리 해제

    // 케이스 3: 배열 전체가 0인 경우 (부호가 NON_NEGATIVE로 설정되어야 함)
    printf("\nTest Case 3: All elements are zero\n");
    result = bi_new(&test_bigint, 3);  // word_len을 3으로 설정
    if (result != SUCCESS_INITIALIZATION || test_bigint == NULL || test_bigint->a == NULL) {
        printf("Error: Failed to initialize bigint.\n");
        return -1;
    }

    test_bigint->a[0] = 0x00000000;
    test_bigint->a[1] = 0x00000000;
    test_bigint->a[2] = 0x00000000;
    test_bigint->sign = -1;  // 부호를 음수로 설정

    printf("Before refinement: word_len = %d, sign = %d\n", test_bigint->word_len, test_bigint->sign);
    result = bi_refine(test_bigint);
    if (result != SUCCESS_REFINE) {
        printf("Error: Refinement failed.\n");
        bi_delete(&test_bigint);
        return -1;
    }
    printf("After refinement: word_len = %d, sign = %d\n", test_bigint->word_len, test_bigint->sign);
    bi_print(test_bigint, 16);

    bi_delete(&test_bigint);  // 메모리 해제

    return 0;
}
*/


int main() {
    bigint* number = NULL;  // 반드시 NULL로 초기화
    char hex_string1[] = "0x1A2B3C4D5E6F";  // 정상적인 16진수 문자열
    char hex_string2[] = "-0xABCDEF123456"; // 음수 16진수 문자열
    char hex_string3[] = "1A2B3C";          // '0x' 없이 16진수 문자열
    char hex_string4[] = "0x";              // 빈 16진수
    char hex_string5[] = "-0x";             // 빈 음수 16진수
    char invalid_hex[] = "0x1A2G3C";        // 유효하지 않은 문자 'G' 포함
    char long_hex[] = "0x123456789ABCDEF0123456789ABCDEF";  // 긴 16진수 문자열

    // 테스트 1: 정상적인 16진수 입력
    printf("Test Case 1: Valid Hex String\n");
    msg result = bi_set_from_string(&number, hex_string1, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        printf("Bigint successfully created from string.\n");
        bi_print(number, 16);  // bigint 출력
    }
    else {
        printf("Failed to create bigint from string.\n");
    }
    bi_delete(&number);  // 메모리 해제

    // 테스트 2: 음수 16진수 입력
    printf("\nTest Case 2: Negative Hex String\n");
    result = bi_set_from_string(&number, hex_string2, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        printf("Bigint successfully created from negative string.\n");
        bi_print(number, 16);  // bigint 출력
    }
    else {
        printf("Failed to create bigint from negative string.\n");
    }
    bi_delete(&number);  // 메모리 해제

    // 테스트 3: '0x' 없이 16진수 입력
    printf("\nTest Case 3: Hex String without '0x'\n");
    result = bi_set_from_string(&number, hex_string3, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        printf("Bigint successfully created from string without '0x'.\n");
        bi_print(number, 16);  // bigint 출력
    }
    else {
        printf("Failed to create bigint from string without '0x'.\n");
    }
    bi_delete(&number);  // 메모리 해제

    // 테스트 4: 빈 16진수 입력
    printf("\nTest Case 4: Empty Hex String\n");
    result = bi_set_from_string(&number, hex_string4, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        printf("Bigint successfully created from empty string.\n");
        bi_print(number, 16);  // bigint 출력
    }
    else {
        printf("Failed to create bigint from empty string.\n");
    }
    bi_delete(&number);  // 메모리 해제

    // 테스트 5: 빈 음수 16진수 입력
    printf("\nTest Case 5: Empty Negative Hex String\n");
    result = bi_set_from_string(&number, hex_string5, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        printf("Bigint successfully created from empty negative string.\n");
        bi_print(number, 16);  // bigint 출력
    }
    else {
        printf("Failed to create bigint from empty negative string.\n");
    }
    bi_delete(&number);  // 메모리 해제

    // 테스트 6: 유효하지 않은 문자 포함
    printf("\nTest Case 6: Invalid Hex String (contains 'G')\n");
    result = bi_set_from_string(&number, invalid_hex, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        printf("Bigint successfully created from string with invalid character.\n");
        bi_print(number, 16);  // bigint 출력
    }
    else {
        printf("Failed to create bigint from string with invalid character.\n");
    }

    // 테스트 7: 긴 16진수 입력
    printf("\nTest Case 7: Long Hex String\n");
    result = bi_set_from_string(&number, long_hex, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        printf("Bigint successfully created from long string.\n");
        bi_print(number, 16);  // bigint 출력
    }
    else {
        printf("Failed to create bigint from long string.\n");
    }
    bi_delete(&number);  // 메모리 해제

    return 0;
}
