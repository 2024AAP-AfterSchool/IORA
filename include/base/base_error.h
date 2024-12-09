#ifndef IORA_BASE_ERROR_H_
#define IORA_BASE_ERROR_H_

#include <stdbool.h>
#include "base_type.h"

#define VERBOSE false                       //< 에러 메시지 출력 여부 (true: 출력, false: 출력 안함)

// 에러 및 성공 코드 정의
#define SUCCESS_MEMORY_ALLOCATION 1         //< 메모리 할당 성공
#define SUCCESS_INITIALIZATION 2            //< 초기화 성공
#define SUCCESS_MEMORY_DEALLOCATION 3       //< 메모리 해제 성공
#define SUCCESS_COPY 4                      //< 배열 복사 성공
#define SUCCESS_SET_FROM_ARRAY 5            //< 배열로부터 세팅 성공
#define SUCCESS_PRINT 6                     //< 출력 성공
#define SUCCESS_RAND_ARRAY 7                //< 랜덤 배열 생성 성공
#define SUCCESS_GEN_RAND 8                  //< 랜덤 생성 성공
#define SUCCESS_ASSIGN 9                    //< 할당 성공
#define SUCCESS_REFINE 10                   //< Refine 성공
#define SUCCESS_SET_FROM_STRING 11          //< 문자열로부터 세팅 성공
#define SUCCESS_SHIFT 12                    //< SHIFT 성공
#define SUCCESS_ADD_ABC 13                  //< ADD_ABC 성공
#define SUCCESS_ADD_C 14                    //< ADD_C 성공
#define SUCCESS_ADD 15                      //< ADD 성공
#define SUCCESS_SUB_AbB 16                  //< SUB_AbB 성공
#define SUCCESS_SUB_Ab 17                   //< SUB_Ab 성공
#define SUCCESS_SUB_C 18                    //< SUB_C 성공
#define SUCCESS_SUB 19                      //< SUB 성공
#define SUCCESS_MUL_AB 20                   //< MUL_AB 성공
#define SUCCESS_MUL_C 21                    //< MUL_C 성공
#define SUCCESS_MUL 22                      //< MUL 성공
#define SUCCESS_MUL_KARATSUBA 22            //< MUL_KARATSUBA 성공
#define SUCCESS_SQUARE_AA 23                //< SQUARE_AA 성공
#define SUCCESS_SQUARE_C 24                 //< SQUARE_C 성공
#define SUCCESS_SQUARE_KARATSUBA 25         //< SQUARE_KARATSUBA 성공
#define SUCCESS_SQUARE 26                   //< SUCCESS_SQUARE 성공
#define SUCCESS_SQUARE_A 27                 //< SUCCESS_SQUARE_A 성공
#define SUCCESS_DIV_BIT 28                  //< DIV_BIT 성공
#define SUCCESS_DIV_WORD 29                 //< DIV_WORD 성공
#define SUCCESS_DIV_CC 30                   //< DIV_CC 성공
#define SUCCESS_DIV_C 31                    //< DIC_C 성공
#define SUCCESS_DIV 32                      //< DIV 성공
#define SUCCESS_LEFT_TO_RIGHT 33            //< LEFT_TO_RIGHT 성공
#define SUCCESS_RIGHT_TO_LEFT 34            //< RIGHT_TO_LEFT 성공
#define SUCCESS_MONTGOMERY 35               //< MONTGOMERY 성공
#define SUCCESS_LEFT_TO_RIGHT_MOD 36        //< LEFT_TO_RIGHT_MOD 성공
#define SUCCESS_RIGHT_TO_LEFT_MOD 37        //< RIGHT_TO_LEFT_MOD 성공
#define SUCCESS_MONTGOMERY_MOD 38           //< MONTGOMERY_MOD 성공
#define SUCCESS_MOD 39                      //< MOD 성공

#define INVALID_WORD_LENGTH_ERROR -1        //< 잘못된 워드 길이 에러
#define MEMORY_ALLOCATION_ERROR -2          //< 메모리 할당 실패 에러
#define ARRAY_ALLOCATION_ERROR -3           //< 배열 메모리 할당 실패 에러
#define NULL_POINTER_ERROR -4               //< 널 포인터 에러
#define ALREADY_FREED_ERROR -5              //< 이미 해제된 포인터 에러
#define COPY_FAILED_ERROR -6                //< 복사 실패 에러
#define INVALID_INPUT_ERROR -7	            //< 유효하지 않은 입력 에러
#define FAIL_ZERO_DIV -8                    //< 유효하지 않은 나눗셈 에러

// 함수 프로토타입 정의 (에러 메시지 출력)
msg print_invalid_word_length_error();
msg print_memory_allocation_error();
msg print_array_allocation_error();
msg print_null_pointer_error();
msg print_already_freed_error();
msg print_copy_failed_error();
msg print_invalid_input_error();
msg print_zero_div_error();

// 함수 프로토타입 정의 (성공 메시지 출력)
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
msg print_success_shift();
msg print_success_add_ABC();
msg print_success_add_C();
msg print_success_add();
msg print_success_sub_AbB();
msg print_success_sub_Ab();
msg print_success_sub_C();
msg print_success_sub();
msg print_success_mul_AB();
msg print_success_mul_C();
msg print_success_mul();
msg print_success_mul_karatsuba();
msg print_success_square_AA();
msg print_success_square_C();
msg print_success_square_KARATSUBA();
msg print_success_square();
msg print_success_div_bit();
msg print_success_div_word();
msg print_success_div_CC();
msg print_success_div_C();
msg print_success_div();
msg print_success_left_to_right();
msg print_success_right_to_left();
msg print_success_montgomery();
msg print_success_left_to_right_mod();
msg print_success_right_to_left_mod();
msg print_success_montgomery_mod();
msg print_success_mod();
#endif // IORA_BASE_ERROR_H_