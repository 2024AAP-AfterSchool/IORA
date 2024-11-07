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
    C = word_left_shift(&C1, 1) | C0;
}
void mul_C(bigint* x, bigint* y, bigint** z)
{
    bigint* result = NULL;
    bi_new(&result, x->wordlen + y->wordlen);
    for (int i = 0; i < (y->wordlen); i++)
    {
        bigint* T0 = NULL;
        bigint* T1 = NULL;
        bigint_create(&T0, 2); // T0는 2 워드 크기로 생성
        bigint_create(&T1, 2); // T1도 2 워드 크기로 생성1
        
        for (int j = 0; j < (x->wordlen) / 2; j++)
        {
            mul_AB(&x->start[2 * j], &y->start[i], &T0);
            word_left_shift(T0, 2 * j);
            mul_AB(&x->start[(2 * j)+1], &y->start[i], &T1);
            word_left_shift(T1, (2 * j)+1);
        }
        bigint* T = NULL;
        add(T0, T1, &T);
        word_left_shift(T, i);
        add(result, T, &result);

        // 임시 변수 삭제
        bigint_delete(&T0);
        bigint_delete(&T1);
        bigint_delete(&T);
    }
    bi_refine(z)
    bigint_assign(z, result); // 최종 결과를 z에 할당
    bigint_delete(&result); // result 삭제
}

///void karatsuba

void mul()