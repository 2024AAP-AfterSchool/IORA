import ctypes

# C언어 사용자 정의 데이터 타입
POSITIVE = 0
NEGATIVE = 1
byte = ctypes.c_uint8
word = ctypes.c_uint32
msg = ctypes.c_uint32

# C언어 사용자 정의 구조체(BigInt)
class bigint(ctypes.Structure):
    _fields_ = [
        ("sign", ctypes.c_int32),
        ("wordlen", ctypes.c_int32),
        ("start", ctypes.POINTER(word))
    ]