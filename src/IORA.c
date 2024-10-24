#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base/base_type.h"
#include "base/base_error.h"
#include "bigint/bigint_calculation.h"

int main() {
    
    fprintf(stdout, "Hello, World!\n");
    
    bigint* test_bigint = NULL;  // 초기화를 NULL로 설정
    msg result;

    // 케이스 1: 배열의 마지막 원소가 0이 아닌 경우 (정제가 일어나지 않아야 함)
    fprintf(stdout, "Test Case 1: No refinement needed\n");
    result = bi_new(&test_bigint, 3);  // word_len을 3으로 설정
    if (result != SUCCESS_INITIALIZATION || test_bigint == NULL || test_bigint->start == NULL) {
        fprintf(stdout, "Error: Failed to initialize bigint.\n");
        return -1;
    }

    test_bigint->start[0] = 0x12345678;
    test_bigint->start[1] = 0x87654321;
    test_bigint->start[2] = 0x11111111;  // 마지막 원소가 0이 아닌 경우
    test_bigint->sign = POSITIVE;

    fprintf(stdout, "Before refinement: word_len = %d\n", test_bigint->wordlen);
    result = bi_refine(test_bigint);
    if (result != SUCCESS_REFINE) {
        fprintf(stdout, "Error: Refinement failed.\n");
        bi_delete(&test_bigint);
        return -1;
    }
    fprintf(stdout, "After refinement: word_len = %d\n", test_bigint->wordlen);
    bi_print(test_bigint, 16);

    bi_delete(&test_bigint);  // 메모리 해제

    // 케이스 2: 배열의 마지막 원소가 0인 경우 (정제가 발생해야 함)
    fprintf(stdout, "\nTest Case 2: Refinement needed\n");
    result = bi_new(&test_bigint, 4);  // word_len을 4로 설정
    if (result != SUCCESS_INITIALIZATION || test_bigint == NULL || test_bigint->start == NULL) {
        fprintf(stdout, "Error: Failed to initialize bigint.\n");
        return -1;
    }

    test_bigint->start[0] = 0x12345678;
    test_bigint->start[1] = 0x87654321;
    test_bigint->start[2] = 0x00000000;  // 마지막 원소가 0인 경우
    test_bigint->start[3] = 0x00000000;  // 또 다른 0
    test_bigint->sign = POSITIVE;

    fprintf(stdout, "Before refinement: word_len = %d\n", test_bigint->wordlen);
    result = bi_refine(test_bigint);
    if (result != SUCCESS_REFINE) {
        fprintf(stdout, "Error: Refinement failed.\n");
        bi_delete(&test_bigint);
        return -1;
    }
    fprintf(stdout, "After refinement: word_len = %d\n", test_bigint->wordlen);
    bi_print(test_bigint, 16);

    bi_delete(&test_bigint);  // 메모리 해제

    // 케이스 3: 배열 전체가 0인 경우 (부호가 POSITIVE로 설정되어야 함)
    fprintf(stdout, "\nTest Case 3: All elements are zero\n");
    result = bi_new(&test_bigint, 3);  // word_len을 3으로 설정
    if (result != SUCCESS_INITIALIZATION || test_bigint == NULL || test_bigint->start == NULL) {
        fprintf(stdout, "Error: Failed to initialize bigint.\n");

        return -1;
    }

    test_bigint->start[0] = 0x00000000;
    test_bigint->start[1] = 0x00000000;
    test_bigint->start[2] = 0x00000000;
    test_bigint->sign = NEGATIVE;

    fprintf(stdout, "Before refinement: word_len = %d, sign = %d\n", test_bigint->wordlen, test_bigint->sign);
    result = bi_refine(test_bigint);
    if (result != SUCCESS_REFINE) {
        fprintf(stdout, "Error: Refinement failed.\n");
        bi_delete(&test_bigint);

        return -1;
    }

    fprintf(stdout, "After refinement: word_len = %d, sign = %d\n", test_bigint->wordlen, test_bigint->sign);
    bi_print(test_bigint, 16);

    bi_delete(&test_bigint);  // 메모리 해제

    return 0;
}