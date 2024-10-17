#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef struct.h
typedef uint32_t word;
typedef uint32_t msg;

typedef struct {
	int sign;
	int word_len;
	word* a;

}bigint;



// typedef bigint* pbigint; // 더블포인터를 그냥 변수로
#endif

