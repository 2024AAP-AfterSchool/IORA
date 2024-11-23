/**
 * @file bigint_calculation.c
 * @brief bigint�� ���� ������ �ϴ� �Լ����� ������ �ҽ� ����
 * @date 2024-10-18
 * @author ������
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "string.h"
#include "memory.h"
#include "base_type.h"
#include "base_error.h"
#include "bigint_calculation.h"



bool is_null_pointer(void* ptr)
{
    return ptr == NULL;
}

/**
 * @brief �������� �޸𸮰� �Ҵ�Ǿ� ������ �����ϴ� �Լ�
 * @param ptr ������
 */
void free_if_exist(void** ptr)
{
    if (!is_null_pointer(ptr) && !is_null_pointer(*ptr))
    {
        printf("Freeing memory...\n");
        printf("ptr: %p\n", ptr);
        free(*ptr);
        *ptr = NULL;
    }
}
 //#define create_prameter
uint32_t str_to_hex(word** dst, char* str, int base)
{
    char* end = NULL;

    uint32_t length = strlen(str);
    uint32_t chunk_size = 8;

    free_if_exist(dst);

    for (int i = 0; i < length; i += chunk_size)
    {
        char* chunk = str + i;
        printf("chunk: %s\n", chunk);
        word num_of_chunk = strtol(chunk, &end, base);
        printf("num_from_str: %d\n", num_of_chunk);
        uint32_t is_valid = !(strcmp(end, ""));

        if (is_valid)
        {
            *(*dst) = num_of_chunk;
            (*dst)++;
        }

        if (!is_valid)
        {
            return -1;
        }
    }
}

/**
 * @brief ���ڿ��� 10�������� Ȯ���ϴ� �Լ�
 * @param c ���ڿ�
 * @return 10�����̸� 1, �ƴϸ� 0
 */
uint32_t is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

/**
 * @brief ���ڿ��� 16�������� Ȯ���ϴ� �Լ�
 * @param c ���ڿ�
 * @return 16�����̸� 1, �ƴϸ� 0
 */
uint32_t is_hex_digit(char c)
{
    return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}
 /**
  * @brief bigint�� ���� �ٸ� bigint�� �Ҵ��ϴ� �Լ�
  * @param dst bigint�� ������
  */
msg bi_delete(OUT bigint** dst)
{
    if (is_null_pointer(dst) || is_null_pointer(*dst))
    {
        return print_null_pointer_error();
    }

    free((*dst)->start);
    free(*dst);
    *dst = NULL;

    return print_success_memory_deallocation();
}

/**
 * @brief bigint�� arrary�� �Ҵ��ϴ� �Լ�
 * @param dst bigint�� ������
 * @param wordlen �迭�� ����
 */
msg bi_new(OUT bigint** dst, IN uint32_t wordlen)
{
    if (wordlen <= 0)
    {
        return print_invalid_word_length_error();
    }

    if (!is_null_pointer(*dst))
    {
        if (VERBOSE) fprintf(stdout, "[Warning] Already freed pointer.\n");

        bi_delete(dst);
    }

    *dst = (bigint*)malloc(sizeof(bigint));

    if (is_null_pointer(*dst))
    {
        return print_memory_allocation_error();
    }

    (*dst)->sign = POSITIVE;
    (*dst)->wordlen = wordlen;
    (*dst)->start = (word*)calloc(wordlen, sizeof(word));

    if (is_null_pointer((*dst)->start))
    {
        free(*dst);
        return print_array_allocation_error();
    }

    return print_success_initialization();
}

/**
* @memory ���� copy�ϴ� �Լ� �߰���
*/
msg array_copy(OUT word* dst, IN word* src, IN uint32_t wordlen)
{
    if (wordlen <= 0)
    {
        return print_invalid_word_length_error();
    }

    if (is_null_pointer(dst) || is_null_pointer(src))
    {
        return print_null_pointer_error();
    }

    memcpy(dst, src, sizeof(word) * wordlen);

    return print_success_copy();
}

/**
 * @brief arrary�� ���� bigint�� ���� �Ҵ��ϴ� �Լ�
 * @param dst bigint�� ������
 * @param sign ��ȣ
 * @param wordlen �迭�� ����
 * @param src �Ҵ��ϰ��� �ϴ� �迭
 */
msg bi_set_from_array(OUT bigint** dst, IN uint32_t sign, IN uint32_t wordlen, IN word* src)
{
    if (wordlen <= 0)
    {
        return print_invalid_word_length_error();
    }

    if (is_null_pointer(src))
    {
        return print_null_pointer_error();
    }

    msg result = bi_new(dst, wordlen);
    if (result != SUCCESS_INITIALIZATION)
    {
        return result;
    }

    (*dst)->sign = sign;

    result = array_copy((*dst)->start, src, wordlen);
    if (result != SUCCESS_COPY)
    {
        return result;
    }

    return print_success_set_from_array();
}

/**
 * @brief string�� ���� bigint�� ���� �Ҵ��ϴ� �Լ�
 * @param dst bigint�� ������
 * @param str �Ҵ��ϰ��� �ϴ� ���ڿ�
 * @param base 10����, 16���� �� ��� �ؼ����� �ǹ���.
 */
msg bi_set_from_string(OUT bigint** dst, IN char* str, IN uint32_t base)
{
    // ���� ó��: NULL ������ �Ǵ� �߸��� ���� �� üũ
    if (dst == NULL || str == NULL || base != 16) {
        return print_null_pointer_error();
    }
    // �޸𸮰� �̹� �Ҵ�� ���, �ߺ� �Ҵ� ����
    if (*dst == NULL) {
        msg result = bi_new(dst, 1);  // �ּ� ũ��� ���� �Ҵ�
        if (result != SUCCESS_INITIALIZATION) {
            return result;
        }
    }

    // ��ȣ Ȯ��
    int start_idx = 0;
    if (str[0] == '-') {
        (*dst)->sign = 1;  // ������ ����
        start_idx = 1;
    }
    else {
        (*dst)->sign = 0;  // ����� ����
    }

    // '0x' ���λ� ó�� (16������ ���)
    if (base == 16) {
        if (strlen(str + start_idx) < 2 ||  // "0x" ���� ���� ����
            !(str[start_idx] == '0' && (str[start_idx + 1] == 'x' || str[start_idx + 1] == 'X'))) {
            return print_invalid_input_error();  // ���λ簡 ���ų� �� ���� ���
        }

        start_idx += 2;  // '0x' �ǳʶٱ�
    }

    // ��ȿ�� ���� ���ڿ����� Ȯ��
    int str_len = strlen(str) - start_idx;
    if (str_len == 0) {  // '0x'�� �ְ� ���ڰ� ���� �� ���� ó��
        return print_invalid_input_error();
    }
    for (int i = start_idx; i < strlen(str); i++) {
        if (!is_hex_digit(str[i]))
        {  // 16���� �������� ���� üũ
            return print_invalid_input_error();
        }
    }

    // ���� ���� ���
    int wordlen = (str_len + 7) / 8;  // 8�ڸ� 16������ 32��Ʈ ����� ��ȯ
    if (*dst == NULL) {
        msg result = bi_new(dst, wordlen);
        if (result != SUCCESS_INITIALIZATION) {
            return result;
        }
    }
    else if ((*dst)->wordlen < wordlen) {
        free((*dst)->start);
        (*dst)->start = (word*)calloc(wordlen, sizeof(word));
        if ((*dst)->start == NULL) {
            return print_memory_allocation_error();
        }
        (*dst)->wordlen = wordlen;
    }

    // ���ڿ��� 16������ ��ȯ�Ͽ� �������� ���� �迭�� ����
    int word_idx = 0;
    word temp_value = 0;
    int shift = 0;

    for (int i = str_len - 1; i >= 0; i--) {
        char c = str[start_idx + i];
        int digit_value = 0;

        if (is_digit(c)) {
            digit_value = c - '0';
        }
        else if (c >= 'a' && c <= 'f') {
            digit_value = c - 'a' + 10;
        }
        else if (c >= 'A' && c <= 'F') {
            digit_value = c - 'A' + 10;
        }

        // 16���� ���ڸ� ��ȯ�ϰ� ������ ��Ʈ ��ġ�� ����
        temp_value |= ((word)digit_value << shift);
        shift += 4;

        // ���尡 �� ä������ ����
        if (shift == sizeof(word) * 8) {
            (*dst)->start[word_idx++] = temp_value;
            temp_value = 0;
            shift = 0;
        }
    }

    // ���� ���� ������ ������ ���忡 ����
    if (shift > 0) {
        (*dst)->start[word_idx] = temp_value;
    }

    return print_success_set_from_string();
}

/**
 * @brief bigint�� ������ ���� �Ҵ��ϴ� �Լ�
 * @create_prime�̸� ������ ���� all 0�� �Ǵ� ���� ���� = Ű ũ�� �ִ��� ����
 * @param dst bigint�� ������
 * @param sign ��ȣ
 * @param wordlen �迭�� ����
 */
msg bi_get_random(OUT bigint** dst, IN uint32_t sign, IN uint32_t wordlen)
{
    if (wordlen <= 0)
    {
        return print_invalid_word_length_error();
    }

    bi_new(dst, wordlen);

    (*dst)->sign = sign;

    for (int i = 0; i < wordlen; i++)
    {
        (*dst)->start[i] = rand() & 0xFF;
    }

#ifdef create_prameter
    if ((*dst)->start[wordlen - 1] == 0)
    {
        (*dst)->start[wordlen - 1] = rand() & 0xFF;
        while ((*dst)->start[wordlen - 1] == 0)
        {
            (*dst)->start[wordlen - 1] = rand() & 0xFF; // 0�� �ƴ� ���� ����
        }
    }
#endif
    bi_refine(*dst);

    return print_success_gen_rand();
}

/**
 * @brief bigint�� ������ ���尡 0�� ��� bigint�� �޸𸮸� ���Ҵ��ϴ� �Լ�
 * @param dst bigint�� ������
 */
msg bi_refine(OUT bigint* dst)
{
    if (is_null_pointer(dst))
    {
        return print_null_pointer_error();
    }

    while ((dst->start[dst->wordlen - 1] == 0) && (dst->wordlen > 1))
    {
        dst->wordlen--;
    }

    // ���ο� �����͸� �޾� ���� �����͸� �״�� �����ϴ� ������� realloc ���
    word* new_start = (word*)realloc(dst->start, dst->wordlen * sizeof(word));
    if (is_null_pointer(new_start))
    {
        return print_memory_allocation_error();
    }

    // ���������� �Ҵ�Ǿ����Ƿ� ������ ������Ʈ
    dst->start = new_start;

    if ((dst->start[0] == 0) && (dst->wordlen == 1))
    {
        dst->sign = POSITIVE;
    }

    return print_success_refine();
}

/**
 * @brief bigint�� ���� �ٸ� bigint�� �Ҵ��ϴ� �Լ�
 * @param dst bigint�� ������
 * @param src bigint�� ������
 */
msg bi_assign(OUT bigint** dst, IN bigint* src)
{
    if (is_null_pointer(src))
    {
        return print_null_pointer_error();
    }

    msg result = bi_new(dst, src->wordlen);
    if (result != SUCCESS_INITIALIZATION)
    {
        return result;
    }

    (*dst)->sign = src->sign;

    result = array_copy((*dst)->start, src->start, src->wordlen);
    if (result != SUCCESS_COPY)
    {
        free_if_exist((void**)*dst);
        return result;
    }

    return print_success_assign();
}

/**
 * @brief bigint�� ����ϴ� �Լ�
 * @param dst bigint�� ������
 * @param base 10����, 16���� �� ��� �ؼ����� �ǹ���.
 */
msg bi_print(OUT bigint* dst, IN uint32_t base)
{
    if (is_null_pointer(dst))
    {
        return print_null_pointer_error();
    }

    if (base != 16)
    {
        return fprintf(stdout, "Base %d not supported yet.\n", base);
    }

    if (dst->sign == NEGATIVE)
    {
        printf("-");
    }

    printf("0x");
    for (int i = dst->wordlen - 1 ; i >= 0; i--)
    {
        printf("%08X", dst->start[i]);
        // TODO: base�� ���� ��� ������ ������ �� �ֵ��� ����
    }
    printf("\n");

    return print_success_print();
}

/**
 * @brief bigint�� 0���� Ȯ���ϴ� �Լ�
 * @param dst bigint�� ������
 * @return 0�̸� 1, �ƴϸ� 0
 */
bool bi_is_zero(OUT bigint* dst)
{
    if (is_null_pointer(dst))
    {
        return print_null_pointer_error();
    }

    if (dst->sign == NEGATIVE)
    {
        return false;
    }

    for (int i = dst->wordlen - 1; i >= 0; i--)
    {
        if (dst->start[i] != 0)
        {
            return false;
        }
    }

    return true;
}

/**
 * @brief bigint�� 1���� Ȯ���ϴ� �Լ�
 * @param dst bigint�� ������
 * @return 1�̸� 1, �ƴϸ� 0
 */
bool bi_is_one(OUT bigint* dst)
{
    if (dst == NULL) return false;
    if (dst->start[0] != 1) return false;
    for (int i = 1; i < (dst->wordlen); i++)
    {
        if (dst->start[i] != 0) return false;
    }
    return true;
}

void bi_set_zero(OUT bigint** x)
{
    bi_new(x, 1);
    (*x)->sign = POSITIVE;
    (*x)->start[0] = 0;
}

int8_t bi_compare_ABS(IN bigint* x, IN bigint* y)
{
    int n = x->wordlen;
    int m = y->wordlen;

    // Case: A > B
    if (n > m)
        return 1;

    // Case: A < B
    if (n < m)
        return -1;

    for (int i = n - 1; i >= 0; i--) {

        if (x->start[i] > y->start[i])
            return 1;

        if (x->start[i] < y->start[i])
            return -1;

    }

    return 0;
}

int8_t bi_compare(IN bigint* x, IN bigint* y)
{
    if (x->sign == POSITIVE && y->sign == NEGATIVE)
        return 1;

    if (x->sign == NEGATIVE && y->sign == POSITIVE)
        return -1;

    int ret = bi_compare_ABS(x, y);

    if (x->sign == POSITIVE)
        return ret;

    return ret * (-1);
}

/**
 * @brief word�� �������� shift�ϴ� �Լ�
 * @param dst bigint�� ������
 * @param k shift �ϰ���� word ������
 */
msg bi_word_left_shift(OUT bigint** dst, IN byte k)
{
    bigint* tmp = NULL;
    bi_new(&tmp, (*dst)->wordlen + k);
    tmp->sign = (*dst)->sign;

    for (int i = k; i < tmp->wordlen; i++)
    {
        tmp->start[i] = (*dst)->start[i - k];
    }
    //bi_refine(tmp); // dst�� refine�� ���̶�� �� �ʿ� x
    // dst�� NULL�� �����Ͽ� bi_assign�� �����ϵ��� �غ�

    // bi_assign�� ����Ͽ� tmp�� ���� dst�� ����
    bi_assign(dst, tmp);

    // tmp �� ���� dst�� �޸𸮸� ����
    bi_delete(&tmp);

    return print_success_shift();
}

msg bi_bit_left_shift(OUT bigint** dst, IN byte k) {
    if (k == 0 || dst == NULL || *dst == NULL) {
        return SUCCESS_SHIFT; // �̵��� �ʿ� ���ų� �Է��� NULL�� ���
    }

    uint32_t word_shift = k / (sizeof(word) * 8);  // ���� ���� �̵� ���
    uint32_t bit_shift = k % (sizeof(word) * 8);   // ���� �� ��Ʈ �̵� ���

    // ���ο� bigint ���� (�߰� ���� ���)
    uint32_t new_wordlen = (*dst)->wordlen + word_shift + (bit_shift > 0 ? 1 : 0);
    bigint* tmp = NULL;
    bi_new(&tmp, new_wordlen); // �޸� �Ҵ�
    tmp->sign = (*dst)->sign;  // ���� ��ȣ ����

    // ��Ʈ �̵� ����
    for (int32_t i = (*dst)->wordlen - 1; i >= 0; i--) {
        // ���� �̵� �� ��Ʈ �̵�
        tmp->start[i + word_shift] |= (*dst)->start[i] << bit_shift;

        // ���� ��Ʈ �ʰ����� ���� ����� ����
        if (bit_shift > 0 && (i + word_shift + 1) < new_wordlen) {
            tmp->start[i + word_shift + 1] |= (*dst)->start[i] >> (sizeof(word) * 8 - bit_shift);
        }
    }

    // ���� �߰��� ���� �ʱ�ȭ
    for (uint32_t i = 0; i < word_shift; i++) {
        tmp->start[i] = 0; // ���� ������� 0���� �ʱ�ȭ
    }

    // ���� dst�� tmp�� ��ü
    bi_assign(dst, tmp);

    // tmp �޸� ����
    bi_delete(&tmp);

    return SUCCESS_SHIFT; // ���� ���� ��ȯ
}
/**
 * @brief word�� ���������� shift�ϴ� �Լ�
 * @param dst bigint�� ������
 * @param k shift �ϰ���� word ������
 */
msg bi_word_right_shift(OUT bigint** dst, IN byte k) // k�� shift�ϰ���� word ������ �ǹ�
{
    bigint* tmp = NULL;
    bi_new(&tmp, (*dst)->wordlen - k);
    tmp->sign = (*dst)->sign;

    for (int i = k; i < tmp->wordlen; i++)
    {
        tmp->start[i] = (*dst)->start[i + k];
    }

    //bi_refine(tmp); // dst�� refine�� ���̶�� �� �ʿ� x
    // dst�� NULL�� �����Ͽ� bi_assign�� �����ϵ��� �غ�

    // bi_assign�� ����Ͽ� tmp�� ���� dst�� ����
    bi_assign(dst, tmp);

    // tmp �� ���� dst�� �޸𸮸� ����
    bi_delete(&tmp);

    return print_success_shift();
}

msg bi_bit_right_shift(OUT bigint** dst, IN byte k) // k�� shift�ϰ���� bit ������ �ǹ�
{
    bigint* tmp = NULL;
    bi_new(&tmp, (*dst)->wordlen - k);
    tmp->sign = (*dst)->sign;

    for (int i = k; i < tmp->wordlen; i++)
    {
        tmp->start[i] = (*dst)->start[i + k];
    }

    //bi_refine(tmp); // dst�� refine�� ���̶�� �� �ʿ� x
    // dst�� NULL�� �����Ͽ� bi_assign�� �����ϵ��� �غ�

    // bi_assign�� ����Ͽ� tmp�� ���� dst�� ����
    bi_assign(dst, tmp);

    // tmp �� ���� dst�� �޸𸮸� ����
    bi_delete(&tmp);

    return print_success_shift();
}