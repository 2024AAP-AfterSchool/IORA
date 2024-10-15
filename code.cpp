#include<stdio.h>
#include <stdint.h>		//uint32_t를 위한 헤더파일
#include <stdlib.h>
#include <string.h>  // memcpy 사용을 위해 필요

#define NON_NEGATIVE 0
#define WordBitLen 32
typedef uint32_t msg;
// typedef unsigned int word;
typedef uint32_t word;
typedef struct {
	int		sign;
	int		word_len;
	word* a;
}big_int;
/* big_int <- array */
msg bi_set_from_array(big_int** dst, int sign, word* b, int word_len)
{
	bi_new(dst, word_len);
	(*dst)->sign = sign;
	array_copy((*dst)->a, b, word_len);
}
msg bi_set_from_string(big_int** dst, char* int_str, int base)
{
	int sign = 1;
	int str_len = strlen(int_str);
	int start_idx = 0;

	// 부호 확인 (음수인지 양수인지)
	if (int_str[0] == '-') {
		sign = -1;
		start_idx = 1;  // 부호가 있으면 다음 문자부터 처리
	}
	else if (int_str[0] == '+') {
		start_idx = 1;  // 양수 부호도 무시하고 다음 문자부터 처리
	}

	// 배열로 변환할 때 필요한 워드 길이 계산
	int word_len = (str_len - start_idx + (WordBitLen / 4) - 1) / (WordBitLen / 4);
	word* b = (word*)malloc(word_len * sizeof(word));
	memset(b, 0, word_len * sizeof(word));

	// 문자열을 숫자로 변환 (base에 따라 다른 방식으로 처리)
	for (int i = start_idx; i < str_len; i++) {
		char c = int_str[i];
		int value = 0;

		// 10진수, 16진수, 2진수 등 베이스에 맞는 값으로 변환
		if (c >= '0' && c <= '9') {
			value = c - '0';
		}
		else if (c >= 'a' && c <= 'f') {
			value = c - 'a' + 10;
		}
		else if (c >= 'A' && c <= 'F') {
			value = c - 'A' + 10;
		}
		else {
			// 잘못된 입력일 경우
			return -1;
		}

		// 변환된 값을 big_int에 저장
		for (int j = 0; j < word_len; j++) {
			b[j] *= base;
			b[j] += value;
		}
	}

	// dst에 big_int 세팅
	bi_set_from_array(dst, sign, b, word_len);

	// 할당된 메모리 해제
	free(b);

	return 0;
}
/*A[4] = {1,2,3,0}->{1,2,3,*1*} */
msg bi_get_random(big_int** dst, int sign, int word_len)
{
	bi_new(dst, word_len);
	(*dst)->sign = sign;
	array_rand((*dst)->a, word_len);

	bi_refine(*dst);
}
msg array_copy(word* dst, word* src, int word_len)
{
	memcpy(dst, src, sizeof(word) * word_len);
}
msg array_rand(word* dst, int word_len)
{
	unsigned char* p = (unsigned char*)dst;
	int cnt = word_len * sizeof(word);
	while (cnt > 0)
	{
		*p = rand() & 0xff;
		p++;
		cnt--;
	}
}

msg show_big_int(big_int* dst, int base) // Big number version with selectable base
{
	if (base == 16) { // Hexadecimal version
		printf("0x");
#if WordBitLen == 32
		for (int i = (dst->word_len) - 1; i >= 0; i--) {
			printf("%08x", dst->a[i]);
		}
#elif WordBitLen == 8
		for (int i = (dst->word_len) - 1; i >= 0; i--) {
			printf("%02x", dst->a[i]);
		}
#elif WordBitLen == 64
		for (int i = (dst->word_len) - 1; i >= 0; i--) {
			printf("%016llx", dst->a[i]);
		}
#endif
		printf("\n");
	}
	else if (base == 2) { // Binary version
		int len = dst->word_len;
		printf("0b");
		for (int i = (len - 1); i >= 0; i--) {
			word t = dst->a[i];
			for (int j = WordBitLen - 1; j >= 0; j--) {
				if ((t >> j) & 0x1)
					printf("1");
				else
					printf("0");
			}
		}
		printf("\n");
	}
}
msg bi_new(big_int** dst, int word_len) //메모리 빌림 메모리 malloc, calloc free선언은 어디서?
{
	if (*dst != NULL)
		bi_delete(dst);

	*dst = (big_int*)malloc(sizeof(big_int));
	if (*dst != NULL) {
		(*dst)->sign = NON_NEGATIVE; // 0
		(*dst)->word_len = word_len;
		(*dst)->a = (word*)calloc(word_len, sizeof(word));
	}
	else {
		printf("malloc fail");
	}
}
msg bi_delete(big_int** dst)
{
	if (*dst == NULL)
		return 0;
#ifdef ZERORIZE
	array_init((*dst)->a, (*dst)->word_len);
#endif
	free((*dst)->a);
	free(*dst);
	*dst = NULL;
	return 0;
}		//메모리 반납
msg bi_refine(big_int* dst)
{
	if (dst == NULL)
		return;
	int new_word_len = dst->word_len;
	while (new_word_len > 1)
	{
		if (dst->a[new_word_len - 1] != 0)
			break;
		new_word_len--;
	}
	if (dst->word_len != new_word_len)
	{
		dst->word_len = new_word_len;
		dst->a = (word*)realloc(dst->a, sizeof(word) * new_word_len);
	}

	if ((dst->word_len == 1) && (dst->a[0] == 0x0))
		dst->sign = NON_NEGATIVE;

}
msg bi_assign(big_int** src, big_int* dst)
{
	if (*src != NULL)
		bi_delete(src);

	bi_new(src, dst->word_len);
	(*src)->sign = dst->sign;
	memcpy((*src)->a, dst->a, dst->word_len * sizeof(word));
}
msg bi_show_hex(big_int* x) {
	if (x == NULL) {
		printf("Input is NULL\n");
		return 1;
	}

	if (x->sign == NON_NEGATIVE) {
		printf("+");
	}
	else {
		printf("-");
	}

	for (int i = x->word_len - 1; i >= 0; i--) {
		printf("%08X", x->a[i]);
		if (i > 0) {
			printf(" ");
		}
	}
	printf("\n");

	return 0;
}
msg bi_show_dec(big_int* x) {
	if (x == NULL) {
		printf("Input is NULL\n");
		return 1;
	}

	if (x->sign == NON_NEGATIVE) {
		printf("+");
	}
	else {
		printf("-");
	}

	for (int i = x->word_len - 1; i >= 0; i--) {
		printf("%u", x->a[i]);
		if (i > 0) {
			printf(" ");
		}
	}
	printf("\n");

	return 0;
}
msg bi_show_bin(big_int* x) {
	if (x == NULL) {
		printf("Input is NULL\n");
		return 1;
	}

	if (x->sign == NON_NEGATIVE) {
		printf("+");
	}
	else {
		printf("-");
	}

	for (int i = x->word_len - 1; i >= 0; i--) {
		for (int bit = sizeof(word) * 8 - 1; bit >= 0; bit--) {
			printf("%d", (x->a[i] >> bit) & 1);
		}
		if (i > 0) {
			printf(" ");
		}
	}
	printf("\n");

	return 0;
}
int main() {
	//puts("hello, world");

	return 0;
}