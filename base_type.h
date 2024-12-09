#ifndef IORA_BASE_TYPE_H_
#define IORA_BASE_TYPE_H_

#include <stdint.h>
#include <stdbool.h>

#define IN
#define OUT

#define TRUE 1 ///< ��
#define FALSE 0 ///< ����

#define POSITIVE 0 ///< ���
#define NEGATIVE 1 ///< ����

#define word_size 8


typedef uint8_t byte; ///< unsigned char�� ��Ī. BYTE�� ���� ����.

#if word_size == 8
typedef uint8_t word; ///< unsigned int�� ��Ī. WORD�� ���� ����. (TODO: 8, 16, 32, 64��Ʈ�� ���� �����ϵ��� ����)

#elif word_size == 32
typedef uint32_t word;
#elif word_size == 64
typedef uint64_t word;
#endif


typedef uint8_t carry;
typedef uint32_t msg; ///< unsigned int�� ��Ī. �޽��� �ڵ带 �����ϱ� ���� ���� Ÿ��.

typedef struct
{
    int32_t sign; ///< ��ȣ
    int32_t wordlen; ///< �����ϰ��� �ϴ� ������ ������ �����ϱ� ���� �ּ� WORD ����
    word* start; ///< ������ ������ �����ϱ� ���� WORD �迭�� ���� �ּ�
} bigint;

typedef struct
{
    msg message; ///< �޽���
    double time; ///< ���� �ð�
} res;

#endif // IORA_BASE_TYPE_H_