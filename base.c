#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"
#include "error.h"
#include "base.h"
# define NON_NEGATIVE 0 

msg bi_delete(bigint** dst) {
    // dst가 NULL인 경우 예외 처리
    if (dst==NULL || *dst == NULL) {
        return print_null_pointer_error();
    }

    // 배열 메모리 해제
    if ((*dst)->a != NULL) {
        free((*dst)->a);
        (*dst)->a = NULL;  // 해제 후 NULL로 초기화
    }

    // 구조체 메모리 해제
    free(*dst);
    *dst = NULL;

    // 메모리 해제 성공 메시지 출력
    return print_success_memory_deallocation();
}

msg bi_new(bigint** dst, int word_len) {
    // 유효하지 않은 단어 길이 체크
    if (word_len <= 0) {
        return print_invalid_word_length_error();
    }

    // 메모리가 이미 할당된 경우 처리
    if (*dst != NULL) {
        //bi_delete(dst);
        return print_already_freed_error();
    }

    // 메모리 할당
    *dst = (bigint*)malloc(sizeof(bigint));
    if (*dst == NULL) {
        return print_memory_allocation_error();
    }

    // 초기화
    (*dst)->sign = NON_NEGATIVE;
    (*dst)->word_len = word_len;

    // 배열 할당
    (*dst)->a = (word*)calloc(word_len, sizeof(word));
    if ((*dst)->a == NULL) {
        free(*dst);  // 구조체 메모리 할당 해제
        *dst = NULL;
        return print_array_allocation_error();
    }

    // 성공적으로 초기화 완료 메시지 출력
    return print_success_initialization();
}



msg copy_array(word* dst, word* src, int word_len) {
    // 유효하지 않은 길이 체크
    if (word_len <= 0) {
        return print_invalid_word_length_error();
    }

    // dst 또는 src가 NULL인지 확인 
    if (dst == NULL || src == NULL) {
        return print_null_pointer_error();
    }

    // 배열 복사 시도 error_t는  memcpy_s, fopen_s와 같은 "안전한" 함수들에서 반환되는 값
    memcpy(dst, src, sizeof(word) * word_len);
    

    // 성공적으로 배열 복사 완료
    return print_success_copy();
}

#define OUT
#define IN

msg bi_set_from_array(bigint** dst, int sign, int word_len, word* b) {
    // 유효하지 않은 단어 길이 체크
    if (word_len <= 0) {
        return print_invalid_word_length_error();
    }

    // b가 NULL인 경우 예외 처리
    if (b == NULL) {
        return print_null_pointer_error();
    }

    // 새로운 bigint 구조체를 생성
    msg result = bi_new(dst, word_len);
    if (result != SUCCESS_INITIALIZATION) {
        return result;  // bi_new에서 발생한 에러 반환
    }

    // 부호 설정
    (*dst)->sign = sign;

    // 배열 복사 시도
    result = copy_array((*dst)->a, b, word_len);
    if (result != SUCCESS_COPY) {
        bi_delete(dst);  // 복사 실패 시 메모리 해제
        return result;   // copy_array에서 발생한 에러 반환
    }

    // 성공적으로 설정 완료
    return print_success_set_from_array();
}


msg bi_print(bigint* dst, int base)
{
    // 입력이 NULL인 경우 예외 처리
    if (dst == NULL) {
        return print_null_pointer_error();
    }

    // 유효하지 않은 base 값 예외 처리 (현재는 16진수만 지원)
    if (base != 16) {
        printf("Base %d not supported yet.\n", base);
        return 0;
    }

    /*
    // Bigint가 0인 경우 바로 출력
    if (dst->word_len == 0 || (dst->word_len == 1 && dst->a[0] == 0)) {
        printf("0x0\n");
    }
    */
    // 부호 출력
    if (dst->sign < 0) {
        printf("-");
    }

    printf("0x");

    // 모든 비트 출력 (앞쪽 0을 스킵하지 않고 출력)
    for (int i = dst->word_len - 1; i >= 0; i--) {
        printf("%08x", dst->a[i]);
    }

    // 마지막 개행
    printf("\n");

    return print_success_print();
}

msg rand_array(word* dst, int word_len) {
    // 유효하지 않은 길이 체크
    if (word_len <= 0) {
        return print_invalid_word_length_error();
    }

    // dst가 NULL인 경우 예외 처리
    if (dst == NULL) {
        return print_null_pointer_error();
    }

    // 난수로 배열을 채움
    unsigned char* c = (unsigned char*)dst;  // 한 바이트씩 읽기 위한 타입 캐스팅
    int cnt_i = word_len * sizeof(word);     // 총 바이트 수 계산

    while (cnt_i > 0) {
        *c = rand() & 0xff;  // 0~255 범위의 난수를 한 바이트에 저장
        c++;
        cnt_i--;
    }

    // 성공적으로 배열 생성 완료
    return print_rand_array();
}

msg bi_gen_random(bigint** dst, int sign, int word_len) {
    // 유효하지 않은 단어 길이 처리
    if (word_len <= 0) {
        return print_invalid_word_length_error();
    }

    // 새로운 bigint 생성
    bi_new(dst, word_len);
    // 부호 설정
    (*dst)->sign = sign;
    rand_array((*dst)->a,word_len);
    // 성공적으로 랜덤 bigint 생성 완료
    return print_success_gen_rand();
}

msg bi_assign(bigint** dst, bigint* src) {

    // 이미 할당된 메모리 해제
    if (*dst != NULL) {
        msg result = bi_delete(dst);
        if (result != SUCCESS_MEMORY_DEALLOCATION) {
            return result;  // 메모리 해제 실패 시 에러 반환
        }
    }

    // 새로운 bigint 생성
    msg result = bi_new(dst, src->word_len);
    if (result != SUCCESS_INITIALIZATION) {
        return result;  // bigint 생성 실패 시 에러 반환
    }

    // 부호 설정
    (*dst)->sign = src->sign;

    // 배열 복사
    result = copy_array((*dst)->a, src->a, src->word_len);
    if (result != SUCCESS_COPY) {
        bi_delete(dst);  // 복사 실패 시 할당된 메모리 해제
        return result;   // 배열 복사 실패 시 에러 반환
    }

    // 성공적으로 할당 완료
    return print_success_assign();
}

msg bi_refine(bigint* x) {
    // 입력이 NULL인 경우 처리
    if (x == NULL) {
        return print_null_pointer_error();
    }

    int new_wordlen = x->word_len;

    // 불필요한 0 제거 (word_len 정제)
    while (new_wordlen > 1) {
        if (x->a[new_wordlen - 1] != 0) {
            break;
        }
        new_wordlen--;
    }

    // 값이 0인 경우 부호를 NON_NEGATIVE로 설정
    if ((new_wordlen == 1) && (x->a[0] == 0x00)) {
        x->sign = NON_NEGATIVE;
    }

    // 필요하다면 메모리 재할당
    if (x->word_len != new_wordlen) {
        word* tmp = (word*)realloc(x->a, sizeof(word) * new_wordlen);
        if (tmp == NULL) {
            return print_memory_allocation_error();  // 메모리 재할당 실패 시 에러 반환
        }

        // 재할당 성공 시 메모리와 길이를 업데이트
        x->a = tmp;
        x->word_len = new_wordlen;
    }

    // 성공적으로 정제 완료
    return print_success_refine();
}


msg bi_set_from_string(bigint** dst, char* int_str, int base) {
    // 예외 처리: NULL 포인터 또는 잘못된 진수 값 체크
    if (dst == NULL || int_str == NULL || base != 16) {
        return print_null_pointer_error();
    }
    // 메모리가 이미 할당된 경우, 중복 할당 방지
    if (*dst == NULL) {
        msg result = bi_new(dst, 1);  // 최소 크기로 먼저 할당
        if (result != SUCCESS_INITIALIZATION) {
            return result;
        }
    }
    // 부호 확인
    // 부호 확인
    int start_idx = 0;
    if (int_str[0] == '-') {
        (*dst)->sign = 1;  // 음수로 설정
        start_idx = 1;
    }
    else {
        (*dst)->sign = 0;  // 양수로 설정
    }

    // '0x' 접두사 처리 (16진수일 경우)
    if (base == 16) {
        if (strlen(int_str + start_idx) < 2 ||  // "0x" 이후 값이 없음
            !(int_str[start_idx] == '0' && (int_str[start_idx + 1] == 'x' || int_str[start_idx + 1] == 'X'))) {
            return print_invalid_input_error();  // 접두사가 없거나 빈 값인 경우
        }

        start_idx += 2;  // '0x' 건너뛰기
    }

    // 유효한 숫자 문자열인지 확인
    int str_len = strlen(int_str) - start_idx;
    if (str_len == 0) {  // '0x'만 있고 숫자가 없을 때 예외 처리
        return print_invalid_input_error();
    }
    for (int i = start_idx; i < strlen(int_str); i++) {
        if (!isxdigit(int_str[i])) {
            return print_invalid_input_error();
        }
    }

    // 워드 길이 계산
    int word_len = (str_len + 7) / 8;  // 8자리 16진수를 32비트 워드로 변환
    if (*dst == NULL) {
        msg result = bi_new(dst, word_len);
        if (result != SUCCESS_INITIALIZATION) {
            return result;
        }
    }
    else if ((*dst)->word_len < word_len) {
        free((*dst)->a);
        (*dst)->a = (word*)calloc(word_len, sizeof(word));
        if ((*dst)->a == NULL) {
            return print_memory_allocation_error();
        }
        (*dst)->word_len = word_len;
    }

    // 문자열을 16진수로 변환하여 정순으로 워드 배열에 저장
    int word_idx = 0;
    word temp_value = 0;
    int shift = 0;

    for (int i = str_len - 1; i >= 0; i--) {
        char c = int_str[start_idx + i];
        int digit_value = 0;

        if (isdigit(c)) {
            digit_value = c - '0';
        }
        else if (c >= 'a' && c <= 'f') {
            digit_value = c - 'a' + 10;
        }
        else if (c >= 'A' && c <= 'F') {
            digit_value = c - 'A' + 10;
        }

        // 16진수 문자를 변환하고 적절한 비트 위치에 저장
        temp_value |= ((word)digit_value << shift);
        shift += 4;

        // 워드가 다 채워지면 저장
        if (shift == sizeof(word) * 8) {
            (*dst)->a[word_idx++] = temp_value;
            temp_value = 0;
            shift = 0;
        }
    }

    // 남은 값이 있으면 마지막 워드에 저장
    if (shift > 0) {
        (*dst)->a[word_idx] = temp_value;
    }

    return print_success_set_from_string();
}