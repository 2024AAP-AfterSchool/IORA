#ifndef BASE_H
#define BASE_H

#include <stdio.h>
#include "struct.h"

// 함수 리스트 만들기
msg bi_set_from_array(bigint** dst, int sign, int word_len, word* a); // big int -> array 
/* str = 0x123qwrq -> 예외처리 */
msg bi_set_from_string(bigint** dst, char* int_str, int base); // 진수 까지 
/* a[4]= {1,2,3,0} wordlen = 3? -> {1,2,3,1} NO! 4 마지막 non zero */
msg bi_gen_random(bigint** dst, int sign,int word_len);

msg bi_print(bigint* dst, int base); // 10진수 출력 어려움, 나눗셈 구현하고 나중에 함

msg bi_new(bigint** dst, int word_len);

msg bi_delete(bigint** dst);

msg bi_refine(bigint* dst);
/* x tmp <- x */
msg bi_assign(bigint** dst, bigint* src);

#endif // BASE_H
 