#include <stdio.h>
#include <stdlib.h>
#include "bigint/bigint_calculation.h"

int main() {
    
    fprintf(stdout, "Hello, World!\n");
    
    bigint* num = NULL;

    printf("############################################\n");

    // 1. bi_new 함수 테스트
    bi_new(&num, 5);
    if (num && num->wordlen == 5) {
        printf("bi_new test passed: sign = %u\n", num->sign);
        printf("bi_new test passed: word length = %u\n", num->wordlen);
        printf("bi_new test passed: first word = %u\n", num->start[0]);
    } else {
        printf("bi_new test failed\n");
    }

    bi_print(num, 16);

    printf("############################################\n");

    // 2. bi_set_from_array 함수 테스트
    word arr[5] = {13, 8, 65535 * 65535, 160, 255};
    bi_set_from_array(&num, NEGATIVE, 5, arr);
    if (num && num->start[0] == 13 && num->wordlen == 5) {
        printf("bi_set_from_array test passed: sign = %u\n", num->sign);
        printf("bi_set_from_array test passed: word length = %u\n", num->wordlen);
        printf("bi_set_from_array test passed: first word = %u\n", num->start[0]);
    } else {
        printf("bi_set_from_array test failed\n");
    }

    bi_print(num, 16);

    printf("############################################\n");

    // 3. bi_set_from_string 함수 테스트 (수정 필요)
    char str[] = "00443300FF012345";
    bi_set_from_string(&num, str, 16);
    if (num && num->sign == POSITIVE) {
        printf("bi_set_from_string test passed: sign = %u\n", num->sign);
        printf("bi_set_from_string test passed: word length = %u\n", num->wordlen);
        printf("bi_set_from_string test passed: first word = %u\n", num->start[0]);
    } else {
        printf("bi_set_from_string test failed\n");
    }

    bi_print(num, 16);

    printf("############################################\n");

    // 4. bi_get_random 함수 테스트
    bi_get_random(&num, POSITIVE, 5);
    if (num && num->wordlen == 5) {
        printf("bi_get_random test passed: sign = %u\n", num->sign);
        printf("bi_get_random test passed: word length = %u\n", num->wordlen);
        printf("bi_get_random test passed: first word = %u\n", num->start[0]);
    } else {
        printf("bi_get_random test failed\n");
    }

    printf("############################################\n");

    // 5. bi_refine 함수 테스트
    bi_refine(num);
    if (num && num->wordlen <= 5) {
        printf("bi_refine test passed\n");
    } else {
        printf("bi_refine test failed\n");
    }

    printf("############################################\n");

    // 6. bi_assign 함수 테스트
    bigint* num2 = NULL;
    bi_assign(&num2, num);
    if (num2 && num2->wordlen == num->wordlen) {
        printf("bi_assign test passed\n");
    } else {
        printf("bi_assign test failed\n");
    }

    printf("############################################\n");

    // 7. bi_print 함수 테스트
    printf("bi_print test: ");
    bi_print(num, 10);
    printf("\n");

    printf("############################################\n");

    // Cleanup
    bi_delete(&num);
    bi_delete(&num2);
    printf("bi_delete test passed\n");

    return 0;
}