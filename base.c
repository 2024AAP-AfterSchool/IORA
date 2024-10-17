
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"
#include "error.h"
#include "base.h"
# define NON_NEGATIVE 0 

msg bi_new(bigint** dst, int word_len) {

    if (*dst != NULL) bi_delete(dst);

    *dst = (bigint*)malloc(sizeof(bigint));
    if ((*dst) != NULL) {
        (*dst)->sign = NON_NEGATIVE;
        (*dst)->word_len = word_len;
        (*dst)->a = (word*)calloc(word_len, sizeof(word));
    }
}

msg bi_delete(bigint** dst)
{
    if (*dst == NULL) {
        return;
    }
    else {
        free((*dst)->a);
        free(*dst);
        *dst = NULL;
    }

}

msg copy_array(word* dst, word* src, int word_len) {
    memcpy(dst, src, sizeof(word) * word_len);
}

#define OUT
#define IN

msg bi_set_from_array(OUT bigint** dst, IN int sign, IN int word_len, IN word* b) {

    bi_new(dst, word_len);
    (*dst)->sign = sign;
    copy_array((*dst)->a, b, word_len); // b의 값 a에 복사


}



msg bi_print(const bigint* dst, int base)
{
    if (base != 16) {
        printf("not yet.\n");
    }
    printf("0x");
    for (int i = (dst->word_len); i >= 0; i--) {
        printf("%08x", dst->a[i]);
    }
}

msg rand_array(word* dst, int word_len)
{
    unsigned char* c = (unsigned char*)dst;  //한바이트 읽기위한 타입캐스팅
    int cnt_i = word_len * sizeof(word);
    while (cnt_i > 0) {
        *c = rand() & 0xff; // rand 0~32767
        c++;
        cnt_i = cnt_i - 1;
    }
}

msg bi_gen_ramdom(bigint** dst, int sign, int word_len)
{
    bi_new(dst, word_len);
    (*dst)->sign = sign;
    rand_array((*dst)->a, word_len);
    bi_refine(*dst); //왜 refine?
}

msg bi_assign(bigint** dst, bigint* src)
{
    if ((*dst) != 0) {
        bi_delete(dst);
    }
    bi_new(dst, src->word_len);
    (*dst)->sign = src->sign;
    copy_array((*dst)->a, src->a, src->word_len);

}

msg bi_refine(bigint* x)
{
    if (x == NULL)
        return;

    int new_wordlen = x->word_len;
    while (new_wordlen > 1) {
        if (x->a[new_wordlen - 1] != 0)
            break;
        new_wordlen--;
    }

    if ((new_wordlen == 1) && (x->a[0] == 0x00))
        x->sign = NON_NEGATIVE;

    word* tmp = NULL;
    if (x->word_len != new_wordlen) {
        x->word_len = new_wordlen;
        tmp = x->a;
        x->a = (word*)realloc(x->a, sizeof(word) * new_wordlen);
        if (x->a == NULL) {
            printf("Memory Reallocation Fail\n");
            x->a = tmp;
        }
    }
}
