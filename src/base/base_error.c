/**
 * @file bigint_error.c
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
 * @brief Refine 성공 시 출력하는 함수
 */
msg print_success_refinement()
{
    if (VERBOSE) fprintf(stdout, "Success: Refinement completed.\n");
    return SUCCESS_REFINEMENT;
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
 * @brief MUL_AB 실패 시 출력하는 함수
 */
msg print_fail_mul_AB()
{
    if (VERBOSE) fprintf(stderr, "Error: print_fail_mul_AB.\n");
    return FAIL_MUL_AB;
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
 * @brief SHIFT 성공 시 출력하는 함수
 */
msg print_success_shift()
{
    if (VERBOSE) fprintf(stdout, "Success: print_shift.\n");
    return SUCCESS_SHIFT;
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
 * @brief DIV_BIT 실패 시 출력하는 함수
 */
msg print_fail_div_bit()
{
    if (VERBOSE) fprintf(stderr, "Error: print_fail_div_bit.\n");
    return FAIL_DIV_BIT;
}