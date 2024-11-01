#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base/base_type.h"
#include "base/base_error.h"
#include "bigint/bigint_calculation.h"

int main() {
    
    fprintf(stdout, "\n############################################\n\n");
    fprintf(stdout, "            START PROGRAM TEST\n");
    
    msg result;
    bigint* test_bigint = NULL;  // 초기화를 NULL로 설정

    fprintf(stdout, "\n############################################\n\n");

    // 케이스 1: 배열의 마지막 원소가 0이 아닌 경우 (정제가 일어나지 않아야 함)
    fprintf(stdout, "[+] Test Case 1: No refinement needed\n");
    result = bi_new(&test_bigint, 3);  // word_len을 3으로 설정
    if (result != SUCCESS_INITIALIZATION || test_bigint == NULL || test_bigint->start == NULL)
    {
        fprintf(stdout, "[X] Error: Failed to initialize bigint.  Error Code: %04d\n", result);
        return -1;
    }

    test_bigint->start[0] = 0x12345678;
    test_bigint->start[1] = 0x87654321;
    test_bigint->start[2] = 0x11111111;  // 마지막 원소가 0이 아닌 경우
    test_bigint->sign = POSITIVE;

    fprintf(stdout, "[O] Before refinement: word_len = %d\n", test_bigint->wordlen);
    result = bi_refine(test_bigint);
    if (result != SUCCESS_REFINE)
    {
        fprintf(stdout, "[X] Error: Refinement failed. Error Code: %04d\n", result);
        bi_delete(&test_bigint);
        return -1;
    }
    fprintf(stdout, "[O] After refinement: word_len = %d\n", test_bigint->wordlen);
    bi_print(test_bigint, 16);

    result = bi_delete(&test_bigint);  // 메모리 해제
    if (result != SUCCESS_MEMORY_DEALLOCATION)
    {
        fprintf(stdout, "[X] Error: Failed to deallocate memory. Error Code: %04d\n", result);
        return -1;
    }
    fprintf(stdout, "[O] Memory deallocation result: %d\n", result);

    fprintf(stdout, "\n############################################\n\n");

    // 케이스 2: 배열의 마지막 원소가 0인 경우 (정제가 발생해야 함)
    fprintf(stdout, "[+] Test Case 2: Refinement needed\n");
    result = bi_new(&test_bigint, 4);  // word_len을 4로 설정
    if (result != SUCCESS_INITIALIZATION || test_bigint == NULL || test_bigint->start == NULL)
    {
        fprintf(stdout, "[X] Error: Failed to initialize bigint. Error Code: %04d\n", result);
        return -1;
    }

    test_bigint->start[0] = 0x12345678;
    test_bigint->start[1] = 0x87654321;
    test_bigint->start[2] = 0x00000000;  // 마지막 원소가 0인 경우
    test_bigint->start[3] = 0x00000000;  // 또 다른 0
    test_bigint->sign = POSITIVE;

    fprintf(stdout, "[O] Before refinement: word_len = %d\n", test_bigint->wordlen);
    result = bi_refine(test_bigint);
    if (result != SUCCESS_REFINE)
    {
        fprintf(stdout, "[-] Error: Refinement failed. Error Code: %04d\n", result);
        bi_delete(&test_bigint);
        return -1;
    }
    fprintf(stdout, "[O] After refinement: word_len = %d\n", test_bigint->wordlen);
    bi_print(test_bigint, 16);

    bi_delete(&test_bigint);  // 메모리 해제

    fprintf(stdout, "\n############################################\n\n");

    // 케이스 3: 배열 전체가 0인 경우 (부호가 POSITIVE로 설정되어야 함)
    fprintf(stdout, "[+] Test Case 3: All elements are zero\n");
    result = bi_new(&test_bigint, 3);  // word_len을 3으로 설정
    if (result != SUCCESS_INITIALIZATION || test_bigint == NULL || test_bigint->start == NULL)
    {
        fprintf(stdout, "[-] Error: Failed to initialize bigint.  Error Code: %04d\n", result);
        return -1;
    }

    test_bigint->start[0] = 0x00000000;
    test_bigint->start[1] = 0x00000000;
    test_bigint->start[2] = 0x00000000;
    test_bigint->sign = NEGATIVE;

    fprintf(stdout, "[O] Before refinement: word_len = %d, sign = %d\n", test_bigint->wordlen, test_bigint->sign);
    result = bi_refine(test_bigint);
    if (result != SUCCESS_REFINE)
    {
        fprintf(stdout, "[X] Error: Refinement failed. Error Code: %04d\n", result);
        bi_delete(&test_bigint);
        return -1;
    }

    fprintf(stdout, "After refinement: word_len = %d, sign = %d\n", test_bigint->wordlen, test_bigint->sign);
    bi_print(test_bigint, 16);

    bi_delete(&test_bigint);  // 메모리 해제

    fprintf(stdout, "\n############################################\n\n");

    // Test Case 4: array_copy 테스트
    fprintf(stdout, "[+] Test Case 4: array_copy Test\n");

    word src[] = {0x12345678, 0x87654321, 0xABCDEF01};
    word dst[3] = {0};
    result = array_copy(dst, src, 3);  // word 배열 복사
    if (result != SUCCESS_COPY) {
        fprintf(stdout, "[X] Error: array_copy failed. Error Code: %04d\n", result);
        return -1;
    }

    for (int i = 0; i < 3; i++) {
        fprintf(stdout, "dst[%d] = 0x%08X\n", i, dst[i]);
    }

    fprintf(stdout, "\n############################################\n\n");

    // Test Case 5: bi_set_from_array 테스트
    fprintf(stdout, "[+] Test Case 5: bi_set_from_array Test\n");

    result = bi_set_from_array(&test_bigint, POSITIVE, 3, src);
    if (result != SUCCESS_SET_FROM_ARRAY) {
        fprintf(stdout, "[X] Error: bi_set_from_array failed. Error Code: %04d\n", result);
        return -1;
    }

    fprintf(stdout, "[O] bigint set from array: \n");
    bi_print(test_bigint, 16);

    result = bi_delete(&test_bigint);  // 메모리 해제
    if (result != SUCCESS_MEMORY_DEALLOCATION) {
        fprintf(stdout, "[X] Error: Failed to deallocate memory. Error Code: %04d\n", result);
        return -1;
    }

    fprintf(stdout, "\n############################################\n\n");

    // Test Case 6: bi_set_from_string 테스트
    // fprintf(stdout, "[+] Test Case 6: bi_set_from_string Test\n");

    // char hex_str[] = "0x123456789ABCDEF";
    // result = bi_set_from_string(&test_bigint, hex_str, 16);
    // if (result != SUCCESS_SET_FROM_ARRAY) {
    //     fprintf(stdout, "[X] Error: bi_set_from_string failed. Error Code: %04d\n", result);
    //     return -1;
    // }

    // fprintf(stdout, "[O] bigint set from string: \n");
    // bi_print(test_bigint, 16);

    // result = bi_delete(&test_bigint);  // 메모리 해제
    // if (result != SUCCESS_MEMORY_DEALLOCATION) {
    //     fprintf(stdout, "[X] Error: Failed to deallocate memory. Error Code: %04d\n", result);
    //     return -1;
    // }

    // fprintf(stdout, "\n############################################\n\n");

    // Test Case 7: bi_get_random 테스트
    fprintf(stdout, "[+] Test Case 7: bi_get_random Test\n");

    result = bi_get_random(&test_bigint, POSITIVE, 4);
    if (result != SUCCESS_GEN_RAND) {
        fprintf(stdout, "[X] Error: bi_get_random failed. Error Code: %04d\n", result);
        return -1;
    }

    fprintf(stdout, "[O] Random bigint generated: \n");
    bi_print(test_bigint, 16);

    result = bi_delete(&test_bigint);  // 메모리 해제
    if (result != SUCCESS_MEMORY_DEALLOCATION) {
        fprintf(stdout, "[X] Error: Failed to deallocate memory. Error Code: %04d\n", result);
        return -1;
    }

    bigint* number = NULL;  // 반드시 NULL로 초기화
    char hex_string1[] = "0x1A2B3C4D5E6F";  // 정상적인 16진수 문자열
    char hex_string2[] = "-0xABCDEF123456"; // 음수 16진수 문자열
    char hex_string3[] = "1A2B3C";          // '0x' 없이 16진수 문자열
    char hex_string4[] = "0x";              // 빈 16진수
    char hex_string5[] = "-0x";             // 빈 음수 16진수
    char invalid_hex[] = "0x1A2G3C";        // 유효하지 않은 문자 'G' 포함
    char long_hex[] = "0x123456789ABCDEF0123456789ABCDEF";  // 긴 16진수 문자열

    // 테스트 8: 정상적인 16진수 입력
    fprintf(stdout, "[+] Test Case 8: Valid Hex String\n");
    result = bi_set_from_string(&number, hex_string1, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        fprintf(stdout, "[O] Bigint successfully created from string.\n");
        bi_print(number, 16);  // bigint 출력
    } else {
        fprintf(stdout, "[X] Failed to create bigint from string.\n");
    }
    bi_delete(&number);  // 메모리 해제

    // 테스트 9: 음수 16진수 입력
    fprintf(stdout, "\n[+] Test Case 9: Negative Hex String\n");
    result = bi_set_from_string(&number, hex_string2, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        fprintf(stdout, "[O] Bigint successfully created from negative string.\n");
        bi_print(number, 16);  // bigint 출력
    } else {
        fprintf(stdout, "[X] Failed to create bigint from negative string.\n");
    }
    bi_delete(&number);  // 메모리 해제

    // 테스트 10: '0x' 없이 16진수 입력
    fprintf(stdout, "\n[+] Test Case 10: Hex String without '0x'\n");
    result = bi_set_from_string(&number, hex_string3, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        fprintf(stdout, "[O] Bigint successfully created from string without '0x'.\n");
        bi_print(number, 16);  // bigint 출력
    } else {
        fprintf(stdout, "[X] Failed to create bigint from string without '0x'.\n");
    }
    bi_delete(&number);  // 메모리 해제

    // 테스트 11: 빈 16진수 입력
    fprintf(stdout, "\n[+] Test Case 11: Empty Hex String\n");
    result = bi_set_from_string(&number, hex_string4, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        fprintf(stdout, "[O] Bigint successfully created from empty string.\n");
        bi_print(number, 16);  // bigint 출력
    } else {
        fprintf(stdout, "[X] Failed to create bigint from empty string.\n");
    }
    bi_delete(&number);  // 메모리 해제

    // 테스트 12: 빈 음수 16진수 입력
    fprintf(stdout, "\n[+] Test Case 12: Empty Negative Hex String\n");
    result = bi_set_from_string(&number, hex_string5, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        fprintf(stdout, "[O] Bigint successfully created from empty negative string.\n");
        bi_print(number, 16);  // bigint 출력
    } else {
        fprintf(stdout, "[X] Failed to create bigint from empty negative string.\n");
    }
    bi_delete(&number);  // 메모리 해제

    // 테스트 13: 유효하지 않은 문자 포함
    fprintf(stdout, "\n[+] Test Case 13: Invalid Hex String (contains 'G')\n");
    result = bi_set_from_string(&number, invalid_hex, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        fprintf(stdout, "[O] Bigint successfully created from string with invalid character.\n");
    } else {
        fprintf(stdout, "[X] Failed to create bigint from string with invalid character.\n");
    }
    bi_delete(&number);  // 메모리 해제

    // 테스트 14: 긴 16진수 입력
    fprintf(stdout, "\n[+] Test Case 14: Long Hex String\n");
    result = bi_set_from_string(&number, long_hex, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        fprintf(stdout, "[O] Bigint successfully created from long string.\n");
        bi_print(number, 16);  // bigint 출력
    } else {
        fprintf(stdout, "[X] Failed to create bigint from long string.\n");
    }
    bi_delete(&number);  // 메모리 해제

    return 0;
}