#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base/base_type.h"
#include "base/base_error.h"
#include "bigint/bigint_addition.h"
#include "bigint/bigint_calculation.h"
#include "bigint/bigint_subtraction.h"
#include "bigint/bigint_multiplication.h"

#define TEST_ADDITION
#define TEST_SUBTRACTION
#define TEST_MULTIPLICATION
// #define TEST_SQUARE
// #define TEST_DIVISION
// #define TEST_MODULAR

int main(int argc, char* argv[]) {

  bigint* A = NULL;
    bigint* B = NULL;
    bigint* result = NULL;
    res add_result;

    // Test Case 1: Same length addition (positive numbers)
    printf("==== Test Case 1: Same Length ====\n");
    word a_array1[] = { 0x12345678, 0x9ABCDEF0 };
    word b_array1[] = { 0x11111111, 0x22222222 };
    bi_set_from_array(&A, POSITIVE, 2, a_array1);
    bi_set_from_array(&B, POSITIVE, 2, b_array1);
    add_result = bi_add(&result, A, B);
    if (add_result.message == SUCCESS_ADD) {
        printf("A = ");
        bi_print(A, 16);
        printf("B = ");
        bi_print(B, 16);
        printf("A + B = ");
        bi_print(result, 16);
    }
    else {
        printf("Error in addition for Test Case 1.\n");
    }
    bi_delete(&A);
    bi_delete(&B);
    bi_delete(&result);

    // Test Case 2: Different length addition (positive numbers)
    printf("==== Test Case 2: Different Lengths ====\n");
    word a_array2[] = { 0xFFFFFFFF, 0xFFFFFFFF, 0x00000001 };
    word b_array2[] = { 0x00000001 };
    bi_set_from_array(&A, POSITIVE, 3, a_array2);
    bi_set_from_array(&B, POSITIVE, 1, b_array2);
    add_result = bi_add(&result, A, B);
    if (add_result.message == SUCCESS_ADD) {
        printf("A = ");
        bi_print(A, 16);
        printf("B = ");
        bi_print(B, 16);
        printf("A + B = ");
        bi_print(result, 16);
    }
    else {
        printf("Error in addition for Test Case 2.\n");
    }
    bi_delete(&A);
    bi_delete(&B);
    bi_delete(&result);

    // Test Case 3: Negative numbers addition
    printf("==== Test Case 3: Negative Numbers ====\n");
    word a_array3[] = { 0x12345678, 0x9ABCDEF0 };
    word b_array3[] = { 0x11111111, 0x22222222 };
    bi_set_from_array(&A, NEGATIVE, 2, a_array3);  // A is negative
    bi_set_from_array(&B, NEGATIVE, 2, b_array3);  // B is negative
    add_result = bi_add(&result, A, B);
    if (add_result.message == SUCCESS_ADD) {
        printf("A = ");
        bi_print(A, 16);
        printf("B = ");
        bi_print(B, 16);
        printf("A + B = ");
        bi_print(result, 16);
    }
    else {
        printf("Error in addition for Test Case 3.\n");
    }
    bi_delete(&A);
    bi_delete(&B);
    bi_delete(&result);

    // Test Case 4: Mixed sign addition (positive + negative)
    printf("==== Test Case 4: Mixed Signs (Positive + Negative) ====\n");
    word a_array4[] = { 0x1, 0x0 };
    word b_array4[] = { 0x0, 0x0 };
    bi_set_from_array(&A, POSITIVE, 2, a_array4);  // A is positive
    bi_set_from_array(&B, NEGATIVE, 2, b_array4);      // B is negative
    add_result = bi_add(&result, A, B);
    if (add_result.message == SUCCESS_ADD) {
        printf("A = ");
        bi_print(A, 16);
        printf("B = ");
        bi_print(B, 16);
        printf("A + B = ");
        bi_print(result, 16);
    }
    else {
        printf("Error in addition for Test Case 4.\n");
    }
    bi_delete(&A);
    bi_delete(&B);
    bi_delete(&result);

    // Test Case 5: Mixed sign addition (negative + positive)
    printf("==== Test Case 5: Mixed Signs (Negative + Positive) ====\n");
    word a_array5[] = { 0x0, 0x0 };
    word b_array5[] = { 0x1, 0x0 };
    bi_set_from_array(&A, POSITIVE, 2, a_array5);  // A is negative
    bi_set_from_array(&B, POSITIVE, 2, b_array5);  // B is positive
    bi_sub(&result, B, A);
   
    printf("A = ");
    bi_print(A, 16);
    printf("B = ");
    bi_print(B, 16);
    printf("A - B = ");
    bi_print(result, 16);
    
    
    bi_delete(&A);
    bi_delete(&B);
    bi_delete(&result);

    // Test Case 6: Large positive + large negative (to test cancellation to zero or small values)
    printf("==== Test Case 6: Large Positive + Large Negative ====\n");
    word a_array6[] = { 0xFFFFFFFF, 0xFFFFFFFF };
    word b_array6[] = { 0xFFFFFFFF, 0xFFFFFFFF };
    bi_set_from_array(&A, POSITIVE, 2, a_array6);  // A is large positive
    bi_set_from_array(&B, NEGATIVE, 2, b_array6);      // B is large negative
    add_result = bi_add(&result, A, B);
    if (add_result.message == SUCCESS_ADD) {
        printf("A = ");
        bi_print(A, 16);
        printf("B = ");
        bi_print(B, 16);
        printf("A + B = ");
        bi_print(result, 16);
    }
    else {
        printf("Error in addition for Test Case 6.\n");
    }
    bi_delete(&A);
    bi_delete(&B);
    bi_delete(&result);

    #ifdef TEST_ADDITION
    // bi_test_add();
    #endif

    #ifdef TEST_SUBTRACTION
    // bi_test_sub();
    #endif

    #ifdef TEST_MULTIPLICATION
    // bi_test_mul();
    #endif

    #ifdef TEST_SQUARE
    bi_test_square();
    #endif

    #ifdef TEST_DIVISION
    bi_test_div();
    #endif

    #ifdef TEST_MODULAR
    bi_test_mod();
    #endif

    return 0;
}