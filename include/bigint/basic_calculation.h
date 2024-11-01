#include "bigint/bigint_calculation.h"

void bi_sub_ABb(word* A, word* B, byte* b, word* C);
void bi_sub_Ab(word* A, byte* b, word* C);
void bi_sub_C(bigint* A, bigint* B, bigint** z);				 
void bi_sub(bigint* x, bigint* y, bigint** z);