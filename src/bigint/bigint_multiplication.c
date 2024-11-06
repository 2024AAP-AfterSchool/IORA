void mul_AB(word* A, word* B, bigint** z) {
    bigint* C = NULL;
    bi_new(&C, 2);

    // word 크기에 따른 마스크 설정
#if (sizeof(word)==4)
    // 32비트 word의 경우
    word mask = (1U << (sizeof(word)*4)) - 1;
#else
    // 64비트 word의 경우
    word mask = (1ULL << (sizeof(word)*4)) - 1;
#endif

    // A의 상위와 하위 비트 추출
    word A1 = *A >> (sizeof(word) * 4);  // 상위 비트 추출
    word A0 = *A & mask;             // 하위 비트 추출

    // B의 상위와 하위 비트 추출
    word B1 = *B >> (sizeof(word) * 4);  // 상위 비트 추출
    word B0 = *B & mask;             // 하위 비트 추출
    
    // A1B0+A0B1 계산하는 부분
    word T1 = A1 * B0;
    word T0 = A0 * B1;
    T0 = T1 + T0;       //A1B0+A0B1 모듈러덧셈
    T1 = T0 < T1;       //carry

    word C1 = A1 * B1;
    word C0 = A0 * B0;
    word tmp = C0;
    C0 = C0 + (T0 << (sizeof(word) * 4));
    C1 = C1 + (T1 << (sizeof(word) * 4)) | (T0 >> (sizeof(word) * 4)) + (C0 < tmp);
    C = word_left_shift(&C1, 1) + C0;
}