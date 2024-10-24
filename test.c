#include <stdio.h>
#include "struct.h"
#include "base.h"
#include "error.h"
#include <time.h>
# define NON_NEGATIVE 0 

/*
int main() {
    // ���� �õ� ����
    srand((unsigned int)time(NULL));  // ���� �ð��� ������� �õ� ����

    // 1. bi_new �� bi_delete �׽�Ʈ
    printf("=== Testing bi_new and bi_delete ===\n");
    bigint* num1 = NULL;
    msg result = bi_new(&num1, 5);

    if (result == SUCCESS_INITIALIZATION) {
        printf("bi_new: SUCCESS\n");
    }
    else {
        printf("bi_new: FAILED\n");
    }

    result = bi_delete(&num1);
    if (result == SUCCESS_MEMORY_DEALLOCATION) {
        printf("bi_delete: SUCCESS\n");
    }
    else {
        printf("bi_delete: FAILED\n");
    }
    printf("\n");

    // 2. bi_gen_random �׽�Ʈ
    printf("=== Testing bi_gen_random ===\n");
    bigint* num2 = NULL;
    result = bi_gen_random(&num2, NON_NEGATIVE, 5);

    if (result == SUCCESS_INITIALIZATION) {
        printf("bi_gen_random: SUCCESS\n");
    }
    else {
        printf("bi_gen_random: FAILED\n");
    }

    // ������ bigint ���
    if (num2 != NULL) {
        bi_print(num2, 16);
        printf("\n");
    }

    result = bi_delete(&num2);
    if (result == SUCCESS_MEMORY_DEALLOCATION) {
        printf("bi_delete: SUCCESS\n");
    }
    else {
        printf("bi_delete: FAILED\n");
    }
    printf("\n");

    // 3. bi_set_from_array �׽�Ʈ
    printf("=== Testing bi_set_from_array ===\n");
    word arr[5] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
    bigint* num3 = NULL;
    result = bi_set_from_array(&num3, NON_NEGATIVE, 5, arr);

    if (result == SUCCESS_INITIALIZATION) {
        printf("bi_set_from_array: SUCCESS\n");
    }
    else {
        printf("bi_set_from_array: FAILED\n");
    }

    // ������ bigint ���
    if (num3 != NULL) {
        bi_print(num3, 16);
        printf("\n");
    }

    result = bi_delete(&num3);
    if (result == SUCCESS_MEMORY_DEALLOCATION) {
        printf("bi_delete: SUCCESS\n");
    }
    else {
        printf("bi_delete: FAILED\n");
    }
    printf("\n");

    // 4. bi_assign �׽�Ʈ
    printf("=== Testing bi_assign ===\n");
    bigint* src = NULL;
    bigint* dst = NULL;

    bi_gen_random(&src, NON_NEGATIVE, 5);
    result = bi_assign(&dst, src);

    if (result == SUCCESS_INITIALIZATION) {
        printf("bi_assign: SUCCESS\n");
    }
    else {
        printf("bi_assign: FAILED\n");
    }

    // src �� dst bigint ����Ͽ� ��
    if (src != NULL) {
        printf("Source bigint: ");
        bi_print(src, 16);
        printf("\n");
    }

    if (dst != NULL) {
        printf("Assigned bigint: ");
        bi_print(dst, 16);
        printf("\n");
    }

    // �޸� ����
    bi_delete(&src);
    bi_delete(&dst);
    printf("\n");

    // 5. bi_refine �׽�Ʈ
    printf("=== Testing bi_refine ===\n");
    bigint* num4 = NULL;
    bi_gen_random(&num4, NON_NEGATIVE, 5);

    // Bigint ��� ��
    printf("Before refinement: ");
    bi_print(num4, 16);
    printf("\n");

    // ���� ��
    result = bi_refine(num4);
    if (result == SUCCESS_REFINEMENT) {
        printf("bi_refine: SUCCESS\n");
    }
    else {
        printf("bi_refine: FAILED\n");
    }

    // ������ bigint ���
    printf("After refinement: ");
    bi_print(num4, 16);
    printf("\n");

    bi_delete(&num4);

    return 0;
}

int main() {
    // ���� �õ� ����
    srand((unsigned int)time(NULL));  // ���� �ð��� ������� �õ� ����

    // 5. bi_refine �׽�Ʈ - ���� ��Ʈ�� 0�� �ִ� ��
    printf("=== Testing bi_refine (with leading zeros) ===\n");
    bigint* num4 = NULL;

    // ���� ��Ʈ�� 0�� ���Ե� �迭 ����
    word arr_with_zeros[5] = { 0x00000000, 0x00000000, 0x00000003, 0x00000000, 0x00000000 };
    bi_set_from_array(&num4, NON_NEGATIVE, 5, arr_with_zeros);

    // ���� �� ���
    printf("Before refinement: ");
    bi_print(num4, 16);
    printf("\n");

    // ���� ��
    msg result = bi_refine(num4);
    if (result == SUCCESS_REFINEMENT) {
        printf("bi_refine: SUCCESS\n");
    }
    else {
        printf("bi_refine: FAILED\n");
    }

    // ���� �� ���
    printf("After refinement: ");
    bi_print(num4, 16);
    printf("\n");

    bi_delete(&num4);

    return 0;
}
*/
/*
int main() {
    bigint* test_bigint = NULL;  // �ʱ�ȭ�� NULL�� ����
    msg result;

    // ���̽� 1: �迭�� ������ ���Ұ� 0�� �ƴ� ��� (������ �Ͼ�� �ʾƾ� ��)
    printf("Test Case 1: No refinement needed\n");
    result = bi_new(&test_bigint, 3);  // word_len�� 3���� ����
    if (result != SUCCESS_INITIALIZATION || test_bigint == NULL || test_bigint->a == NULL) {
        printf("Error: Failed to initialize bigint.\n");
        return -1;
    }

    test_bigint->a[0] = 0x12345678;
    test_bigint->a[1] = 0x87654321;
    test_bigint->a[2] = 0x11111111;  // ������ ���Ұ� 0�� �ƴ� ���
    test_bigint->sign = NON_NEGATIVE;

    printf("Before refinement: word_len = %d\n", test_bigint->word_len);
    result = bi_refine(test_bigint);
    if (result != SUCCESS_REFINE) {
        printf("Error: Refinement failed.\n");
        bi_delete(&test_bigint);
        return -1;
    }
    printf("After refinement: word_len = %d\n", test_bigint->word_len);
    bi_print(test_bigint, 16);

    bi_delete(&test_bigint);  // �޸� ����

    // ���̽� 2: �迭�� ������ ���Ұ� 0�� ��� (������ �߻��ؾ� ��)
    printf("\nTest Case 2: Refinement needed\n");
    result = bi_new(&test_bigint, 4);  // word_len�� 4�� ����
    if (result != SUCCESS_INITIALIZATION || test_bigint == NULL || test_bigint->a == NULL) {
        printf("Error: Failed to initialize bigint.\n");
        return -1;
    }

    test_bigint->a[0] = 0x12345678;
    test_bigint->a[1] = 0x87654321;
    test_bigint->a[2] = 0x00000000;  // ������ ���Ұ� 0�� ���
    test_bigint->a[3] = 0x00000000;  // �� �ٸ� 0
    test_bigint->sign = NON_NEGATIVE;

    printf("Before refinement: word_len = %d\n", test_bigint->word_len);
    result = bi_refine(test_bigint);
    if (result != SUCCESS_REFINE) {
        printf("Error: Refinement failed.\n");
        bi_delete(&test_bigint);
        return -1;
    }
    printf("After refinement: word_len = %d\n", test_bigint->word_len);
    bi_print(test_bigint, 16);

    bi_delete(&test_bigint);  // �޸� ����

    // ���̽� 3: �迭 ��ü�� 0�� ��� (��ȣ�� NON_NEGATIVE�� �����Ǿ�� ��)
    printf("\nTest Case 3: All elements are zero\n");
    result = bi_new(&test_bigint, 3);  // word_len�� 3���� ����
    if (result != SUCCESS_INITIALIZATION || test_bigint == NULL || test_bigint->a == NULL) {
        printf("Error: Failed to initialize bigint.\n");
        return -1;
    }

    test_bigint->a[0] = 0x00000000;
    test_bigint->a[1] = 0x00000000;
    test_bigint->a[2] = 0x00000000;
    test_bigint->sign = -1;  // ��ȣ�� ������ ����

    printf("Before refinement: word_len = %d, sign = %d\n", test_bigint->word_len, test_bigint->sign);
    result = bi_refine(test_bigint);
    if (result != SUCCESS_REFINE) {
        printf("Error: Refinement failed.\n");
        bi_delete(&test_bigint);
        return -1;
    }
    printf("After refinement: word_len = %d, sign = %d\n", test_bigint->word_len, test_bigint->sign);
    bi_print(test_bigint, 16);

    bi_delete(&test_bigint);  // �޸� ����

    return 0;
}
*/


int main() {
    bigint* number = NULL;  // �ݵ�� NULL�� �ʱ�ȭ
    char hex_string1[] = "0x1A2B3C4D5E6F";  // �������� 16���� ���ڿ�
    char hex_string2[] = "-0xABCDEF123456"; // ���� 16���� ���ڿ�
    char hex_string3[] = "1A2B3C";          // '0x' ���� 16���� ���ڿ�
    char hex_string4[] = "0x";              // �� 16����
    char hex_string5[] = "-0x";             // �� ���� 16����
    char invalid_hex[] = "0x1A2G3C";        // ��ȿ���� ���� ���� 'G' ����
    char long_hex[] = "0x123456789ABCDEF0123456789ABCDEF";  // �� 16���� ���ڿ�

    // �׽�Ʈ 1: �������� 16���� �Է�
    printf("Test Case 1: Valid Hex String\n");
    msg result = bi_set_from_string(&number, hex_string1, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        printf("Bigint successfully created from string.\n");
        bi_print(number, 16);  // bigint ���
    }
    else {
        printf("Failed to create bigint from string.\n");
    }
    bi_delete(&number);  // �޸� ����

    // �׽�Ʈ 2: ���� 16���� �Է�
    printf("\nTest Case 2: Negative Hex String\n");
    result = bi_set_from_string(&number, hex_string2, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        printf("Bigint successfully created from negative string.\n");
        bi_print(number, 16);  // bigint ���
    }
    else {
        printf("Failed to create bigint from negative string.\n");
    }
    bi_delete(&number);  // �޸� ����

    // �׽�Ʈ 3: '0x' ���� 16���� �Է�
    printf("\nTest Case 3: Hex String without '0x'\n");
    result = bi_set_from_string(&number, hex_string3, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        printf("Bigint successfully created from string without '0x'.\n");
        bi_print(number, 16);  // bigint ���
    }
    else {
        printf("Failed to create bigint from string without '0x'.\n");
    }
    bi_delete(&number);  // �޸� ����

    // �׽�Ʈ 4: �� 16���� �Է�
    printf("\nTest Case 4: Empty Hex String\n");
    result = bi_set_from_string(&number, hex_string4, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        printf("Bigint successfully created from empty string.\n");
        bi_print(number, 16);  // bigint ���
    }
    else {
        printf("Failed to create bigint from empty string.\n");
    }
    bi_delete(&number);  // �޸� ����

    // �׽�Ʈ 5: �� ���� 16���� �Է�
    printf("\nTest Case 5: Empty Negative Hex String\n");
    result = bi_set_from_string(&number, hex_string5, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        printf("Bigint successfully created from empty negative string.\n");
        bi_print(number, 16);  // bigint ���
    }
    else {
        printf("Failed to create bigint from empty negative string.\n");
    }
    bi_delete(&number);  // �޸� ����

    // �׽�Ʈ 6: ��ȿ���� ���� ���� ����
    printf("\nTest Case 6: Invalid Hex String (contains 'G')\n");
    result = bi_set_from_string(&number, invalid_hex, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        printf("Bigint successfully created from string with invalid character.\n");
        bi_print(number, 16);  // bigint ���
    }
    else {
        printf("Failed to create bigint from string with invalid character.\n");
    }

    // �׽�Ʈ 7: �� 16���� �Է�
    printf("\nTest Case 7: Long Hex String\n");
    result = bi_set_from_string(&number, long_hex, 16);
    if (result == SUCCESS_SET_FROM_STRING) {
        printf("Bigint successfully created from long string.\n");
        bi_print(number, 16);  // bigint ���
    }
    else {
        printf("Failed to create bigint from long string.\n");
    }
    bi_delete(&number);  // �޸� ����

    return 0;
}
