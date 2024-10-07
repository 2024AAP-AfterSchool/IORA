/**
 * @file example.c
 * @author 조동후 : 작성자
 * @date 2024-10-07 : 최초 작성일자
 * @brief 이 파일은 Doxygen 문서화 주석의 이해를 돕기 위한 예제 파일입니다. : 간단한 파일 설명 작성.
 * @details 자세한 설명이 필요하면 작성합니다. : 자세한 파일 설명 작성(brief로 충분한 경우 작성 X)
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief example 함수는 간단한 출력을 하는 함수입니다. : 간단한 함수 설명 작성
 * @details 자세한 설명이 필요하면 작성합니다. : 자세한 파일 설명 작성(brief로 충분한 경우 작성 X)
 * 
 * @param count : 출력 횟수 : 입력값에 대한 설명 작성(없는 경우 작성 X)
 * @param bonus : 보너스 문자 : 입력값에 대한 설명 작성
 * 
 * @return 출력 횟수와 보너스 문자의 합 : 반환값에 대한 설명 작성(없는 경우 작성 X)
 */
void example(int count, char bonus) {

    // b를 int로 변경
    int c = bonus;

    printf("Hello, World!\n");

    return count + c;
}