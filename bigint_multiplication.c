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
#include "time.h"
 /**
  * @brief �� ���� ���� A�� B�� ������ �����ϴ� �Լ�
  * @param dst ����� ������ bigint�� ������
  * @param A ������ ������ ù ��° bigint
  * @param B ������ ������ �� ��° bigint
  */
res bi_mul_AB(OUT bigint** dst, IN word* A, IN word* B)
{
    res result;
    START_TIMER();

    bigint* C = NULL;
    bi_new(&C, 2);

    word mask = (word_size == 8) ? 0xFF :
                (word_size == 32) ? 0xFFFFFFFF :
                 0xFFFFFFFFFFFFFFFF;

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
    T1 = (T0 < T1) ? 1 : 0;      //carry

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

    END_TIMER(result, print_success_mul_AB());
    return result;
}

/**
 * @brief �� bigint A�� B�� ������ �����ϴ� �Լ�
 * @param dst ����� ������ bigint�� ������
 * @param A ������ ������ ù ��° bigint
 * @param B ������ ������ �� ��° bigint
 */
res bi_mul_C(OUT bigint** dst, IN bigint* A, IN bigint* B)
{
    res result;
    START_TIMER();

    bigint* C = NULL;
    bigint* T = NULL;
    bi_new(&C, A->wordlen + B->wordlen);
    bi_new(&C, A->wordlen + 1);
    bigint* tmp_A = NULL;

    // A�� Ȧ�� ������ ��� Ȯ��
    if (A->wordlen % 2 == 1)
    {
        bi_new(&tmp_A, A->wordlen + 1);
        for (int i = 0; i < A->wordlen; i++)
        {
            tmp_A->start[i] = A->start[i];
        }
        tmp_A->start[A->wordlen] = 0;
    }
    else
    {
        bi_assign(&tmp_A, A);
    }

    // ���� �� ��� ����
    for (int i = 0; i < B->wordlen; i++)
    {
        bigint* T0 = NULL;
        bigint* T1 = NULL;
        bigint* tmp_T0 = NULL;
        bigint* tmp_T1 = NULL;
        bi_new(&T0, 2); // T0�� 2 ���� ũ��� ����
        bi_new(&T1, 2); // T1�� 2 ���� ũ��� ����
        bi_new(&tmp_T0, (tmp_A->wordlen));
        bi_new(&tmp_T1, (tmp_A->wordlen) + 1);

        for (int j = 0; j < (tmp_A->wordlen / 2); j++)
        {
            res verification_result;

            verification_result = bi_mul_AB(&T0, &(tmp_A->start[2 * j]), &(B->start[i]));
            if (verification_result.message != SUCCESS_MUL_AB)
            {
                END_TIMER(result, print_fail_mul_AB());
                return result;
            }
            tmp_T0->start[2 * j] = T0->start[0];
            tmp_T0->start[(2 * j) + 1] = T0->start[1];

            verification_result = bi_mul_AB(&T1, &(tmp_A->start[(2 * j) + 1]), &(B->start[i]));
            if (verification_result.message != SUCCESS_MUL_AB)
            {
                END_TIMER(result, print_fail_mul_AB());
                return result;
            }
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

    END_TIMER(result, print_success_mul_C());
    return result;
}

/**
 * @brief �� bigint A�� B�� ������ �����ϴ� �Լ�(ī���߹� �˰��� ���)
 * @param dst ����� ������ bigint�� ������
 * @param A ������ ������ ù ��° bigint
 * @param B ������ ������ �� ��° bigint
 */
res bi_mul_karatsuba(OUT bigint** dst, IN bigint* A, IN bigint* B)
{
    res result;
    START_TIMER();

    bigint* tmp_A = NULL;
    bigint* tmp_B = NULL;

    bi_assign(&tmp_A, A);
    bi_assign(&tmp_B, B);

    uint32_t n = (tmp_A->wordlen > B->wordlen) ? tmp_A->wordlen : tmp_B->wordlen;

    if (n <= 12)
    {
        return bi_mul(dst, tmp_A, tmp_B, 0);
    }

    uint32_t half = (n + 1) / 2;

    bigint* A1 = NULL, * A0 = NULL, * B1 = NULL, * B0 = NULL;

    bi_new(&A1, n - half);
    bi_new(&A0, half);
    for (uint32_t i = 0; i < half; i++)
    {
        A0->start[i] = (i < tmp_A->wordlen) ? tmp_A->start[i] : 0;
    }
    for (uint32_t i = half; i < n; i++)
    {
        A1->start[i - half] = (i < tmp_A->wordlen) ? tmp_A->start[i] : 0;
    }

    bi_new(&B1, n - half);
    bi_new(&B0, half);
    for (uint32_t i = 0; i < half; i++)
    {
        B0->start[i] = (i < tmp_B->wordlen) ? tmp_B->start[i] : 0;
    }
    for (uint32_t i = half; i < n; i++)
    {
        B1->start[i - half] = (i < tmp_B->wordlen) ? tmp_B->start[i] : 0;
    }

    bigint* T1 = NULL, * T0 = NULL, * T2 = NULL;
    bi_mul_karatsuba(&T1, A1, B1); // T1 = A1 * B1
    T1->sign = A1->sign ^ B1->sign;
    bi_mul_karatsuba(&T0, A0, B0); // T0 = A0 * B0
    T0->sign = A0->sign ^ B0->sign;

    //S1 = A0 - A1, S2 = B1 - B0
    bigint* S1 = NULL, * S2 = NULL;
    bi_sub(&S1, A0, A1);
    bi_sub(&S2, B1, B0);

    //T2 = S1 * S2
    bi_mul_karatsuba(&T2, S1, S2);
    T2->sign = S1->sign ^ S2->sign;

    // T2: T2 = T2 + T1 + T0
    bi_add(&T2, T2, T1);
    bi_add(&T2, T2, T0);

    bi_word_left_shift(&T1, 2 * half);
    bi_word_left_shift(&T2,  half);

    bigint* tmp_result = NULL;
    bi_add(&tmp_result, T1, T2);
    bi_add(&tmp_result, tmp_result, T0);

    tmp_result->sign = A->sign ^ B->sign;

    // Assign result to dst
    bi_assign(dst, tmp_result);


    bi_delete(&A1); bi_delete(&A0);
    bi_delete(&B1); bi_delete(&B0);
    bi_delete(&T1); bi_delete(&T0); bi_delete(&T2);
    bi_delete(&S1); bi_delete(&S2);
    bi_delete(&tmp_result);

    END_TIMER(result, SUCCESS_MUL_C);
    return result;
}

/**
 * @brief �� bigint A�� B�� ������ �����ϴ� �Լ�
 * @param dst ����� ������ bigint�� ������
 * @param A ������ ������ ù ��° bigint
 * @param B ������ ������ �� ��° bigint
 * @param is_karatsuba ī���߹� ������ ������� ����
 */
res bi_mul(OUT bigint** dst, IN bigint* A, IN bigint* B, IN bool is_karatsuba)
{
    res result;
    START_TIMER();

    bigint* tmp_A = NULL;
    bigint* tmp_B = NULL;

    bi_assign(&tmp_A, A);
    bi_assign(&tmp_B, B);

    if (bi_is_zero(tmp_A) || bi_is_zero(tmp_B))
    {
        bi_new(dst, 1);
        (*dst)->start[0] = 0;

        bi_delete(&tmp_A);
        bi_delete(&tmp_B);

        END_TIMER(result, print_success_mul());
        return result;
    }

    if (bi_is_one(tmp_A))
    {
        bi_assign(dst, tmp_B);
        (*dst)->sign = (tmp_A->sign ^ tmp_B->sign);

        bi_delete(&tmp_A);
        bi_delete(&tmp_B);

        END_TIMER(result, print_success_mul());
        return result;
    }

    if (bi_is_one(tmp_B))
    {
        bi_assign(dst, tmp_A);
        (*dst)->sign = (tmp_A->sign ^ tmp_B->sign);

        bi_delete(&tmp_A);
        bi_delete(&tmp_B);

        END_TIMER(result, print_success_mul());
        return result;
    }

    //ī���߹ٿ� ��� ���� �ְ� ��ȣ�� ���� �����ϱ� ����
    byte A_sign = tmp_A->sign;
    byte B_sign = tmp_B->sign;
    tmp_A->sign = POSITIVE;

    if (is_karatsuba) bi_mul_karatsuba(dst, tmp_A, tmp_B);
    if (!is_karatsuba) bi_mul_C(dst, tmp_A, tmp_B);

    (*dst)->sign = A_sign ^ B_sign;
    bi_delete(&tmp_A);
    bi_delete(&tmp_B);

    END_TIMER(result, print_success_mul());
    return result;
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

/**
 * @brief bigint�� ������ �����ϴ� �Լ�
 * @param dst ����� ������ bigint�� ������
 * @param A ������ ������ bigint
 */
res bi_square_AA(OUT bigint** dst, IN bigint* A)
{
    res result;
    START_TIMER();

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
    bi_word_left_shift(&C1, 1); // C1 << w

    bi_add(&C, C0, C1); // C = C1 + C0

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

    END_TIMER(result, SUCCESS_SQUARE_A);
    return result;
}

/**
 * @brief bigint�� ������ �����ϴ� �Լ�
 * @param dst ����� ������ bigint�� ������
 * @param X ������ ������ bigint
 */
res bi_square_C(OUT bigint** dst, IN bigint* X)
{
    res result;
    START_TIMER();

    bigint* C1 = NULL;
    bigint* C2 = NULL;
    bigint* T1 = NULL;
    bigint* T2 = NULL;

    // C1, C2 �ʱ�ȭ
    bi_new(&C1, X->wordlen * 2);
    bi_new(&C2, X->wordlen * 2);

    // C1 ���
    for (int j = 0; j < X->wordlen; j++)
    {
        // T1 = x[j]^2
        bigint* temp = NULL;
        bi_new(&temp, 1);
        temp->start[0] = X->start[j];
        bi_square_AA(&T1, temp);
        bi_delete(&temp);
        // T1 <<= 2 * j (���� ���� �̵�)
        bi_word_left_shift(&T1, 2 * j);
        // C1 += T1
        bi_add(&C1, C1, T1);
        bi_delete(&T1);
    }

    // C2 ���
    for (int j = 0; j < X->wordlen; j++)
    {
        for (int i = j + 1; i < X->wordlen; i++)
        {
            // T2 = x[j] * x[i]
            bigint* temp1 = NULL;
            bigint* temp2 = NULL;
            bi_new(&temp1, 1);
            bi_new(&temp2, 1);
            temp1->start[0] = X->start[j];
            temp2->start[0] = X->start[i];
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

    END_TIMER(result, SUCCESS_SQUARE_A);
    return result;
}

/**
 * @brief bigint A�� ������ �����ϴ� �Լ�(karatuba ���)
 * @param dst ����� ������ bigint�� ������
 * @param A ������ ������ bigint
 */
res bi_square_karatsuba(OUT bigint** dst, IN bigint* A)
{
    res result;
    START_TIMER();

    if (A == NULL || A->wordlen == 0)
    {
        bi_new(dst, 1); // A�� NULL�̰ų� ������� ��� 0 ��ȯ
        (*dst)->start[0] = 0;

        END_TIMER(result, SUCCESS_SQUARE_A);
        return result;
    }
    if (A->wordlen == 1)
    {
        // ���� ���� ���� ó��
        res tmp_result = bi_square_C(dst, A);
        END_TIMER(result, tmp_result.message);
        return tmp_result;
    }

    // ���� ���� ����
    uint32_t l = (A->wordlen + 1) / 2; // ���� ����
    bigint* A1 = NULL;
    bigint* A0 = NULL;
    bi_new(&A1, A->wordlen - l);
    bi_new(&A0, l);

    // ���� ����� ���� ���� �и�
    for (uint32_t i = 0; i < l; i++) \
    {
        A0->start[i] = A->start[i]; // ���� ���� ����
    }
    for (uint32_t i = l; i < A->wordlen; i++)
    {
        A1->start[i - l] = A->start[i]; // ���� ���� ����
    }

    // �߰� ��� �� �ʱ�ȭ
    bigint* T1 = NULL;
    bigint* T0 = NULL;
    bigint* R = NULL;
    bigint* S = NULL;

    // ��������� A1^2, A0^2 ���
    bi_square_karatsuba(&T1, A1); // T1 = A1^2
    bi_refine(T1);
    bi_square_karatsuba(&T0, A0); // T0 = A0^2

    // R = (T1 << 2l) + T0
    bi_refine(T0);
    bi_word_left_shift(&T1, 2 * l);
    bi_add(&R, T1, T0); // R = T1 + T0

    // S = A1 * A0
    bi_mul_karatsuba(&S, A1, A0); // S = A1 * A0
    // S <<= (l + 1)

    //bi_word_left_shift(&S,l);
    bi_bit_left_shift(&S, l * 32 + 1);

    // C = R + S
    bigint* C = NULL;
    bi_add(&C, R, S);

    // ����� dst�� ����
    bi_assign(dst, C);

    // �޸� ����
    bi_delete(&A1);
    bi_delete(&A0);
    bi_delete(&T1);
    bi_delete(&T0);
    bi_delete(&R);
    bi_delete(&S);
    bi_delete(&C);

    END_TIMER(result, SUCCESS_SQUARE_A);
    return result;
}

/**
 * @brief bigint A�� ������ �����ϴ� �Լ�
 * @param dst ����� ������ bigint�� ������
 * @param A ������ ������ bigint
 * @param is_karatsuba ī���߹� ������ ������� ����
 */
res bi_square(OUT bigint** dst, IN bigint* A, IN bool is_karatsuba)
{
    res result;
    START_TIMER();

    if (is_karatsuba) bi_square_karatsuba(dst, A);
    if (!is_karatsuba) bi_square_C(dst, A);

    END_TIMER(result, SUCCESS_SQUARE_A);
    return result;
}