/**
 * @file bigint_multiplication.c
 * @brief bigint�� ���� ���� ������ �ϴ� �Լ����� ������ �ҽ� ����
 * @date 2024-11-08
 * @author �賲��
 */
#include <stdio.h>
#include "base_type.h"
#include "base_error.h"
#include "bigint_addition.h"
#include "bigint_subtraction.h"
#include "bigint_calculation.h"
#include "bigint_multiplication.h"
#include "bigint_division.h"

 /**
  * @brief �� ���� ���� A�� B�� ������ �����ϴ� �Լ�
  * @param dst ����� ������ bigint�� ������
  * @param A ������ ������ ù ��° bigint
  * @param B ������ ������ �� ��° bigint
  */
msg bi_mul_AB(OUT bigint** dst, IN word* A, IN word* B)
{
    bigint* C = NULL;
    bi_new(&C, 2);

    word mask = (sizeof(word) == 4) ? 0xFFFF : 0xFFFFFFFF;

    // A�� ������ ���� ��Ʈ ����
    word A1 = *A >> (sizeof(word) * 4); // ���� ��Ʈ ����
    word A0 = *A & mask;                // ���� ��Ʈ ����
    // B�� ������ ���� ��Ʈ ����
    word B1 = *B >> (sizeof(word) * 4); // ���� ��Ʈ ����
    word B0 = *B & mask;                // ���� ��Ʈ ����
    // A1B0+A0B1 ����ϴ� �κ�
    word T1 = A1 * B0;
    word T0 = A0 * B1;
    T0 = T1 + T0;                //A1B0+A0B1 ��ⷯ����
    T1 = (T0 < T1) ? 1 : 0;
    word C1 = A1 * B1;
    word C0 = A0 * B0;
    word tmp = C0;
    C0 = C0 + (T0 << (sizeof(word) * 4));
    C1 = C1 + (T1 << (sizeof(word) * 4)) + (T0 >> (sizeof(word) * 4)) + (C0 < tmp);
    C->start[0] = C0;
    C->start[1] = C1;

    // �ʱ�ȭ �Լ�
    bi_assign(dst, C);
    bi_delete(&C);
    return print_success_mul_AB();
}

/**
 * @brief �� bigint A�� B�� ������ �����ϴ� �Լ�
 * @param dst ����� ������ bigint�� ������
 * @param A ������ ������ ù ��° bigint
 * @param B ������ ������ �� ��° bigint
 */
msg bi_mul_C(OUT bigint** dst, IN bigint* A, IN bigint* B)
{
    bigint* C = NULL;
    bigint* T = NULL;
    bi_new(&C, A->wordlen + B->wordlen);
    bi_new(&C, A->wordlen + 1);
    bigint* tmp_A = NULL;

    // A�� Ȧ�� ������ ��� Ȯ��
    if (A->wordlen % 2 == 1) {
        bi_new(&tmp_A, A->wordlen + 1);
        for (int i = 0; i < A->wordlen; i++) {
            tmp_A->start[i] = A->start[i];
        }
        tmp_A->start[A->wordlen] = 0;
    }
    else {
        bi_assign(&tmp_A, A);
    }

    // ���� �� ��� ����
    for (int i = 0; i < B->wordlen; i++) {
        bigint* T0 = NULL;
        bigint* T1 = NULL;
        bigint* tmp_T0 = NULL;
        bigint* tmp_T1 = NULL;
        bi_new(&T0, 2); // T0�� 2 ���� ũ��� ����
        bi_new(&T1, 2); // T1�� 2 ���� ũ��� ����
        bi_new(&tmp_T0, (tmp_A->wordlen));
        bi_new(&tmp_T1, (tmp_A->wordlen) + 1);

        for (int j = 0; j < (tmp_A->wordlen / 2); j++) {
            msg result;

            result = bi_mul_AB(&T0, &(tmp_A->start[2 * j]), &(B->start[i]));
            if (result != SUCCESS_MUL_AB) return print_fail_mul_AB();
            tmp_T0->start[2 * j] = T0->start[0];
            tmp_T0->start[(2 * j) + 1] = T0->start[1];

            result = bi_mul_AB(&T1, &(tmp_A->start[(2 * j) + 1]), &(B->start[i]));
            if (result != SUCCESS_MUL_AB) return print_fail_mul_AB();
            tmp_T1->start[(2 * j) + 1] = T1->start[0];
            tmp_T1->start[(2 * (j + 1))] = T1->start[1];
        }
        bi_add(&T, tmp_T1, tmp_T0);
        // i��° ���忡 ���� ����Ʈ ó�� �� ���� �� ��
        bi_word_left_shift(&T, i);
        bi_add(&C, C, T);

        // �ӽ� ���� ����
        bi_delete(&T0);
        bi_delete(&T1);
        bi_delete(&tmp_T0);
        bi_delete(&tmp_T1);
    }

    // ���� ��� �Ҵ�
    bi_refine(C);
    bi_assign(dst, C);
    bi_delete(&C);
    bi_delete(&tmp_A);

    return print_success_mul_C();
}

/**
 * @brief �� bigint A�� B�� ������ �����ϴ� �Լ�
 * @param dst ����� ������ bigint�� ������
 * @param A ������ ������ ù ��° bigint
 * @param B ������ ������ �� ��° bigint
 * @param is_karatsuba ī���߹� ������ ������� ����
 */

msg bi_mul(OUT bigint** dst, IN bigint* A, IN bigint* B, IN bool is_karatsuba)
{
    bigint* tmp_A = NULL;
    bigint* tmp_B = NULL;
    bigint* result = NULL;
    bi_assign(&tmp_A, A);
    bi_assign(&tmp_B, B);
    /*
    if (bi_is_zero(tmp_A) || bi_is_zero(tmp_B))
    {
        bigint* tmp_dst = NULL;
        bi_new(&tmp_dst, 1);
        tmp_dst->start[0] = 0;
        bi_assign(dst, tmp_dst);

        bi_delete(&tmp_A);
        bi_delete(&tmp_B);
        bi_delete(&tmp_dst);
        return print_success_mul();
    }
    if (bi_is_one(tmp_A))
    {
        bi_assign(dst, tmp_B);
        (*dst)->sign = (tmp_A->sign ^ tmp_B->sign);

        bi_delete(&tmp_A);
        bi_delete(&tmp_B);

        return print_success_mul();
    }
    if (bi_is_one(tmp_B))
    {
        bi_assign(dst, tmp_A);
        (*dst)->sign = (tmp_A->sign ^ tmp_B->sign);

        bi_delete(&tmp_A);
        bi_delete(&tmp_B);

        return print_success_mul();
    }
    */
    //ī���߹ٿ� ��� ���� �ְ� ��ȣ�� ���� �����ϱ� ����
    byte A_sign = tmp_A->sign;
    byte B_sign = tmp_B->sign;
    tmp_A->sign = POSITIVE;
    tmp_B->sign = POSITIVE;

    // if ( is_karatsuba) mul_karatsuba(dst, tmp_A, tmp_B);
    if (!is_karatsuba) bi_mul_C(&result, tmp_A, tmp_B);
     
    result->sign = A_sign ^ B_sign;
    bi_assign(dst, result);
    bi_delete(&tmp_A);
    bi_delete(&tmp_B);

    return print_success_mul();
}

msg bi_karatsuba_mul(OUT bigint** dst, IN bigint* A, IN bigint* B) {

    bigint* tmp_A = NULL;
    bigint* tmp_B = NULL;

    bi_assign(&tmp_A, A);
    bi_assign(&tmp_B, B);

    uint32_t n = (tmp_A->wordlen > B->wordlen) ? tmp_A->wordlen : tmp_B->wordlen;


    if (n == 1) {
        return bi_mul_C(dst, tmp_A, tmp_B);
    }

    uint32_t half = (n + 1) / 2;


    bigint* A1 = NULL, * A0 = NULL, * B1 = NULL, * B0 = NULL;


    bi_new(&A1, n - half);
    bi_new(&A0, half);
    for (uint32_t i = 0; i < half; i++) {
        A0->start[i] = (i < tmp_A->wordlen) ? tmp_A->start[i] : 0;
    }
    for (uint32_t i = half; i < n; i++) {
        A1->start[i - half] = (i < tmp_A->wordlen) ? tmp_A->start[i] : 0;
    }


    bi_new(&B1, n - half);
    bi_new(&B0, half);
    for (uint32_t i = 0; i < half; i++) {
        B0->start[i] = (i < tmp_B->wordlen) ? tmp_B->start[i] : 0;
    }
    for (uint32_t i = half; i < n; i++) {
        B1->start[i - half] = (i < tmp_B->wordlen) ? tmp_B->start[i] : 0;
    }


    bigint* T1 = NULL, * T0 = NULL, * T2 = NULL;
    bi_karatsuba_mul(&T1, A1, B1); // T1 = A1 * B1
    T1->sign = A1->sign ^ B1->sign;
    bi_karatsuba_mul(&T0, A0, B0); // T0 = A0 * B0
    T0->sign = A0->sign ^ B0->sign;

    //S1 = A0 - A1, S2 = B1 - B0
    bigint* S1 = NULL, * S2 = NULL;
    bi_sub(&S1, A0, A1);
    bi_sub(&S2, B1, B0);

    //T2 = S1 * S2
    bi_karatsuba_mul(&T2, S1, S2);
    T2->sign = S1->sign ^ S2->sign;
    // T2: T2 = T2 + T1 + T0
    bi_add(&T2, T2, T1);
    bi_add(&T2, T2, T0);


    bi_word_left_shift(&T1, 2 * half);
    bi_word_left_shift(&T2, half);

    bigint* result = NULL;
    bi_add(&result, T1, T2);
    bi_add(&result, result, T0);

    result->sign = A->sign ^ B->sign;

    // Assign result to dst
    bi_refine(result);
    bi_assign(dst, result);


    bi_delete(&A1); bi_delete(&A0);
    bi_delete(&B1); bi_delete(&B0);
    bi_delete(&T1); bi_delete(&T0); bi_delete(&T2);
    bi_delete(&S1); bi_delete(&S2);
    bi_delete(&result);

    return SUCCESS_MUL_C;
}


msg squaring_AA(OUT bigint **dst, IN bigint * A)
{
    bigint* C = NULL;
    bi_new(&C, 2); // ����� ũ���� bigint ����
    int half_word_size = sizeof(word) * 4;

    // word ũ�⿡ ���� ����ũ ����
#if (half_word_size == 16)
    // 32��Ʈ word�� ���
    word mask = (1U << (half_word_size)) - 1;
#else
    // 64��Ʈ word�� ���
    word mask = (1ULL << (half_word_size)) - 1;
#endif

    // A�� ������ ���� ��Ʈ ����

    word A1 = *A->start >> half_word_size; // A�� ���� ��Ʈ
    word A0 = *A->start & mask;            // A�� ���� ��Ʈ

    // C1 = A1^2, C0 = A0^2
    bigint* C1 = NULL;
    bi_new(&C1, 1);
    C1->start[0] = (A1 * A1);

    bigint* C0 = NULL;
    bi_new(&C0, 1);
    C0->start[0] = (A0 * A0);
    // C = (C1 << w) + C0

    bi_word_left_shift(&C1, 1);             // C1 << w
    
    bi_add(&C, C0,  C1);                    // C = C1 + C0

    // T = A0 * A1
    bigint* T = NULL;
    bi_new(&T, 1);
    word cross = A0 * A1;
    T->start[0] = cross;

    // T = T << (w/2 + 1)
    bi_bit_left_shift(&T, half_word_size + 1);
   
    // C = C + T
    bi_add(&C, C, T);
    bi_refine(C);
    // ����� dst�� ����
    bi_assign(dst, C);

    // �޸� ����
    bi_delete(&C);
    bi_delete(&C1);
    bi_delete(&C0);
    bi_delete(&T);

    return SUCCESS_SQUARE_A;
}

msg bi_square_C(OUT bigint** dst, IN bigint* x) {

    bigint* C1 = NULL;
    bigint* C2 = NULL;
    bigint* T1 = NULL;
    bigint* T2 = NULL;

    // C1, C2 �ʱ�ȭ
    bi_new(&C1, x->wordlen * 2);
    bi_new(&C2, x->wordlen * 2);

    // C1 ���
    for (int j = 0; j < x->wordlen; j++) {
        // T1 = x[j]^2
        bigint* temp = NULL;
        bi_new(&temp, 1);
        temp->start[0] = x->start[j];
        squaring_AA(&T1, temp);
        bi_delete(&temp);

        // T1 <<= 2 * j (���� ���� �̵�)
        bi_word_left_shift(&T1, 2 * j);

        // C1 += T1
        bi_add(&C1, C1, T1);
        bi_delete(&T1);
    }

    // C2 ���
    for (int j = 0; j < x->wordlen; j++) {
        for (int i = j + 1; i < x->wordlen; i++) {
            // T2 = x[j] * x[i]
            bigint* temp1 = NULL;
            bigint* temp2 = NULL;
            bi_new(&temp1, 1);
            bi_new(&temp2, 1);
            temp1->start[0] = x->start[j];
            temp2->start[0] = x->start[i];
            bi_mul_AB(&T2, &(temp1->start[0]), &(temp2->start[0]));
            bi_delete(&temp1);
            bi_delete(&temp2);

            // T2 <<= (i + j)
            bi_word_left_shift(&T2, i + j);

            // C2 += T2
            bi_add(&C2, C2, T2);
            bi_delete(&T2);
        }
    }

    // C2 <<= 1
    bi_bit_left_shift(&C2, 1);

    // C = C1 + C2
    bi_add(dst, C1, C2);

    // �޸� ����
    bi_delete(&C1);
    bi_delete(&C2);

    return SUCCESS_SQUARE_A;
}


msg squaring_Karatsuba(OUT bigint** dst, IN bigint* A) {
    if (A == NULL || A->wordlen == 0) {
        bi_new(dst, 1);
        (*dst)->start[0] = 0;
        return SUCCESS_SQUARE_A;
    }

    if (A->wordlen == 1) {
        return bi_square_C(dst, A);
    }

    uint32_t l = (A->wordlen + 1) / 2;
    bigint* A1 = NULL;
    bigint* A0 = NULL;
    bi_new(&A1, A->wordlen - l);
    bi_new(&A0, l);

    for (uint32_t i = 0; i < l; i++) {
        if (i < A->wordlen) {
            A0->start[i] = A->start[i];
        }
        else {
            A0->start[i] = 0;
        }
    }
    for (uint32_t i = l; i < A->wordlen; i++) {
        if (i < A->wordlen) {
            A1->start[i - l] = A->start[i];
        }
        else {
            A1->start[i - l] = 0;
        }
    }

    bigint* T1 = NULL;
    bigint* T0 = NULL;
    bigint* R = NULL;
    bigint* S = NULL;

    squaring_Karatsuba(&T1, A1);
    bi_refine(T1);
    squaring_Karatsuba(&T0, A0);
    bi_refine(T0);
    bi_word_left_shift(&T1, 2 * l);
    bi_refine(T1);
    bi_add(&R, T1, T0);

    bi_karatsuba_mul(&S, A1, A0);

    bi_word_left_shift(&S, l);
    bi_bit_left_shift(&S, 1);

    bigint* C = NULL;
    bi_add(&C, R, S);

    bi_assign(dst, C);

    bi_delete(&A1);
    bi_delete(&A0);
    bi_delete(&T1);
    bi_delete(&T0);
    bi_delete(&R);
    bi_delete(&S);

    return SUCCESS_SQUARE_A;
}

msg bi_left_to_right(OUT bigint** dst, IN bigint* A, IN bigint* exp)
{
    if (A == NULL || exp == NULL || dst == NULL) {
        return NULL_POINTER_ERROR;
    }

    // ��� ���� �ʱ�ȭ 
    bigint* result = NULL;
    bi_new(&result, 1);
    result->start[0] = 1;

    // ����� base�� exp ����
    bigint* tmp_base = NULL;
    bigint* tmp_exp = NULL;
    bi_assign(&tmp_base, A);
    bi_assign(&tmp_exp, exp);

    // exp�� �� ��Ʈ�� Ȯ���ϸ� ���� ����
    for (int i = (tmp_exp->wordlen * sizeof(word) * 8) - 1; i >= 0; i--) {
        // result = result^2
        squaring_Karatsuba(&result, result);
       
        // ���� ��Ʈ�� 1�� ��� result = result * base
        if ((tmp_exp->start[i / (sizeof(word) * 8)] >> (i % (sizeof(word) * 8))) & 1) {
            msg mul_status = bi_mul(&result, result, tmp_base, 0);
            if (mul_status != SUCCESS_MUL) {
                bi_delete(&result);
                bi_delete(&tmp_base);
                bi_delete(&tmp_exp);
            }
        }
    }

    // ���� ��� ����
    bi_assign(dst, result);

    // �޸� ����
    bi_delete(&result);
    bi_delete(&tmp_base);
    bi_delete(&tmp_exp);

    return SUCCESS_MUL;
}


msg bi_right_to_left(OUT bigint** dst, IN bigint* A, IN bigint* exp) {
    if (A == NULL || exp == NULL || dst == NULL) {
        return NULL_POINTER_ERROR;
    }

    // ��� ���� �ʱ�ȭ (��� = 1�� ����)
    bigint* result = NULL;
    bi_new(&result, 1);
    result->start[0] = 1;

    // ����� base�� exp ����
    bigint* tmp_base = NULL;
    bigint* tmp_exp = NULL;
    bi_assign(&tmp_base, A);
    bi_assign(&tmp_exp, exp);

    // tmp_exp�� 0�� �ƴ� ������ �ݺ�
    while (!bi_is_zero(tmp_exp)) {
        // exp�� LSB(���� ���� ��Ʈ)�� 1�̸� result *= base
        if (tmp_exp->start[0] & 1) {
            msg mul_status = bi_mul(&result, result, tmp_base, false);
            if (mul_status != SUCCESS_MUL) {
                bi_delete(&result);
                bi_delete(&tmp_base);
                bi_delete(&tmp_exp);
                return mul_status; // ���� �� ���� �߻� �� ó��
            }
        }

        // base = base^2
        msg square_status = bi_square_C(&tmp_base, tmp_base);
        if (square_status != SUCCESS_SQUARE_A) {
            bi_delete(&result);
            bi_delete(&tmp_base);
            bi_delete(&tmp_exp);
            return square_status; // ���� �� ���� �߻� �� ó��
        }

        // exp = exp >> 1 (������ ���������� 1��Ʈ �̵�)
        bi_bit_right_shift(&tmp_exp, 1);
    }

    // ���� ��� ����
    bi_assign(dst, result);

    // �޸� ����
    bi_delete(&result);
    bi_delete(&tmp_base);
    bi_delete(&tmp_exp);

    return SUCCESS_MUL;
}

/*
msg bi_extended_euclidean(OUT bigint** gcd, OUT bigint** x, OUT bigint** y, IN bigint* A, IN bigint* B)
{
    // �ʱ�ȭ
    bigint* old_r = NULL;
    bigint* r = NULL;
    bigint* old_s = NULL;
    bigint* s = NULL;
    bigint* old_t = NULL;
    bigint* t = NULL;

    // A�� B�� �����Ͽ� ���
    bi_assign(&old_r, A); // old_r = A
    bi_assign(&r, B);     // r = B

    // �ʱ�ȭ: s, old_s
    bi_new(&old_s, 1);
    old_s->start[0] = 1; // old_s = 1
    bi_new(&s, 1);
    s->start[0] = 0;     // s = 0

    // �ʱ�ȭ: t, old_t
    bi_new(&old_t, 1);
    old_t->start[0] = 0; // old_t = 0
    bi_new(&t, 1);
    t->start[0] = 1;     // t = 1

    bigint* quotient = NULL;
    bigint* temp = NULL;

    while (!bi_is_zero(r)) {
        bigint* temp_r = NULL;
        bigint* temp_s = NULL;
        bigint* temp_t = NULL;

        // quotient = old_r / r
        bi_div(&quotient, &temp_r, old_r, r);

        // r = old_r - quotient * r
        bi_mul(&temp, quotient, r, false);
        bi_sub(&temp_r, old_r, temp);
        bi_assign(&old_r, r);
        bi_assign(&r, temp_r);

        // s = old_s - quotient * s
        bi_mul(&temp, quotient, s, false);
        bi_sub(&temp_s, old_s, temp);
        bi_assign(&old_s, s);
        bi_assign(&s, temp_s);

        // t = old_t - quotient * t
        bi_mul(&temp, quotient, t, false);
        bi_sub(&temp_t, old_t, temp);
        bi_assign(&old_t, t);
        bi_assign(&t, temp_t);

        // �޸� ����
        bi_delete(&temp_r);
        bi_delete(&temp_s);
        bi_delete(&temp_t);
    }

    // GCD, x, y �� ��ȯ
    bi_assign(gcd, old_r);
    bi_assign(x, old_s);
    bi_assign(y, old_t);

    // �޸� ����
    bi_delete(&old_r);
    bi_delete(&r);
    bi_delete(&old_s);
    bi_delete(&s);
    bi_delete(&old_t);
    bi_delete(&t);
    bi_delete(&quotient);
    bi_delete(&temp);

    return SUCCESS_MUL;
}
*/

msg bi_left_to_right_mod(bigint** dst, bigint* A, bigint* exp, bigint* mod) {
    if (A == NULL || exp == NULL || dst == NULL || mod == NULL) {
        return NULL_POINTER_ERROR;
    }

    bigint* result = NULL;
    bi_new(&result, 1);
    result->start[0] = 1;

    bigint* tmp_base = NULL;
    bigint* tmp_exp = NULL;
    bi_assign(&tmp_base, A);
    bi_assign(&tmp_exp, exp);

    for (int i = (tmp_exp->wordlen * sizeof(int) * 8) - 1; i >= 0; i--) {
        squaring_Karatsuba(&result, result);

        if ((tmp_exp->start[i / (sizeof(int) * 8)] >> (i % (sizeof(int) * 8))) & 1) {
            bi_mul(&result, result, tmp_base, 0);
          
        }

        bigint* tmp_q = NULL;
        bigint* tmp_r = NULL;
        bi_div(&tmp_q, &tmp_r, result, mod);
       

        bi_assign(&result, tmp_r);
        bi_delete(&tmp_q);
        bi_delete(&tmp_r);
    }

    bi_assign(dst, result);

    bi_delete(&result);
    bi_delete(&tmp_base);
    bi_delete(&tmp_exp);

    return SUCCESS_MUL;
}

msg bi_right_to_left_mod(OUT bigint** dst, IN bigint* A, IN bigint* exp, IN bigint* mod) {
    if (A == NULL || exp == NULL || dst == NULL || mod == NULL) {
        return NULL_POINTER_ERROR;
    }

    // ��� ���� �ʱ�ȭ (��� = 1�� ����)
    bigint* result = NULL;
    bi_new(&result, 1);
    result->start[0] = 1;

    // ����� base�� exp ����
    bigint* tmp_base = NULL;
    bigint* tmp_exp = NULL;
    bi_assign(&tmp_base, A);
    bi_assign(&tmp_exp, exp);

    // tmp_exp�� 0�� �ƴ� ������ �ݺ�
    while (!bi_is_zero(tmp_exp)) {
        // exp�� LSB(���� ���� ��Ʈ)�� 1�̸� result *= base
        if (tmp_exp->start[0] & 1) {

            bi_mul(&result, result, tmp_base, false);

            // ���� �� mod ���� ����
            bigint* tmp_q = NULL;
            bigint* tmp_r = NULL;
            bi_div(&tmp_q, &tmp_r, result, mod);
            
            // ����� �������� ������Ʈ
            bi_assign(&result, tmp_r);

            // �޸� ����
            bi_delete(&tmp_q);
            bi_delete(&tmp_r);
        }

        // base = base^2
        squaring_Karatsuba(&tmp_base, tmp_base);

        // ���� �� mod ���� ����
        bigint* tmp_q = NULL;
        bigint* tmp_r = NULL;

        bi_div(&tmp_q, &tmp_r, tmp_base, mod);
        

        // base ������Ʈ
        bi_assign(&tmp_base, tmp_r);

        // �޸� ����
        bi_delete(&tmp_q);
        bi_delete(&tmp_r);

        // exp = exp >> 1 (������ ���������� 1��Ʈ �̵�)
        bi_bit_right_shift(&tmp_exp, 1);
    }

    // ���� ��� ����
    bi_assign(dst, result);

    // �޸� ����
    bi_delete(&result);
    bi_delete(&tmp_base);
    bi_delete(&tmp_exp);

    return SUCCESS_MUL;
}

msg bi_exp_mon(bigint** dst, bigint* x, bigint* n)
{
    bigint* t0 = NULL;
    bigint* t1 = NULL;
    bi_new(&t0, 1);
    t0->start[0] = 1;
    bi_assign(&t1, x);
    int l = 0;

    for (int i = n->wordlen * sizeof(word) * 8 - 1; i > -1; i--)
    {
        int ni = (n->start[i / (sizeof(word) * 8)] >> (i % (sizeof(word) * 8)) & 1);
        if (ni == 0)
        {
            bi_mul_C(&t1, t0, t1);
            bi_square_C(&t0, t0);    //squaring �Լ��� Ȯ���Ǹ� �ٲٱ� 
        }
        else
        {
            bi_mul_C(&t0, t0, t1);
            bi_square_C(&t1, t1);    //squaring �Լ��� Ȯ���Ǹ� �ٲٱ� 
        }
    }
    bi_assign(dst, t0);
    bi_delete(&t0);
    bi_delete(&t1);
    return SUCCESS_MUL;
}


msg bi_exp_mon_mod(bigint** dst, bigint* x, bigint* n, bigint* mod)
{
    bigint* t0 = NULL;
    bigint* t1 = NULL;
    bi_new(&t0, 1);
    t0->start[0] = 1;
    bi_assign(&t1, x);
    int l = 0;

    for (int i = n->wordlen * sizeof(word) * 8 - 1; i > -1; i--)
    {
        int ni = (n->start[i / (sizeof(word) * 8)] >> (i % (sizeof(word) * 8)) & 1);
        if (ni == 0)
        {
            bi_mul_C(&t1, t0, t1);

            // ���� �� mod ���� ����
            bigint* tmp_q = NULL;
            bigint* tmp_r = NULL;
            bi_div(&tmp_q, &tmp_r, t1, mod);
           
            bi_assign(&t1, tmp_r);

            bi_square_C(&t0, t0);

            // ���� �� mod ���� ����
            bi_div(&tmp_q, &tmp_r, t0, mod);
           
            bi_assign(&t0, tmp_r);

            // �޸� ����
            bi_delete(&tmp_q);
            bi_delete(&tmp_r);
        }
        else
        {
            bi_mul_C(&t0, t0, t1);

            // ���� �� mod ���� ����
            bigint* tmp_q = NULL;
            bigint* tmp_r = NULL;
            bi_div(&tmp_q, &tmp_r, t0, mod);
           
            bi_assign(&t0, tmp_r);

            bi_square_C(&t1, t1);

            // ���� �� mod ���� ����
            bi_div(&tmp_q, &tmp_r, t1, mod);
           
            bi_assign(&t1, tmp_r);

            // �޸� ����
            bi_delete(&tmp_q);
            bi_delete(&tmp_r);
        }
    }

    bi_assign(dst, t0);
    bi_delete(&t0);
    bi_delete(&t1);

    return SUCCESS_MUL;
}

void bi_test_mul()
{
    fprintf(stdout, "1. �⺻ ���� �׽�Ʈ\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "1.1 �� ���� ���� A�� B�� ��\n");
    // Implement test logic for positive * positive

    fprintf(stdout, "1.2 �� ���� ���� A�� B�� ��\n");

    fprintf(stdout, "1.3 ���� ���� A�� ���� ���� B�� ��\n");
    // Implement test logic for positive * negative

    fprintf(stdout, "1.4 ���� ���� A�� ���� ���� B�� ��\n");
    // Implement test logic for negative * positive

    fprintf(stdout, "\n2. 0���� ���� �׽�Ʈ\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "2.1 A * 0 (����� 0)\n");
    fprintf(stdout, "2.2 0 * B (����� 0)\n");
    fprintf(stdout, "2.3 0 * 0 (����� 0)\n");

    fprintf(stdout, "\n3. �ڸ� �ø� �߻� �׽�Ʈ\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "3.1 �ڸ� �ø��� �߻��ϴ� ���\n");
    // Implement test case logic for carry

    fprintf(stdout, "3.2 �������� �ڸ� �ø�\n");
    // Implement test case logic for multiple carries
}