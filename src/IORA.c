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

    #ifdef TEST_ADDITION
    bi_test_add();
    #endif

    #ifdef TEST_SUBTRACTION
    bi_test_sub();
    #endif

    #ifdef TEST_MULTIPLICATION
    bi_test_mul();
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