#include <stdio.h>
#include "base/base_type.h"
#include "base/base_error.h"

msg print_invalid_word_length_error()
{   
    if (VERBOSE) fprintf(stderr, "Error: Invalid word length.\n");
    return INVALID_WORD_LENGTH_ERROR;
}

msg print_memory_allocation_error()
{   
    if (VERBOSE) fprintf(stderr, "Error: Memory allocation failed.\n");
    return MEMORY_ALLOCATION_ERROR;
}

msg print_array_allocation_error()
{
    if (VERBOSE) fprintf(stderr, "Error: Array memory allocation failed.\n");
    return ARRAY_ALLOCATION_ERROR;
}

msg print_success_memory_allocation()
{
    if (VERBOSE) fprintf(stdout, "Success: Memory allocation completed.\n");
    return SUCCESS_MEMORY_ALLOCATION;
}

msg print_success_initialization()
{
    if (VERBOSE) fprintf(stdout, "Success: Initialization completed.\n");
    return SUCCESS_INITIALIZATION;
}

msg print_null_pointer_error()
{
    if (VERBOSE) fprintf(stderr, "Error: Null pointer detected.\n");
    return NULL_POINTER_ERROR;
}

msg print_already_freed_error()
{
    if (VERBOSE) fprintf(stderr, "Error: Memory already freed.\n");
    return ALREADY_FREED_ERROR;
}
msg print_success_memory_deallocation() 
{
    if (VERBOSE) fprintf(stdout, "Success: Memory deallocation completed.\n");
    return SUCCESS_MEMORY_DEALLOCATION;
}

msg print_copy_failed_error()
{
    if (VERBOSE) fprintf(stderr, "Error: Array copy failed.\n");
    return COPY_FAILED_ERROR;
}

msg print_success_copy()
{
    if (VERBOSE) fprintf(stdout, "Success: Array copy completed.\n");
    return SUCCESS_COPY;
}

msg print_success_refinement()
{
    if (VERBOSE) fprintf(stdout, "Success: Refinement completed.\n");
    return SUCCESS_REFINEMENT;
}

msg print_success_set_from_array()
{
    if (VERBOSE) fprintf(stdout, "Success: Set_from_array completed.\n");
    return SUCCESS_SET_FROM_ARRAY;
}

msg print_success_print()
{
    if (VERBOSE) fprintf(stdout, "Success: print_success.\n");
    return SUCCESS_PRINT;
}

msg print_rand_array()
{
    if (VERBOSE) fprintf(stdout, "Success: print_rand_array.\n");
    return SUCCESS_RAND_ARRAY;
}

msg print_success_gen_rand()
{
    if (VERBOSE) fprintf(stdout, "Success: print_gen_rand.\n");
    return SUCCESS_GEN_RAND;
}

msg print_success_assign()
{
    if (VERBOSE) fprintf(stdout, "Success: print_success_assign.\n");
    return SUCCESS_ASSIGN;
}

msg print_success_refine()
{
    if (VERBOSE) fprintf(stdout, "Success: print_success_refine.\n");
    return SUCCESS_REFINE;
}

msg print_success_set_from_string()
{
    if (VERBOSE) fprintf(stdout, "Success: print_success_set_from_string.\n");
    return SUCCESS_SET_FROM_STRING;
}

msg print_invalid_input_error()
{
    if (VERBOSE) fprintf(stderr, "Error: Invalid input detected.\n");
    return INVALID_INPUT_ERROR;
}