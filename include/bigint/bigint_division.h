#ifndef BIGINT_DIVISION_H_
#define BIGINT_DIVISION_H_

#include "base/base_type.h"

res bi_div_bit(OUT bigint** Q, OUT bigint** R, IN bigint* A, IN bigint* B);

res bi_div_word(OUT bigint** Q, IN bigint* A, IN bigint* B);

res bi_div_C(OUT bigint** Q, OUT bigint** R, IN bigint* A, IN bigint* B);

res bi_div_CC(OUT bigint** Q, OUT bigint** R, IN bigint* A, IN bigint* B);

res bi_div(OUT bigint** Q, OUT bigint** R, IN bigint* A, IN bigint* B);

#endif // BIGINT_MULTIPLICATION_H_