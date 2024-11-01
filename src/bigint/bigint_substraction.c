#include "bigint/basic_calculation.h"
#include "base/base_type.h"

void bi_sub_AbB(word* A, word* B, byte* b, word* C)
{
	byte tmp = 0;
	*C = *A - *b;
	if (*A < *b)
		tmp = 1;
	if (*C < *B)
		tmp = 1;
	*C = *C - *B;
	*b = tmp;
}

void bi_sub_Ab(word* A, byte* b, word* C)
{
	byte tmp = 0;
	*C = *A - *b;
	if (*A < *b)
		tmp = 1;
	*b = tmp;
}
void bi_sub_C(bigint* A, bigint* B, bigint** z)
{
    byte b = 0;
    word C = 0;
    bigint* bi_sub = NULL;
    bi_new(&bi_sub, A->wordlen);

    for (int j = 0; j < B->wordlen; j++) 
	{
        bi_sub_AbB(&(A->start[j]), &(B->start[j]), &b, &C);
        bi_sub->start[j] = C;
        C = 0;
    }
	for (int j = B->wordlen; j < A->wordlen; j++) {
		bi_sub_Ab(&(A->start[j]), &b, &C);
		bi_sub->start[j] = C;
		C = 0;
	}
    bi_refine(bi_sub);
    bi_refine(B);
    bi_assign(z, bi_sub);
    bi_delete(&bi_sub);
}

void bi_sub(bigint* x, bigint* y, bigint** z)
{
	bigint* tmp_x = NULL;
	bigint* tmp_y = NULL;

	bi_assign(&tmp_x, x);
	bi_assign(&tmp_y, y);

	if (is_zero(tmp_x))
	{
		bi_assign(z, tmp_y);
		(*z)->sign = NEGATIVE;
		bi_delete(&tmp_x);
		bi_delete(&tmp_y);
		return;
	}
	if (is_zero(tmp_y))
	{
		bi_assign(z, tmp_x);
		(*z)->sign = POSITIVE;
		bi_delete(&tmp_x);
		bi_delete(&tmp_y);
		return;
	}
	if (compare(tmp_x, tmp_y) == 0) 
	{
		bi_set_zero(z);
		bi_delete(&tmp_y);
		bi_delete(&tmp_x);
		return;
	}
	if ((tmp_x->sign == POSITIVE) && (tmp_y->sign == POSITIVE) && (compare(tmp_x, tmp_y) != -1))
	{
		bi_sub_C(tmp_x, tmp_y, z);
		(*z)->sign = POSITIVE;
		bi_delete(&tmp_x);
		bi_delete(&tmp_y);
		return;
	}
	if ((tmp_x->sign == POSITIVE) && (tmp_y->sign == POSITIVE) && (compare(tmp_x, tmp_y) == -1))
	{
		bi_sub_C(tmp_x, tmp_y, z);
		(*z)->sign = NEGATIVE;
		bi_delete(&tmp_x);
		bi_delete(&tmp_y);
		return;
	}
	if ((tmp_x->sign == NEGATIVE) && (tmp_y->sign == NEGATIVE) && (compare(tmp_x, tmp_y) != -1))
	{
		tmp_x->sign = POSITIVE;
		tmp_y->sign = POSITIVE;
		bi_sub_C(tmp_y, tmp_x, z);
		bi_delete(&tmp_x);
		bi_delete(&tmp_y);
		return;
	}
	if ((tmp_x->sign == NEGATIVE) && (tmp_y->sign == NEGATIVE) && (compare(tmp_x, tmp_y) == -1))
	{
		tmp_x->sign = POSITIVE;
		tmp_y->sign = POSITIVE;
		bi_sub_C(tmp_y, tmp_x, z);
		(*z)->sign = NEGATIVE;
		bi_delete(&tmp_x);
		bi_delete(&tmp_y);
		return;
	}
	if ((tmp_x->sign == POSITIVE) && (tmp_y->sign == NEGATIVE))
	{
		tmp_y->sign = POSITIVE;
		add(tmp_y, tmp_x, z);
		(*z)->sign = POSITIVE;
		bi_delete(&tmp_x);
		bi_delete(&tmp_y);
		return;
	}
	if ((tmp_x->sign == NEGATIVE) && (tmp_y->sign == POSITIVE))
	{
		tmp_x->sign = POSITIVE;
		add(tmp_x, tmp_y, z);
		(*z)->sign = NEGATIVE;
		bi_delete(&tmp_x);
		bi_delete(&tmp_y);
		return;
	}
}