/**
 * @file string.h
 * @brief 문자열과 관련된 함수들을 정의한 헤더 파일
 * @date 2024-10-17
 * @author 조동후
 */

#ifndef IORA_UTILS_STRING_H_
#define IORA_UTILS_STRING_H_

#include "base/base_type.h"

/**
 * @brief 문자열을 숫자로 변환하는 함수
 * @param str 문자열
 * @param base 10진수, 16진수 등 어떻게 해석할지 의미함.
 */
void str_to_hex(word** dst, char* str, int base);

/**
 * @brief 문자열이 10진수인지 확인하는 함수
 * @param c 문자열
 * @return 10진수이면 1, 아니면 0
 */
uint32_t is_digit(char c);

/**
 * @brief 문자열이 16진수인지 확인하는 함수
 * @param c 문자열
 * @return 16진수이면 1, 아니면 0
 */
uint32_t is_hex_digit(char c);

#endif // IORA_UTILS_STRING_H_