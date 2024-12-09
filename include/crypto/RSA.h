/**
 * @file RSA.h
 * @brief RSA 암호화 알고리즘을 구현한 헤더 파일
 * @date 2024-12-09
 */
#ifndef CRYPTO_RSA_H_
#define CRYPTO_RSA_H_

#include "base/base_type.h"

res bi_extended_euclidean(OUT bigint** gcd, OUT bigint** x, OUT bigint** y, IN bigint* A, IN bigint* B);

res bi_euclidean(OUT bigint** gcd, IN bigint* A, IN bigint* B);

res Primality_Test(bigint* IN a, bigint* IN n);

res RSA_parameter_create(OUT bigint** n, OUT bigint** e, OUT bigint** d, IN uint32_t size);

res RSA_encrypt(OUT bigint** c, IN bigint* m, IN bigint* e, IN bigint* n);

res RSA_decrypt(OUT bigint** m, IN bigint* c, IN bigint* d, IN bigint* n);

#endif // CRYPTO_RSA_H_