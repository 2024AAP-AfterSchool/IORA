#ifndef BASE_H
#define BASE_H

#include <stdio.h>
#include "struct.h"

// �Լ� ����Ʈ �����
msg bi_set_from_array(bigint** dst, int sign, int word_len, word* a); // big int -> array 
/* str = 0x123qwrq -> ����ó�� */
msg bi_set_from_string(bigint** dst, char* int_str, int base); // ���� ���� 
/* a[4]= {1,2,3,0} wordlen = 3? -> {1,2,3,1} NO! 4 ������ non zero */
msg bi_gen_random(bigint** dst, int sign,int word_len);

msg bi_print(bigint* dst, int base); // 10���� ��� �����, ������ �����ϰ� ���߿� ��

msg bi_new(bigint** dst, int word_len);

msg bi_delete(bigint** dst);

msg bi_refine(bigint* dst);
/* x tmp <- x */
msg bi_assign(bigint** dst, bigint* src);

#endif // BASE_H
 