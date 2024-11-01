/**
 * @file error.c
 * @�Լ� ���� �� ���� �� ���� ���� �� �̸� ����ϴ� �ҽ� ����
 * @date 2024-10-25
 * @author �ۿ���
 */
#include <stdio.h>
#include "error.h"
#include "struct.h"


 /**
  * @brief '��ȿ���� ���� �Է�'�� ����ϴ� �Լ� 
  */
msg print_invalid_word_length_error() {
    fprintf(stderr, "Error: Invalid word length.\n");
    return INVALID_WORD_LENGTH_ERROR;
}

/**
  * @brief '�޸� �Ҵ� ����'�� ����ϴ� �Լ�
  */
msg print_memory_allocation_error() {
    fprintf(stderr, "Error: Memory allocation failed.\n");
    return MEMORY_ALLOCATION_ERROR;
}

msg print_array_allocation_error()
{
    fprintf(stderr, "Error: Array allocation failed.\n");
    return ARRAY_ALLOCATION_ERROR;
}

/**
  * @brief 'bi_new'�Լ��� ������ ����ϴ� �Լ�
  */
msg print_success_memory_allocation() {
    fprintf(stdout, "Success: Memory allocation completed.\n");
    return SUCCESS_MEMORY_ALLOCATION;
}

/**
  * @brief '�޸� �ʱ�ȭ ����'�� ����ϴ� �Լ�
  */
msg print_success_initialization() {
    fprintf(stdout, "Success: Initialization completed.\n");
    return SUCCESS_INITIALIZATION;
}

/**
  * @brief 'NULL ������ ����'�� ����ϴ� �Լ�
  */
msg print_null_pointer_error() {
    fprintf(stderr, "Error: Null pointer detected.\n");
    return NULL_POINTER_ERROR;
}

/**
  * @brief 'bi_new'�Լ��� ������ ����ϴ� �Լ�
  */
msg print_already_freed_error() {
    fprintf(stderr, "Error: Memory already freed.\n");
    return ALREADY_FREED_ERROR;
}

/**
  * @brief 'bi_delete'�Լ��� ������ ����ϴ� �Լ�
  */
msg print_success_memory_deallocation() {
    fprintf(stdout, "Success: Memory deallocation completed.\n");
    return SUCCESS_MEMORY_DEALLOCATION;
}

/**
  * @brief '�迭 ���� ����'�� ����ϴ� �Լ�
  */
msg print_copy_failed_error() {
    fprintf(stderr, "Error: Array copy failed.\n");
    return COPY_FAILED_ERROR;
}

msg print_invalid_input_error()
{
    fprintf(stderr, "Error: Invalid Input Error.\n");
    return INVALID_INPUT_ERROR;
}

/**
  * @brief 'copy_array'�Լ��� ������ ����ϴ� �Լ�
  */
msg print_success_copy() {
    fprintf(stdout, "Success: Array copy completed.\n");
    return SUCCESS_COPY;
}

/**
  * @brief 'bi_set_from_array'�Լ��� ������ ����ϴ� �Լ�
  */
msg print_success_set_from_array() {
    fprintf(stdout, "Success: Set_from_array completed.\n");
    return SUCCESS_SET_FROM_ARRAY;
}

/**
  * @brief 'bi_print' �Լ��� ������ ����ϴ� �Լ�
  */
msg print_success_print() {
    fprintf(stdout, "Success: print_success.\n");
    return SUCCESS_PRINT;
}

/**
  * @brief 'rand_array'�Լ��� ������ ����ϴ� �Լ�
  */
msg print_rand_array() {
    fprintf(stdout, "Success: print_rand_array.\n");
    return SUCCESS_RAND_ARRAY;
}

/**
  * @brief ���� bigint ���� ������ ����ϴ� �Լ�
  */
msg print_success_gen_rand() {
    fprintf(stdout, "Success: print_gen_rand.\n");
    return SUCCESS_GEN_RAND;
}

/**
  * @brief 'bi_assign'�Լ���  ������ ����ϴ� �Լ�
  */
msg print_success_assign() {
    fprintf(stdout, "Success: print_success_assign.\n");
    return SUCCESS_ASSIGN;
}

/**
  * @brief 'bi_refine'�Լ��� ������ ����ϴ� �Լ�
  */
msg print_success_refine() {
    fprintf(stdout, "Success: print_success_refine.\n");
    return SUCCESS_REFINE;
}

/**
  * @brief bi_set_from_string�Լ��� ������ ����ϴ� �Լ�
  */
msg print_success_set_from_string() {
    fprintf(stdout, "Success: print_success_set_from_string.\n");
    return SUCCESS_SET_FROM_STRING;
}


