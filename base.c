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
    free((*dst)->a);

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
        bi_delete(dst);
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
    return 0;
}