#ifndef IORA_BASE_ERROR_H_
#define IORA_BASE_ERROR_H_

#include "base_type.h"

// 에러 및 성공 코드 정의
#define SUCCESS_MEMORY_ALLOCATION 1         // 메모리 할당 성공
#define SUCCESS_INITIALIZATION 2            // 초기화 성공
#define SUCCESS_MEMORY_DEALLOCATION 3       // 메모리 해제 성공
#define SUCCESS_COPY 4                      // 배열 복사 성공
#define SUCCESS_SET_FROM_ARRAY 5
#define SUCCESS_PRINT 6
#define SUCCESS_RAND_ARRAY 7
#define SUCCESS_GEN_RAND 8
#define SUCCESS_ASSIGN 9
#define SUCCESS_REFINE 10
#define SUCCESS_SET_FROM_STRING 11
#define SUCCESS_REFINEMENT 12
#define INVALID_WORD_LENGTH_ERROR -1        // 잘못된 워드 길이 에러
#define MEMORY_ALLOCATION_ERROR -2          // 메모리 할당 실패 에러
#define ARRAY_ALLOCATION_ERROR -3           // 배열 메모리 할당 실패 에러
#define NULL_POINTER_ERROR -4               // 널 포인터 에러
#define ALREADY_FREED_ERROR -5              // 이미 해제된 포인터 에러
#define COPY_FAILED_ERROR -6                // 복사 실패 에러
#define INVALID_INPUT_ERROR -7	            // 유효하지 않은 입력 에러

// 함수 프로토타입 정의 (에러 메시지 출력)
msg print_invalid_word_length_error();
msg print_memory_allocation_error();
msg print_array_allocation_error();
msg print_null_pointer_error();
msg print_already_freed_error();
msg print_copy_failed_error();
msg print_invalid_input_error();

// 함수 프로토타입 정의 (성공 메시지 출력)
msg print_success_memory_allocation();
msg print_success_initialization();
msg print_success_memory_deallocation();
msg print_success_copy();
msg print_success_refinement();
msg print_success_set_from_array();
msg print_success_print();
msg print_rand_array();
msg print_success_gen_rand();
msg print_success_assign();
msg print_success_refine();
msg print_success_set_from_string();

#endif // IORA_BASE_ERROR_H_