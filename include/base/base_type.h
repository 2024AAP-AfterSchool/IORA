#ifndef IORA_BASE_TYPE_H_
#define IORA_BASE_TYPE_H_

#include <stdint.h>
#define POSITIVE 0 ///< 양수
#define NEGATIVE 1 ///< 음수
#define word_size 32 ///< word 비트 크기


typedef uint8_t byte; ///< unsigned char의 별칭. BYTE와 같은 역할.
#if word_size == 32
typedef uint32_t word; ///< unsigned int의 별칭. WORD와 같은 역할. (TODO: 8, 16, 32, 64비트로 변경 가능하도록 구현)
#elif word_size == 64
typedef uint64_t word; ///< unsigned int의 별칭. WORD와 같은 역할. (TODO: 8, 16, 32, 64비트로 변경 가능하도록 구현)
<<<<<<< HEAD
#endif // __BASE_TYPE_H__

typedef struct 
{
    int8_t sign; ///< 부호
    int8_t wordlen; ///< 저장하고자 하는 정수의 절댓값을 저장하기 위한 최소 WORD 개수
    word* start; ///< 정수의 절댓값을 저장하기 위한 WORD 배열의 시작 주소
} bigint;

