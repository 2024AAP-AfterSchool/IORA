#ifndef BIGINT_ADDITION_H
#define BIGINT_ADDITION_H

#include "struct.h"  // bigint ����ü ����
#include "base.h"    // �⺻ �Լ� �� Ÿ�� ����

// ĳ��(carry) Ÿ�� ����
typedef uint8_t carry;

// ADD_ABC �Լ�: A, B, c�� ������ �����ϰ� C�� ����, ĳ���� c_prime�� ����
void add_ABC(word A, word B, carry c, word* C, carry* c_prime);

// ADDC �Լ�: �� bigint A�� B�� ������ �����ϰ�, ����� result�� ����
void addC(bigint** result, bigint* A, bigint* B);

// ADD �Լ�: A�� B�� ������ �����ϸ�, A�� B�� ���̿� ���� ADDC�� ȣ��
void add(bigint** result, bigint* a, bigint* b);

#endif // BIGINT_ADDITION_H