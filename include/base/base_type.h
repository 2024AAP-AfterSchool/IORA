#ifndef IORA_BASE_TYPE_H_
#define IORA_BASE_TYPE_H_

#include <stdint.h>
#include <stdbool.h>

#define IN
#define OUT

#define TRUE 1 ///< 참
#define FALSE 0 ///< 거짓

#define POSITIVE 0 ///< 양수
#define NEGATIVE 1 ///< 음수

typedef uint8_t byte; ///< unsigned char의 별칭. BYTE와 같은 역할.
typedef uint32_t word; ///< unsigned int의 별칭. WORD와 같은 역할. (TODO: 8, 16, 32, 64비트로 변경 가능하도록 구현)

typedef uint8_t carry;
typedef uint32_t msg; ///< unsigned int의 별칭. 메시지 코드를 저장하기 위한 변수 타입.

typedef struct 
{
    int32_t sign; ///< 부호
    int32_t wordlen; ///< 저장하고자 하는 정수의 절댓값을 저장하기 위한 최소 WORD 개수
    word* start; ///< 정수의 절댓값을 저장하기 위한 WORD 배열의 시작 주소
} bigint;

typedef struct
{
    msg message; ///< 메시지
    double time; ///< 실행 시간
} res;

#endif // IORA_BASE_TYPE_H_