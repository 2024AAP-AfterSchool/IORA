/**
 * @file RSA.c
 * @brief RSA 암호화 알고리즘을 구현한 소스 파일
 * @date 2024-12-09
 * @author 김남일
 */
#include <stdio.h>
#include "utils/time.h"
#include "base/base_type.h"
#include "base/base_error.h"
#include "bigint/bigint_addition.h"
#include "bigint/bigint_subtraction.h"
#include "bigint/bigint_calculation.h"
#include "bigint/bigint_division.h"
#include "bigint/bigint_multiplication.h"
#include "bigint/bigint_exponential.h"
#include "crypto/RSA.h"

/**
 * @brief 확장 유클리드 알고리즘을 수행하는 함수
 * @param gcd 최대공약수
 * @param x x에 대한 역원
 * @param y y에 대한 역원
 * @param A bigint A
 * @param B bigint B
 */
res bi_extended_euclidean(OUT bigint** gcd, OUT bigint** x, OUT bigint** y, IN bigint* A, IN bigint* B)
{
    res result;
    START_TIMER();

    // 초기화
    bigint* old_r = NULL;
    bigint* r = NULL;
    bigint* old_s = NULL;
    bigint* s = NULL;
    bigint* old_t = NULL;
    bigint* t = NULL;

    // A와 B를 복사하여 사용
    bi_assign(&old_r, A); // old_r = A
    bi_assign(&r, B);     // r = B

    // 초기화: s, old_s
    bi_new(&old_s, 1);
    old_s->start[0] = 1; // old_s = 1
    bi_new(&s, 1);
    s->start[0] = 0;     // s = 0

    // 초기화: t, old_t
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

        // 메모리 해제
        bi_delete(&temp_r);
        bi_delete(&temp_s);
        bi_delete(&temp_t);
    }

    // GCD, x, y 값 반환
    bi_assign(gcd, old_r);
    bi_assign(x, old_s);
    bi_assign(y, old_t);

    // 메모리 해제
    bi_delete(&old_r);
    bi_delete(&r);
    bi_delete(&old_s);
    bi_delete(&s);
    bi_delete(&old_t);
    bi_delete(&t);
    bi_delete(&quotient);
    bi_delete(&temp);

    END_TIMER(result, SUCCESS_MUL);
    return result;
}

/**
 * @brief 유클리드 알고리즘을 수행하는 함수
 * @param gcd 최대공약수
 * @param A bigint A
 * @param B bigint B
 */
res bi_euclidean(OUT bigint** gcd, IN bigint* A, IN bigint* B)
{
    res result;
    START_TIMER();

    // 초기화
    bigint* old_r = NULL;
    bigint* r = NULL;
    bigint* old_s = NULL;
    bigint* s = NULL;
    bigint* old_t = NULL;
    bigint* t = NULL;

    // A와 B를 복사하여 사용
    bi_assign(&old_r, A); // old_r = A
    bi_assign(&r, B);     // r = B

    // 초기화: s, old_s
    bi_new(&old_s, 1);
    old_s->start[0] = 1; // old_s = 1
    bi_new(&s, 1);
    s->start[0] = 0;     // s = 0

    // 초기화: t, old_t
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

        // 메모리 해제
        bi_delete(&temp_r);
        bi_delete(&temp_s);
        bi_delete(&temp_t);
    }

    // GCD, x, y 값 반환
    bi_assign(gcd, old_r);

    // 메모리 해제
    bi_delete(&old_r);
    bi_delete(&r);
    bi_delete(&old_s);
    bi_delete(&s);
    bi_delete(&old_t);
    bi_delete(&t);
    bi_delete(&quotient);
    bi_delete(&temp);

    END_TIMER(result, SUCCESS_MUL);
    return result;
}

/**
 * @brief 밀러-라빈 소수 판별법을 수행하는 함수
 * @param a 밀러-라빈 테스트에서 사용할 bigint
 * @param n 밀러-라빈 테스트에서 사용할 bigint
 * @return 소수이면 1, 아니면 0
 */
res Primality_Test(bigint* IN a, bigint* IN n)
{
    res result;
    START_TIMER();

    bigint* gcd = NULL;
    bigint* one = NULL;
    bigint* two = NULL;
    bigint* len = NULL;
    bigint* n_1 = NULL;
    bigint* q = NULL;
    bigint* tmp_a = NULL;
    bigint* tmp_n = NULL;
    
    bi_assign(&tmp_a, a);
    bi_assign(&tmp_n, n);
    // GCD 계산
    bi_euclidean(&gcd, tmp_a, tmp_n);
    if (!bi_is_one(gcd)) {
        bi_delete(&gcd);
        bi_delete(&one);
        bi_delete(&two);
        bi_delete(&len);
        bi_delete(&n_1);
        bi_delete(&q);
        bi_delete(&tmp_a);
        bi_delete(&tmp_n);
        
        END_TIMER(result, FAIL_ZERO_DIV);
        return result;
    }

    // 변수 초기화
    bi_set_zero(&len);
    bi_new(&one, 1);
    one->start[0] = 1;
    bi_new(&two, 1);
    two->start[0] = 2;
    bi_sub(&n_1, tmp_n, one);
    bi_assign(&q, n_1);
    // n-1에서 2의 배수 제거

    while ((q->start[0] & 0x1) == 0) {
        bi_bit_right_shift(&q, 1);
        bi_add(&len, len, one);
    }

    uint32_t len1 = len->start[0];
    // a^q mod n 계산
    bi_exp_left_to_right_mod(&tmp_a, tmp_a, q, tmp_n);
    if (bi_is_one(tmp_a)) {
        bi_delete(&gcd);
        bi_delete(&one);
        bi_delete(&two);
        bi_delete(&len);
        bi_delete(&n_1);
        bi_delete(&q);
        bi_delete(&tmp_a);
        bi_delete(&tmp_n);
       
        END_TIMER(result, SUCCESS_RSA);
        return result;
    }

    for (uint32_t j = 0; j < len1; j++) {

        if (bi_compare(tmp_a, n_1) == 0) {
            bi_delete(&gcd);
            bi_delete(&one);
            bi_delete(&two);
            bi_delete(&len);
            bi_delete(&n_1);
            bi_delete(&q);
            bi_delete(&tmp_a);
            bi_delete(&tmp_n);
            
            END_TIMER(result, SUCCESS_RSA);
            return result;
        }
        bi_exp_left_to_right_mod(&tmp_a, tmp_a, two, n);
    }

    bi_delete(&gcd);
    bi_delete(&one);
    bi_delete(&two);
    bi_delete(&len);
    bi_delete(&n_1);
    bi_delete(&q);
    bi_delete(&tmp_a);
    bi_delete(&tmp_n);
    
    END_TIMER(result, FAIL_ZERO_DIV);
    return result;
}

/**
 * @brief RSA 암호화에 필요한 파라미터를 생성하는 함수
 * @param n RSA 암호화에 사용할 n 값
 * @param e RSA 암호화에 사용할 e 값
 * @param d RSA 암호화에 사용할 d 값
 * @param size RSA 암호화에 사용할 비트 수
 */
res RSA_parameter_create(OUT bigint** n, OUT bigint** e, OUT bigint** d, IN uint32_t size)
{
    res result;
    START_TIMER();

    bigint* p = NULL;
    bigint* q = NULL;
    bigint* a = NULL;   // Miller-Rabin 테스트에서 사용할 `a`
    bigint* tmp_e = NULL;
    bigint* tmp_d = NULL;
    bigint* tmp_c = NULL;
    bigint* pi_n = NULL;
    bigint* one = NULL;
    bigint* gcd = NULL;
    bi_new(&one, 1);
    one->start[0] = 1;
    bi_new(&a, 1);
    a->start[0] = 0x3;
    bi_new(&tmp_e, 1);
    tmp_e->start[0] = 0x10001;
    word m1 = 0;
    word m2 = 0;

    while (m1 != SUCCESS_RSA)
    {
        bigint* tmp_p = NULL;
        bi_get_random(&tmp_p, 0, size / (word_size * 2) );
        res m1 = Primality_Test(a, tmp_p);
        if (m1.message == SUCCESS_RSA)
        {
            bi_assign(&p, tmp_p);
            printf("p = ");
            bi_print(p, 16);
            bi_delete(&tmp_p);
            break;
        }
        bi_delete(&tmp_p);
    }
    while (m2 != SUCCESS_RSA)
    {

        bigint* tmp_q = NULL;
        bi_get_random(&tmp_q, 0, size / (word_size * 2) );
        res m2 = Primality_Test(a, tmp_q);
        if (m2.message == SUCCESS_RSA)
        {
            bi_assign(&q, tmp_q);
            printf("q = ");
            bi_print(q, 16);
            bi_delete(&tmp_q);
            break;
        }
        bi_delete(&tmp_q);
    }
    bi_mul(n, p, q, 1);
    bi_sub(&p, p, one);
    bi_sub(&q, q, one);
    bi_mul(&pi_n, p, q, 1);
    bi_extended_euclidean(&gcd, &tmp_d, &tmp_c, tmp_e, pi_n);

    if (!bi_is_one(gcd))
    {
        bi_delete(&p);
        bi_delete(&q);
        bi_delete(&a);
        bi_delete(&tmp_e);
        bi_delete(&tmp_d);
        bi_delete(&tmp_c);
        bi_delete(&pi_n);
        bi_delete(&one);
        bi_delete(&gcd);

        END_TIMER(result, FAIL_ZERO_DIV);
        return result; //RSA 공평한 심사를 위해 이거 실패하면 다시 돌리기
    }

    if (tmp_d->sign == 1)
    {
        bi_add(&tmp_d, tmp_d, pi_n);
    }

    bi_assign(e, tmp_e);
    bi_assign(d, tmp_d);
    bi_delete(&p);
    bi_delete(&q);
    bi_delete(&a);
    bi_delete(&tmp_e);
    bi_delete(&tmp_d);
    bi_delete(&tmp_c);
    bi_delete(&pi_n);
    bi_delete(&one);
    bi_delete(&gcd);

    END_TIMER(result, SUCCESS_RSA);
    return result;
}

/**
 * @brief RSA 암호화를 수행하는 함수
 * @param c RSA 암호화 결과
 * @param m RSA 암호화할 메시지
 * @param e RSA 암호화에 사용할 e 값
 * @param n RSA 암호화에 사용할 n 값
 */
res RSA_encrypt(OUT bigint** c, IN bigint* m, IN bigint* e, IN bigint* n)
{
    res result;
    START_TIMER();

    bi_exp_montgomery_mod(c, m, e, n);

    END_TIMER(result, SUCCESS_RSA);
    return result;
}

/**
 * @brief RSA 복호화를 수행하는 함수
 * @param m RSA 복호화 결과
 * @param c RSA 복호화할 메시지
 * @param d RSA 복호화에 사용할 d 값
 * @param n RSA 복호화에 사용할 n 값
 */
res RSA_decrypt(OUT bigint** m, IN bigint* c, IN bigint* d, IN bigint* n)
{
    res result;
    START_TIMER();

    bi_exp_montgomery_mod(m, c, d, n);
    
    END_TIMER(result, SUCCESS_RSA);
    return result;
}