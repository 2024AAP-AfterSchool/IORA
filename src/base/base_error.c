/**
 * @file base_error.c
 * @brief bigint에 대한 에러 및 성공 코드를 출력하는 함수들을 정의한 소스 파일
 * @date 2024-10-22
 * @author 송원우
 */
#include <stdio.h>
#include "base/base_type.h"
#include "base/base_error.h"

/**
 * @brief bigiint의 길이가 올바르지 않을 때 출력하는 함수
 */
msg print_invalid_word_length_error()
{   
    if (VERBOSE) fprintf(stderr, "Error: Invalid word length.\n");
    return INVALID_WORD_LENGTH_ERROR;
}

/**
 * @brief 메모리 할당 실패 시 출력하는 함수
 */
msg print_memory_allocation_error()
{   
    if (VERBOSE) fprintf(stderr, "Error: Memory allocation failed.\n");
    return MEMORY_ALLOCATION_ERROR;
}

/**
 * @brief 배열 메모리 할당 실패 시 출력하는 함수
 */
msg print_array_allocation_error()
{
    if (VERBOSE) fprintf(stderr, "Error: Array memory allocation failed.\n");
    return ARRAY_ALLOCATION_ERROR;
}

/**
 * @brief 메모리 할당 성공 시 출력하는 함수
 */
msg print_success_memory_allocation()
{
    if (VERBOSE) fprintf(stdout, "Success: Memory allocation completed.\n");
    return SUCCESS_MEMORY_ALLOCATION;
}

/**
 * @brief 초기화 성공 시 출력하는 함수
 */
msg print_success_initialization()
{
    if (VERBOSE) fprintf(stdout, "Success: Initialization completed.\n");
    return SUCCESS_INITIALIZATION;
}

/**
 * @brief Null 포인터가 발생했을 때 출력하는 함수
 */
msg print_null_pointer_error()
{
    if (VERBOSE) fprintf(stderr, "Error: Null pointer detected.\n");
    return NULL_POINTER_ERROR;
}

/**
 * @brief 이미 해제된 포인터가 발생했을 때 출력하는 함수
 */
msg print_already_freed_error()
{
    if (VERBOSE) fprintf(stderr, "Error: Memory already freed.\n");
    return ALREADY_FREED_ERROR;
}

/**
 * @brief 메모리 해제 성공 시 출력하는 함수
 */
msg print_success_memory_deallocation() 
{
    if (VERBOSE) fprintf(stdout, "Success: Memory deallocation completed.\n");
    return SUCCESS_MEMORY_DEALLOCATION;
}

/**
 * @brief 배열 복사 실패 시 출력하는 함수
 */
msg print_copy_failed_error()
{
    if (VERBOSE) fprintf(stderr, "Error: Array copy failed.\n");
    return COPY_FAILED_ERROR;
}

/**
 * @brief 배열 복사 성공 시 출력하는 함수
 */
msg print_success_copy()
{
    if (VERBOSE) fprintf(stdout, "Success: Array copy completed.\n");
    return SUCCESS_COPY;
}

/**
 * @brief 배열로부터 bigint에 값을 할당 성공 시 출력하는 함수
 */
msg print_success_set_from_array()
{
    if (VERBOSE) fprintf(stdout, "Success: Set_from_array completed.\n");
    return SUCCESS_SET_FROM_ARRAY;
}

/**
 * @brief bigint를 출력 성공 시 출력하는 함수
 */
msg print_success_print()
{
    if (VERBOSE) fprintf(stdout, "Success: print_success.\n");
    return SUCCESS_PRINT;
}

/**
 * @brief 랜덤 값을 통해 bigint에 값을 할당 성공 시 출력하는 함수
 */
msg print_rand_array()
{
    if (VERBOSE) fprintf(stdout, "Success: print_rand_array.\n");
    return SUCCESS_RAND_ARRAY;
}

/**
 * @brief 랜덤 값 생성 성공 시 출력하는 함수
 */
msg print_success_gen_rand()
{
    if (VERBOSE) fprintf(stdout, "Success: print_gen_rand.\n");
    return SUCCESS_GEN_RAND;
}

/**
 * @brief bigint에 값을 할당 성공 시 출력하는 함수
 */
msg print_success_assign()
{
    if (VERBOSE) fprintf(stdout, "Success: print_success_assign.\n");
    return SUCCESS_ASSIGN;
}

/**
 * @brief refine 성공 시 출력하는 함수
 */
msg print_success_refine()
{
    if (VERBOSE) fprintf(stdout, "Success: print_success_refine.\n");
    return SUCCESS_REFINE;
}

/**
 * @brief string을 통해 bigint에 값을 할당 성공 시 출력하는 함수
 */
msg print_success_set_from_string()
{
    if (VERBOSE) fprintf(stdout, "Success: print_success_set_from_string.\n");
    return SUCCESS_SET_FROM_STRING;
}

/**
 * @brief 유효하지 않은 입력 에러 시 출력하는 함수
 */
msg print_invalid_input_error()
{
    if (VERBOSE) fprintf(stderr, "Error: Invalid input detected.\n");
    return INVALID_INPUT_ERROR;
}

/**
 * @brief ADD_ABC 성공 시 출력하는 함수
 */
msg print_success_add_ABC()
{
    if (VERBOSE) fprintf(stdout, "Success: print_add_ABC.\n");
    return SUCCESS_ADD_ABC;
}

/**
 * @brief ADD_C 성공 시 출력하는 함수
 */
msg print_success_add_C()
{
    if (VERBOSE) fprintf(stdout, "Success: print_add_C.\n");
    return SUCCESS_ADD_C;
}

/**
 * @brief ADD 성공 시 출력하는 함수
 */
msg print_success_add()
{
    if (VERBOSE) fprintf(stdout, "Success: print_add.\n");
    return SUCCESS_ADD;
}

/**
 * @brief SUB_AbB 성공 시 출력하는 함수
 */
msg print_success_sub_AbB()
{
    if (VERBOSE) fprintf(stdout, "Success: print_sub_AbB.\n");
    return SUCCESS_SUB_AbB;
}

/**
 * @brief SUB_Ab 성공 시 출력하는 함수
 */
msg print_success_sub_Ab()
{
    if (VERBOSE) fprintf(stdout, "Success: print_sub_Ab.\n");
    return SUCCESS_SUB_Ab;
}

/**
 * @brief SUB_C 성공 시 출력하는 함수
 */
msg print_success_sub_C()
{
    if (VERBOSE) fprintf(stdout, "Success: print_sub_C.\n");
    return SUCCESS_SUB_C;
}

/**
 * @brief SUB 성공 시 출력하는 함수
 */
msg print_success_sub()
{
    if (VERBOSE) fprintf(stdout, "Success: print_sub.\n");
    return SUCCESS_SUB;
}

/**
 * @brief MUL_AB 성공 시 출력하는 함수
 */
msg print_success_mul_AB()
{
    if (VERBOSE) fprintf(stdout, "Success: print_mul_AB.\n");
    return SUCCESS_MUL_AB;
}

/**
 * @brief MUL_C 성공 시 출력하는 함수
 */
msg print_success_mul_C()
{
    if (VERBOSE) fprintf(stdout, "Success: print_mul_C.\n");
    return SUCCESS_MUL_C;
}


/**
 * @brief MUL 성공 시 출력하는 함수
 */
msg print_success_mul()
{
    if (VERBOSE) fprintf(stdout, "Success: print_mul.\n");
    return SUCCESS_MUL;
}

/**
 * @brief MUL_KARATSUBA 성공 시 출력하는 함수
 */
msg print_success_mul_karatsuba()
{
    if (VERBOSE) fprintf(stdout, "Success: print_mul_karatsuba.\n");
    return SUCCESS_MUL_KARATSUBA;
}
/**
 * @brief SHIFT 성공 시 출력하는 함수
 */
msg print_success_shift()
{
    if (VERBOSE) fprintf(stdout, "Success: print_shift.\n");
    return SUCCESS_SHIFT;
}

/**
 * @brief square_AA 성공 시 출력하는 함수
 */
msg print_success_square_AA()
{
    if (VERBOSE) fprintf(stdout, "Success: print_SQUARE_AA.\n");
    return SUCCESS_SQUARE_AA;
}

/**
 * @brief square_C 성공 시 출력하는 함수
 */
msg print_success_square_C()
{
    if (VERBOSE) fprintf(stdout, "Success: print_SQUARE_C.\n");
    return SUCCESS_SQUARE_C;
}

/**
 * @brief square_KARATSUBA 성공 시 출력하는 함수
 */
msg print_success_square_KARATSUBA()
{
    if (VERBOSE) fprintf(stdout, "Success: print_SQUARE_KARATSUBA.\n");
    return SUCCESS_SQUARE_KARATSUBA;
}

/**
 * @brief square 성공 시 출력하는 함수
 */
msg print_success_square()
{
    if (VERBOSE) fprintf(stdout, "Success: print_SQUARE.\n");
    return SUCCESS_SQUARE;
}

/**
 * @brief div_bit 성공 시 출력하는 함수
 */
msg print_success_div_bit()
{
    if (VERBOSE) fprintf(stdout, "Success: print_div_bit.\n");
    return SUCCESS_DIV_BIT;
}

/**
 * @brief div_word 성공 시 출력하는 함수
 */
msg print_success_div_word()
{
    if (VERBOSE) fprintf(stdout, "Success: print_div_word.\n");
    return SUCCESS_DIV_WORD;
}

/**
 * @brief div_cc 성공 시 출력하는 함수
 */
msg print_success_div_CC()
{
    if (VERBOSE) fprintf(stdout, "Success: print_div_CC.\n");
    return SUCCESS_DIV_CC;
}

/**
 * @brief div_c 성공 시 출력하는 함수
 */
msg print_success_div_C()
{
    if (VERBOSE) fprintf(stdout, "Success: print_div_C.\n");
    return SUCCESS_DIV_C;
}

/**
 * @brief DIV 성공 시 출력하는 함수
 */
msg print_success_div()
{
    if (VERBOSE) fprintf(stdout, "Success: print_div.\n");
    return SUCCESS_DIV;
}

/**
 * @brief DIV_BIT 실패 시 출력하는 함수(0으로 나누었을 때)
 */
msg print_zero_div_error()
{
    if (VERBOSE) fprintf(stderr, "Error: print_zero_div.\n");
    return FAIL_ZERO_DIV;
}

/**
 * @brief left_to_right 성공 시 출력하는 함수
 */
msg print_success_left_to_right()
{
    if (VERBOSE) fprintf(stdout, "Success: print_left_to_right.\n");
    return SUCCESS_LEFT_TO_RIGHT;
}

/**
 * @brief right_to_left 성공 시 출력하는 함수
 */
msg print_success_right_to_left()
{
    if (VERBOSE) fprintf(stdout, "Success: print_right_to_left.\n");
    return SUCCESS_RIGHT_TO_LEFT;
}

/**
 * @brief montgomery 성공 시 출력하는 함수
 */
msg print_success_montgomery()
{
    if (VERBOSE) fprintf(stdout, "Success: print_montgomery.\n");
    return SUCCESS_MONTGOMERY;
}


/**
 * @brief left_to_right_mod 성공 시 출력하는 함수
 */
msg print_success_left_to_right_mod()
{
    if (VERBOSE) fprintf(stdout, "Success: print_left_to_right_mod.\n");
    return SUCCESS_LEFT_TO_RIGHT_MOD;
}

/**
 * @brief right_to_left_mod 성공 시 출력하는 함수
 */
msg print_success_right_to_left_mod()
{
    if (VERBOSE) fprintf(stdout, "Success: print_right_to_left_mod.\n");
    return SUCCESS_RIGHT_TO_LEFT_MOD;
}

/**
 * @brief montgomery_mod 성공 시 출력하는 함수
 */
msg print_success_montgomery_mod()
{
    if (VERBOSE) fprintf(stdout, "Success: print_montgomery_mod.\n");
    return SUCCESS_MONTGOMERY_MOD;
}

/**
 * @brief mod 성공 시 출력하는 함수
 */
msg print_success_mod()
{
    if (VERBOSE) fprintf(stdout, "Success: print_mod.\n");
    return SUCCESS_MOD;
}

