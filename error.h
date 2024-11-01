/**
 * @file error.h
 * @�Լ� ���� �� ���� �� ���� ���� �� �̸� ����ϴ� �ҽ� ����
 * @date 2024-10-25
 * @author �賲��
 */
#ifndef ERRORS_H
#define ERRORS_H
#include "struct.h"

// ���� �� ���� �ڵ� ����
#define SUCCESS_MEMORY_ALLOCATION 1         ///< �޸� �Ҵ� ����
#define SUCCESS_INITIALIZATION 2            // �ʱ�ȭ ����
#define SUCCESS_MEMORY_DEALLOCATION 3       // �޸� ���� ����
#define SUCCESS_COPY 4                      // �迭 ���� ����
#define SUCCESS_REFINEMENT 5                // ���� �۾� ����
#define SUCCESS_SET_FROM_ARRAY 6			// �迭�κ��� �� ���� ����
#define SUCCESS_PRINT 7						// ��� ����
#define SUCCESS_RAND_ARRAY 8				// ���� �迭 ���� ����
#define SUCCESS_GEN_RAND 9					// ���� ���� ����
#define SUCCESS_ASSIGN 10					// �� �Ҵ� ����
#define SUCCESS_REFINE 11					// �� REFINE ����
#define SUCCESS_SET_FROM_STRING 12			// ���ڿ��κ��� �� ���� ����
#define INVALID_WORD_LENGTH_ERROR -1        // �߸��� ���� ���� ����
#define MEMORY_ALLOCATION_ERROR -2          // �޸� �Ҵ� ���� ����
#define ARRAY_ALLOCATION_ERROR -3           // �迭 �޸� �Ҵ� ���� ����
#define NULL_POINTER_ERROR -4               // �� ������ ����
#define ALREADY_FREED_ERROR -5              // �̹� ������ ������ ����
#define COPY_FAILED_ERROR -6                // ���� ���� ����
#define INVALID_INPUT_ERROR -7			    // �Է� ���� ����	

// �Լ� ������Ÿ�� ���� (���� �޽��� ���)
msg print_invalid_word_length_error();
msg print_memory_allocation_error();
msg print_array_allocation_error();
msg print_null_pointer_error();
msg print_already_freed_error();
msg print_copy_failed_error();
msg print_invalid_input_error();

// �Լ� ������Ÿ�� ���� (���� �޽��� ���)
msg print_success_memory_allocation();
msg print_success_initialization();
msg print_success_memory_deallocation();
msg print_success_copy();
msg print_success_set_from_array();
msg print_success_print();
msg print_rand_array();
msg print_success_gen_rand();
msg print_success_assign();
msg print_success_refine();
msg print_success_set_from_string();

#endif // ERRORS_H