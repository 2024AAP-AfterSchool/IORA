#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"
#include "error.h"
#include "base.h"
# define NON_NEGATIVE 0 

msg bi_delete(bigint** dst) {
    // dst�� NULL�� ��� ���� ó��
    if (dst==NULL || *dst == NULL) {
        return print_null_pointer_error();
    }

    // �迭 �޸� ����
    free((*dst)->a);

    // ����ü �޸� ����
    free(*dst);
    *dst = NULL;

    // �޸� ���� ���� �޽��� ���
    return print_success_memory_deallocation();
}

msg bi_new(bigint** dst, int word_len) {
    // ��ȿ���� ���� �ܾ� ���� üũ
    if (word_len <= 0) {
        return print_invalid_word_length_error();
    }

    // �޸𸮰� �̹� �Ҵ�� ��� ó��
    if (*dst != NULL) {
        bi_delete(dst);
        return print_already_freed_error();
    }

    // �޸� �Ҵ�
    *dst = (bigint*)malloc(sizeof(bigint));
    if (*dst == NULL) {
        return print_memory_allocation_error();
    }

    // �ʱ�ȭ
    (*dst)->sign = NON_NEGATIVE;
    (*dst)->word_len = word_len;

    // �迭 �Ҵ�
    (*dst)->a = (word*)calloc(word_len, sizeof(word));
    if ((*dst)->a == NULL) {
        free(*dst);  // ����ü �޸� �Ҵ� ����
        *dst = NULL;
        return print_array_allocation_error();
    }

    // ���������� �ʱ�ȭ �Ϸ� �޽��� ���
    return print_success_initialization();
}



msg copy_array(word* dst, word* src, int word_len) {
    // ��ȿ���� ���� ���� üũ
    if (word_len <= 0) {
        return print_invalid_word_length_error();
    }

    // dst �Ǵ� src�� NULL���� Ȯ�� 
    if (dst == NULL || src == NULL) {
        return print_null_pointer_error();
    }

    // �迭 ���� �õ� error_t��  memcpy_s, fopen_s�� ���� "������" �Լ��鿡�� ��ȯ�Ǵ� ��
    memcpy(dst, src, sizeof(word) * word_len);
    

    // ���������� �迭 ���� �Ϸ�
    return print_success_copy();
}

#define OUT
#define IN

msg bi_set_from_array(bigint** dst, int sign, int word_len, word* b) {
    // ��ȿ���� ���� �ܾ� ���� üũ
    if (word_len <= 0) {
        return print_invalid_word_length_error();
    }

    // b�� NULL�� ��� ���� ó��
    if (b == NULL) {
        return print_null_pointer_error();
    }

    // ���ο� bigint ����ü�� ����
    msg result = bi_new(dst, word_len);
    if (result != SUCCESS_INITIALIZATION) {
        return result;  // bi_new���� �߻��� ���� ��ȯ
    }

    // ��ȣ ����
    (*dst)->sign = sign;

    // �迭 ���� �õ�
    result = copy_array((*dst)->a, b, word_len);
    if (result != SUCCESS_COPY) {
        bi_delete(dst);  // ���� ���� �� �޸� ����
        return result;   // copy_array���� �߻��� ���� ��ȯ
    }

    // ���������� ���� �Ϸ�
    return print_success_set_from_array();
}


msg bi_print(bigint* dst, int base)
{
    // �Է��� NULL�� ��� ���� ó��
    if (dst == NULL) {
        return print_null_pointer_error();
    }

    // ��ȿ���� ���� base �� ���� ó�� (����� 16������ ����)
    if (base != 16) {
        printf("Base %d not supported yet.\n", base);
        return 0;
    }

    /*
    // Bigint�� 0�� ��� �ٷ� ���
    if (dst->word_len == 0 || (dst->word_len == 1 && dst->a[0] == 0)) {
        printf("0x0\n");
    }
    */
    // ��ȣ ���
    if (dst->sign < 0) {
        printf("-");
    }

    printf("0x");

    // ��� ��Ʈ ��� (���� 0�� ��ŵ���� �ʰ� ���)
    for (int i = dst->word_len - 1; i >= 0; i--) {
        printf("%08x", dst->a[i]);
    }

    // ������ ����
    printf("\n");

    return print_success_print();
}

msg rand_array(word* dst, int word_len) {
    // ��ȿ���� ���� ���� üũ
    if (word_len <= 0) {
        return print_invalid_word_length_error();
    }

    // dst�� NULL�� ��� ���� ó��
    if (dst == NULL) {
        return print_null_pointer_error();
    }

    // ������ �迭�� ä��
    unsigned char* c = (unsigned char*)dst;  // �� ����Ʈ�� �б� ���� Ÿ�� ĳ����
    int cnt_i = word_len * sizeof(word);     // �� ����Ʈ �� ���

    while (cnt_i > 0) {
        *c = rand() & 0xff;  // 0~255 ������ ������ �� ����Ʈ�� ����
        c++;
        cnt_i--;
    }

    // ���������� �迭 ���� �Ϸ�
    return print_rand_array();
}

msg bi_gen_random(bigint** dst, int sign, int word_len) {
    // ��ȿ���� ���� �ܾ� ���� ó��
    if (word_len <= 0) {
        return print_invalid_word_length_error();
    }

    // ���ο� bigint ����
    bi_new(dst, word_len);
    // ��ȣ ����
    (*dst)->sign = sign;
    rand_array((*dst)->a,word_len);
    // ���������� ���� bigint ���� �Ϸ�
    return print_success_gen_rand();
}

msg bi_assign(bigint** dst, bigint* src) {

    // �̹� �Ҵ�� �޸� ����
    if (*dst != NULL) {
        msg result = bi_delete(dst);
        if (result != SUCCESS_MEMORY_DEALLOCATION) {
            return result;  // �޸� ���� ���� �� ���� ��ȯ
        }
    }

    // ���ο� bigint ����
    msg result = bi_new(dst, src->word_len);
    if (result != SUCCESS_INITIALIZATION) {
        return result;  // bigint ���� ���� �� ���� ��ȯ
    }

    // ��ȣ ����
    (*dst)->sign = src->sign;

    // �迭 ����
    result = copy_array((*dst)->a, src->a, src->word_len);
    if (result != SUCCESS_COPY) {
        bi_delete(dst);  // ���� ���� �� �Ҵ�� �޸� ����
        return result;   // �迭 ���� ���� �� ���� ��ȯ
    }

    // ���������� �Ҵ� �Ϸ�
    return print_success_assign();
}

msg bi_refine(bigint* x) {
    // �Է��� NULL�� ��� ó��
    if (x == NULL) {
        return print_null_pointer_error();
    }

    int new_wordlen = x->word_len;

    // ���ʿ��� 0 ���� (word_len ����)
    while (new_wordlen > 1) {
        if (x->a[new_wordlen - 1] != 0) {
            break;
        }
        new_wordlen--;
    }

    // ���� 0�� ��� ��ȣ�� NON_NEGATIVE�� ����
    if ((new_wordlen == 1) && (x->a[0] == 0x00)) {
        x->sign = NON_NEGATIVE;
    }

    // �ʿ��ϴٸ� �޸� ���Ҵ�
    if (x->word_len != new_wordlen) {
        word* tmp = (word*)realloc(x->a, sizeof(word) * new_wordlen);
        if (tmp == NULL) {
            return print_memory_allocation_error();  // �޸� ���Ҵ� ���� �� ���� ��ȯ
        }

        // ���Ҵ� ���� �� �޸𸮿� ���̸� ������Ʈ
        x->a = tmp;
        x->word_len = new_wordlen;
    }

    // ���������� ���� �Ϸ�
    return print_success_refine();
}

msg bi_set_from_string(bigint** dst, char* int_str, int base) {
    return 0;
}