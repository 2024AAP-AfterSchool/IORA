void mul_AB(word* A, word* B, bigint** z) {
    bigint* C = NULL;
    bi_new(&C, 2);

    // word ũ�⿡ ���� ����ũ ����
#if (sizeof(word)==4)
    // 32��Ʈ word�� ���
    word mask = (1U << (sizeof(word)*4)) - 1;
#else
    // 64��Ʈ word�� ���
    word mask = (1ULL << (sizeof(word)*4)) - 1;
#endif

    // A�� ������ ���� ��Ʈ ����
    word A1 = *A >> (sizeof(word) * 4);  // ���� ��Ʈ ����
    word A0 = *A & mask;             // ���� ��Ʈ ����

    // B�� ������ ���� ��Ʈ ����
    word B1 = *B >> (sizeof(word) * 4);  // ���� ��Ʈ ����
    word B0 = *B & mask;             // ���� ��Ʈ ����
    
    // A1B0+A0B1 ����ϴ� �κ�
    word T1 = A1 * B0;
    word T0 = A0 * B1;
    T0 = T1 + T0;       //A1B0+A0B1 ��ⷯ����
    T1 = T0 < T1;       //carry

    word C1 = A1 * B1;
    word C0 = A0 * B0;
    word tmp = C0;
    C0 = C0 + (T0 << (sizeof(word) * 4));
    C1 = C1 + (T1 << (sizeof(word) * 4)) | (T0 >> (sizeof(word) * 4)) + (C0 < tmp);
    C = word_left_shift(&C1, 1) + C0;
}