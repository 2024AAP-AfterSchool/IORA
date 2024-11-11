void bi_mul_AB(word* A, word* B, bigint** z) {
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
void bi_mul_C(bigint* x, bigint* y, bigint** z)
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
            bi_mul_AB(&x->start[2 * j], &y->start[i], &T0);
            word_left_shift(T0, 2 * j);
            bi_mul_AB(&x->start[(2 * j)+1], &y->start[i], &T1);
            word_left_shift(T1, (2 * j)+1);
        }
        bigint* T = NULL;
        bi_add(T0, T1, &T);
        word_left_shift(T, i);
        bi_add(result, T, &result);

        // 임시 변수 삭제
        bi_delete(&T0);
        bi_delete(&T1);
        bi_delete(&T);
    }
    bi_refine(z)
    bi_assign(z, result); // 최종 결과를 z에 할당
    bi_delete(&result); // result 삭제
}

///void karatsuba

void bi_mul(bigint* x, bigint* y, bigint** z)
{
    bigint* tmp_x = NULL;
    bigint* tmp_y = NULL;

    bi_assign(&tmp_x, x);
    bi_assign(&tmp_y, y);

    if (is_zero(tmp_x) || is_zero(tmp_y))
    {
        bi_new(&z, 1);
        (*z)->start[0] = 0;
        //임시 변수들 삭제
        bi_delete(tmp_x);
        bi_delete(tmp_y);
    }
    if (is_one(tmp_x)
    {
        bi_assign(z, tmp_y);
        (*z)->sign = (tmp_x->sign ^ tmp_y->sign);
        //임시 변수들 삭제
        bi_delete(tmp_x);
        bi_delete(tmp_y);
    }
    if (is_one(tmp_y)
    {
        bi_assign(z, tmp_x);
        (*z)->sign = (tmp_x->sign ^ tmp_y->sign);
        //임시 변수들 삭제
        bi_delete(tmp_x);
        bi_delete(tmp_y);
    }
    //카라추바에 양수 곱셈 넣고 부호는 따로 저장하기 위함
    byte x_sign = tmp_x->sign;
    byte y_sign = tmp_y->sign;
    tmp_x->sign = POSITIVE;
    mul_karatsuba(tmp_x, tmp_y, z);
    (*z)->sign = x_sign ^ y_sign;

    bi_delete(&tmp_x);
    bi_delete(&tmp_x);
}

void squaring_AA(word* A, bigint** z)
{
    bigint* C = NULL;
    bi_new(&C, 2);

    // word 크기에 따른 마스크 설정
#if (sizeof(word)==4)
    // 32비트 word의 경우
    word mask = (1U << (sizeof(word) * 4)) - 1;
#else
    // 64비트 word의 경우
    word mask = (1ULL << (sizeof(word) * 4)) - 1;
#endif

    // A의 상위와 하위 비트 추출
    word A1 = *A >> (sizeof(word) * 4);  // 상위 비트 추출
    word A0 = *A & mask;             // 하위 비트 추출

    // A1B0+A0B1 계산하는 부분
    word C1= A1 * A1;
    word C0 = A0 * A0;
    C = word_left_shift(C1) | T0;       //A1B0+A0B1 모듈러덧셈
    
    bigint* T = NULL;
    bi_new(&T, 2);
    word T0 = A0 * A1;
    T->start[0] = T0 << (sizeof(word) * 4 + 1);
    T->start[1] = T0 & (mask | 1 << ((sizeof(word) + 1));
    add(C, T, &C);
    bi_assign(C, z);

    bi_delete(&C);
    bi_delete(&T)
}

void squaring_C(bigint* x, bigint** z)
{
    int t = x->wordlen;
    bigint* C1 = NULL;
    bigint* C2 = NULL;
    bigint* T1 = NULL;
    bigint* T2 = NULL;

    bi_new(&C1, 1);
    bi_new(&tmp_C2, 1);
    bi_new(&C2, 2t);
    
    for (int j = 0; j < t; j++)
    {
        squaring_AA(x->start[j], &T1);
        bi_left_shift(T1, 2 * j);
        add(C1, T1, &C1);
        for (int i = j + 1; i < t; j++)
        {
            mul_ab(&(x->start[j]), &(x->start[i]), &T2);
            word_left_shift(T2, i + j);
            add(tmp_C2, T2, &tmp_C2);
        }
    }
    //C2에 (tmp_C2<<1bit) 저장하기 위함
    for (int k = 0; k < 2t; k++)
    {
        C2[k] = (tmp_C2[k] << 1);
        tmp_C2[k] = 1 & (tmp_C2[k] >> (wordlen - 1));
        C2[k] = C2[k] | tmp_C2[k];
    }
    ADD(C1, C2, z);
    bi_delete(&C1);
    bi_delete(&tmp_C2);
    bi_delete(&T1);
    bi_delete(&T2);
}