
import time
a = 2640504022163
b = 12241618054908317
  
start_time = time.time()
result = a * b
end_time = time.time()
#print('a =',a)
#print('b =',b)
#print('result =', result)
execution_time = end_time - start_time
#print(f'계산 시간: {execution_time:.4f}초')

def prime_factors(n):
    """소인수분해를 수행하여 결과를 리스트로 반환"""
    factors = []
    divisor = 2  # 가장 작은 소수부터 시작
    
    while n > 1:
        while n % divisor == 0:  # 나누어 떨어지면
            factors.append(divisor)
            n //= divisor  # 몫을 n으로 갱신
        divisor += 1
        # 제곱근을 초과하는 경우 남은 n 자체가 소수
        if divisor * divisor > n:
            if n > 1:
                factors.append(n)
            break
    return factors

# 분해할 숫자
num = 32324041711768611622701029671
#print(num/12241618054908317)
start_time = time.time()#

## 소인수분해 수행
factors = prime_factors(num)#

## 종료 시간 기록
end_time = time.time()#

## 결과 출력
print("소인수분해 결과:", factors)

## 계산 시간 출력
execution_time = end_time - start_time
print(f"계산 시간: {execution_time:.10f}초")
#print(7*7*61*883407167)
#*12241618054908317
#6972261491058980795198569  12241618054908317