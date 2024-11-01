#include "bigint/basic_calculation.h"
#include "base/base_type.h"

void sub_AbB(word* A, word* B, unsigned int* b, word* C)
{
	unsigned int tmp = 0;
	*C = *A - *b;
	if (*A < *b)
		tmp = 1;
	if (*C < *B)
		tmp = 1;
	*C = *C - *B;
	*b = tmp;
}

void sub_Ab(word* A, unsigned int* b, word* C)
{
	unsigned int tmp = 0;
	*C = *A - *b;
	if (*A < *b)
		tmp = 1;
	*b = tmp;
}
void sub_C(bigint* A, bigint* B, bigint** z)
{
    unsigned int b = 0;
    word C = 0;
    bigint* sub = NULL;
    bigint_create(&sub, A->wordlen);

    for (int j = 0; j < B->wordlen; j++) 
	{
        sub_AbB(&(A->start[j]), &(B->start[j]), &b, &C);
        sub->start[j] = C;
        C = 0;
    }
	for (int j = B->wordlen; j < A->wordlen; j++) {
		sub_Ab(&(A->start[j]), &b, &C);
		sub->start[j] = C;
		C = 0;
	}
    bigint_refine(sub);
    bigint_refine(B);
    bigint_assign(z, sub);
    bigint_delete(&sub);
}

void sub(bigint* x, bigint* y, bigint** z)
{
	bigint* tmp_x = NULL;
	bigint* tmp_y = NULL;

	bigint_assign(&tmp_x, x);
	bigint_assign(&tmp_y, y);

	if (is_zero(tmp_x))
	{
		bigint_assign(z, tmp_y);
		(*z)->sign = NEGATIVE;
		bigint_delete(&tmp_x);
		bigint_delete(&tmp_y);
		return;
	}
	if (is_zero(tmp_y))
	{
		bigint_assign(z, tmp_x);
		(*z)->sign = POSITIVE;
		bigint_delete(&tmp_x);
		bigint_delete(&tmp_y);
		return;
	}
	if (compare(tmp_x, tmp_y) == 0) 
	{
		bigint_set_zero(z);
		bigint_delete(&tmp_y);
		bigint_delete(&tmp_x);
		return;
	}
	if ((tmp_x->sign == POSITIVE) && (tmp_y->sign == POSITIVE) && (compare(tmp_x, tmp_y) != -1))
	{
		sub_C(tmp_x, tmp_y, z);
		(*z)->sign = POSITIVE;
		bigint_delete(&tmp_x);
		bigint_delete(&tmp_y);
		return;
	}
	if ((tmp_x->sign == POSITIVE) && (tmp_y->sign == POSITIVE) && (compare(tmp_x, tmp_y) == -1))
	{
		sub_C(tmp_x, tmp_y, z);
		(*z)->sign = NEGATIVE;
		bigint_delete(&tmp_x);
		bigint_delete(&tmp_y);
		return;
	}
	if ((tmp_x->sign == NEGATIVE) && (tmp_y->sign == NEGATIVE) && (compare(tmp_x, tmp_y) != -1))
	{
		tmp_x->sign = POSITIVE;
		tmp_y->sign = POSITIVE;
		sub_C(tmp_y, tmp_x, z);
		bigint_delete(&tmp_x);
		bigint_delete(&tmp_y);
		return;
	}
	if ((tmp_x->sign == NEGATIVE) && (tmp_y->sign == NEGATIVE) && (compare(tmp_x, tmp_y) == -1))
	{
		tmp_x->sign = POSITIVE;
		tmp_y->sign = POSITIVE;
		sub_C(tmp_y, tmp_x, z);
		(*z)->sign = NEGATIVE;
		bigint_delete(&tmp_x);
		bigint_delete(&tmp_y);
		return;
	}
	if ((tmp_x->sign == POSITIVE) && (tmp_y->sign == NEGATIVE))
	{
		tmp_y->sign = POSITIVE;
		add(tmp_y, tmp_x, z);
		(*z)->sign = POSITIVE;
		bigint_delete(&tmp_x);
		bigint_delete(&tmp_y);
		return;
	}
	if ((tmp_x->sign == NEGATIVE) && (tmp_y->sign == POSITIVE))
	{
		tmp_x->sign = POSITIVE;
		add(tmp_x, tmp_y, z);
		(*z)->sign = NEGATIVE;
		bigint_delete(&tmp_x);
		bigint_delete(&tmp_y);
		return;
	}
}