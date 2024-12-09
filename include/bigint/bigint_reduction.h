/**
 * @file bigint_reduction.h
 * @brief bigint에 대한 나머지 연산을 하는 함수들을 정의한 헤더 파일
 * @date 2024-12-01
 * @author 김남일
 */
#ifndef BIGINT_REDUCTION_H_
#define BIGINT_REDUCTION_H_

#include "base/base_type.h"   // 기본 함수 및 타입 정의

// MOD 함수: bigint A를 bigint n으로 나눈 나머지를 구하는 함수
res bi_mod(OUT bigint** dst, IN bigint* A, IN bigint* n, IN bigint* T);

#endif // BIGINT_REDUCTION_H_