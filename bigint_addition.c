/**
 * @file bigint_addition.c
 * @brief bigint�� ���� ���� ������ �ϴ� �Լ����� ������ �ҽ� ����
 * @date 2024-11-01
 * @author �ۿ���
 */
#include <stdio.h>
#include "base.h"
#include "error.h"
#include "bigint_subtraction.h"
#include "struct.h"

 /**
  * @brief A + B + c ������ �����ϴ� �Լ�
  * @param C ������� ������ ����
  * @param A �ǿ����� A
  * @param B �ǿ����� B
  * @param c ���� ���꿡�� �߻��� carry
  */
msg bi_add_ABC(OUT word* C, IN word A, IN word B, carry c, IN carry* c_prime)
{
    *c_prime = 0;  // �ʱ� ĳ���� 0���� ����
    *C = A + B;
    if (*C < A)
    {
        *c_prime = 1;
    }
    *C += c;
    if (*C < c)
    {
        *c_prime += 1;
    }

    return print_success_add_ABC();
}

/**
 * @brief A + B ������ �����ϴ� �Լ�
 * @param C ������� ������ ����
 * @param A �ǿ����� A
 * @param B �ǿ����� B
 */


msg bi_add_C(OUT bigint** dst, IN bigint* A, IN bigint* B) {
    if (A == NULL || A->start == NULL) {
        return NULL_POINTER_ERROR;
    }
    if (B == NULL || B->start == NULL) {
        return NULL_POINTER_ERROR;
    }

    int n = A->wordlen;
    int m = B->wordlen;

    bigint* temp = NULL;

    // dst�� A �Ǵ� B�� ������ ��� �ӽ� ������ ����� ����
    if (*dst == A || *dst == B) {
        if (bi_new(&temp, n + 1) != SUCCESS_MEMORY_ALLOCATION) {
            return MEMORY_ALLOCATION_ERROR;
        }
    }
    else {
        if (bi_new(dst, n + 1) != SUCCESS_MEMORY_ALLOCATION) {
            return MEMORY_ALLOCATION_ERROR;
        }
    }

    carry c = 0;
    word C_word;

    for (int j = 0; j < n; j++) {
        word A_word = A->start[j];
        word B_word;

        // B�� ���� ���� ������ B�� j��° ���带 �Ҵ��ϰ�, �ʰ��ϸ� 0���� ����
        if (j < m) {
            B_word = B->start[j];
        }
        else {
            B_word = 0;
        }

        // bi_add_ABC ȣ��
        bi_add_ABC(A_word, B_word, c, &C_word, &c);

        // ��� ����
        if (*dst == A || *dst == B) {
            temp->start[j] = C_word;
        }
        else {
            (*dst)->start[j] = C_word;
        }
    }

    if (*dst == A || *dst == B) {
        temp->start[n] = c;
        if (c == 0) {
            temp->wordlen = n;
        }
        else {
            temp->wordlen = n + 1;
        }
        bi_refine(temp);
        bi_assign(dst, temp);  // ���� ����� dst�� ����
        bi_delete(&temp);       // �ӽ� ���� ����
    }
    else {
        (*dst)->start[n] = c;
        if (c == 0) {
            (*dst)->wordlen = n;
        }
        else {
            (*dst)->wordlen = n + 1;
        }
        bi_refine(*dst);
    }

    return print_success_add_C();
}

/**
 * @brief �� bigint�� ������ �����ϴ� �Լ�
 * @param result ������� ������ ����
 * @param a �ǿ����� a
 * @param b �ǿ����� b
 */
msg bi_add(OUT bigint** dst, IN bigint* A, IN bigint* B)
{
    if ((A->sign == 0) && (B->sign == 1))
    {
        bi_sub(dst, A, B);
    }
    if ((B->sign == 0) && (A->sign == 1))
    {
        bi_sub(dst, B, A);
    }
    if ((B->sign == 1) && (A->sign == 1))
    {
        if (A->wordlen >= B->wordlen)
        {
            bi_addC(dst, A, B);
            (*dst)->sign = 1;
        }
        else
        {
            bi_addC(dst, B, A);
            (*dst)->sign = 1;
        }
    }
    else
    {
        if (A->wordlen >= B->wordlen)
        {
            bi_add_C(dst, A, B);
        }
        else
        {
            bi_add_C(dst, B, A);
        }
    }

    return print_success_add();
}

/**
 * @brief bigint ���� �׽�Ʈ �Լ�
 */
msg bi_test_add()
{

    fprintf(stdout, "===========================\n");
    fprintf(stdout, "BigInt ���� �׽�Ʈ ����\n");
    fprintf(stdout, "===========================\n");

    fprintf(stdout, "\n1. �Ϲ����� ���� �׽�Ʈ\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "1.1 �� ��� A�� B�� ���ϴ� ���\n");
    bigint* A = NULL;
    bigint* B = NULL;
    bigint* dst = NULL;

    bi_get_random(&A, POSITIVE, 3);
    bi_get_random(&B, POSITIVE, 3);
    bi_add(&dst, A, B);

    fprintf(stdout, "1.2 �� ���� A�� B�� ���ϴ� ���\n");
    // Implement test case logic for negative addition

    fprintf(stdout, "1.3 �ϳ��� ����� �ϳ��� ���� ���� (����� ���)\n");
    // Implement test case logic for mixed sign (positive result)

    fprintf(stdout, "1.4 �ϳ��� ����� �ϳ��� ���� ���� (����� ����)\n");
    // Implement test case logic for mixed sign (negative result)

    fprintf(stdout, "\n2. �ڸ� �ø� �߻� �׽�Ʈ\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "2.1 �ڸ� �ø��� �߻��ϴ� ���\n");
    // Implement test case logic for carry

    fprintf(stdout, "2.2 �������� �ڸ� �ø�\n");
    // Implement test case logic for consecutive carries

    fprintf(stdout, "\n3. �ٸ� ������ bigint ���� �׽�Ʈ\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "3.1 A�� ���̰� B���� �� ���\n");
    // Implement test case logic for A longer than B

    fprintf(stdout, "3.2 A�� ���̰� B���� ª�� ���\n");
    // Implement test case logic for B longer than A

    fprintf(stdout, "3.3 A�� B�� ���̰� ���� ���\n");
    // Implement test case logic for equal length

    fprintf(stdout, "\n4. ��� ���� �׽�Ʈ\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "4.1 A �Ǵ� B�� NULL�� ���\n");
    // Implement test case logic for NULL pointers

    fprintf(stdout, "4.2 A�� B �� �ϳ� �Ǵ� �� �� ���̰� 0�� ���\n");
    // Implement test case logic for zero-length

    fprintf(stdout, "4.3 A�� B�� ��� 0�� ���\n");
    // Implement test case logic for both zero

    fprintf(stdout, "4.4 A �Ǵ� B�� 0x0���� ������ ���\n");
    // Implement test case logic for single 0 word

    fprintf(stdout, "\n5. �޸� ���� �׽�Ʈ\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "5.1 �޸� �Ҵ� ����\n");
    // Implement test case logic for memory allocation failure

    fprintf(stdout, "5.2 dst�� ��� �ְų� �ʱ�ȭ���� ���� ���\n");
    // Implement test case logic for uninitialized dst

    fprintf(stdout, "\n6. ������ �� �� ĳ�� �׽�Ʈ\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "6.1 ������ ��\n");
    // Implement test case logic for arbitrary values

    fprintf(stdout, "6.2 ���� ĳ���� 1�� ���¿��� ����\n");
    // Implement test case logic for carry 1

    fprintf(stdout, "\n7. bi_refine ��� Ȯ��\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "7.1 ���� ���尡 0�� ��, bi_refine �� wordlen Ȯ��\n");
    // Implement test case logic for bi_refine

    fprintf(stdout, "7.2 �ڸ� �ø����� ���� wordlen ���� Ȯ��\n");
    // Implement test case logic for wordlen increase

    fprintf(stdout, "\n8. ���� ���� �׽�Ʈ\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "8.1 1��Ʈ ������ ������ ����\n");
    // Implement test case logic for binary addition

    fprintf(stdout, "\n9. Ư�� ���̽�\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "9.1 A�� �ִ� ���� ���̰� B�� 1�� ���\n");
    // Implement test case logic for max int + 1

    fprintf(stdout, "9.2 A�� �ּ� ���� ���̰� B�� ������ ���\n");
    // Implement test case logic for min int + negative

    fprintf(stdout, "\n10. ���� ���� ���� �׽�Ʈ\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "10.1 ���� ����� ������ A�� B�� ����\n");
    // Implement test case logic for single-word addition

    fprintf(stdout, "10.2 c�� 0�� ���� 1�� ���\n");
    // Implement test case logic for carry 0 and 1

    fprintf(stdout, "\n11. ��ø�� ���� �׽�Ʈ\n");
    fprintf(stdout, "---------------------------\n");
    fprintf(stdout, "11.1 ���� �� ��ø�� bi_add ȣ��\n");
    // Implement test case logic for nested addition

    fprintf(stdout, "\n===========================\n");
    fprintf(stdout, "BigInt ���� �׽�Ʈ �Ϸ�\n");
    fprintf(stdout, "===========================\n");
}