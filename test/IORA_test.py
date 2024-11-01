import os
import ctypes
import subprocess
from datetime import datetime
from IORA_type import bigint, POSITIVE, NEGATIVE, byte, word, msg
from IORA_print import print_line, print_center, print_time, print_total_time, print_result
try:
    from zoneinfo import ZoneInfo
except ImportError:
    # pip install backports.zoneinfo
    from backports.zoneinfo import ZoneInfo

seoul_tz = ZoneInfo('Asia/Seoul')

def preset():
    current_path = os.path.dirname(os.path.realpath(__file__))
    os.chdir(current_path)
    os.chdir("../")

def build_project(OS):
    print_center(" 1. BUILD IORA PROJECT ", '=')

    if OS == "Darwin":
        OS = "mac"
        os.system(command=f"./build/{OS}/build.sh")
    elif OS == "Linux":
        OS = "linux"
        os.system(command=f"./build/{OS}/build.sh")
    elif OS == "Windows":
        OS = "windows"
        os.system(command=f"./build/{OS}/build.ps1")
    
    print()
    print_center(" BUILD SUCCESS ", '=')

    return OS

def load_library(OS):
    print_center(" 2. LOAD IORA PROJECT ", '=')

    lib = ""
    if OS == "mac":
        lib = ctypes.CDLL(f"./IORA.so")
    elif OS == "linux":
        lib = ctypes.CDLL(f"./IORA.so")
    elif OS == "windows":
        lib = ctypes.CDLL(f"./IORA.dll")
    
    print_center(" LOAD FILE: " + lib._name, ' ')
    print_center(" LOAD SUCCESS ", '=')

    return lib

def load_function(lib):
    print_center(" 3-1. LOAD FUNCTIONS ", '-')

    function = {}
    function['bi_new'] = lib.bi_new
    function['bi_delete'] = lib.bi_delete
    function['bi_set_from_array'] = lib.bi_set_from_array
    function['bi_set_from_string'] = lib.bi_set_from_string
    function['bi_get_random'] = lib.bi_get_random
    function['bi_refine'] = lib.bi_refine
    function['bi_assign'] = lib.bi_assign
    function['bi_print'] = lib.bi_print

    function['bi_add'] = lib.bi_add
    function['bi_sub'] = lib.bi_sub
    
    # 로드한 함수 목록
    print_center(f" LOAD FUNCTION: {list(function.keys())[:3]} ...", ' ')
    print_center(" LOAD SUCCESS ", '-')

    return function

def generate_random_number(byte_length=4):
    hex_number = hex(int.from_bytes(os.urandom(byte_length), byteorder='little'))

    return int(hex_number, 16)

def test_addtion(function, wordlen=64, iteration=10000, verbose=False):
    print_center(" 3-8. BigInt 덧셈 테스트 ", '-', '\n', 95)

    for i in range(iteration):
        sign = POSITIVE
        test_bigint1 = ctypes.POINTER(bigint)()
        test_bigint2 = ctypes.POINTER(bigint)()
        test_bigint3 = ctypes.POINTER(bigint)()

        src_array1 = (word * wordlen)(*(generate_random_number() for _ in range(wordlen)))
        src_array2 = (word * wordlen)(*(generate_random_number() for _ in range(wordlen)))
        
        src_num_from_array1 = ''.join(format(x, '08X') for x in reversed(src_array1))
        src_num_from_array2 = ''.join(format(x, '08X') for x in reversed(src_array2))

        result = function['bi_new'](ctypes.byref(test_bigint1), wordlen)
        result = function['bi_new'](ctypes.byref(test_bigint2), wordlen)
        result = function['bi_new'](ctypes.byref(test_bigint3), wordlen)
        result = function['bi_set_from_array'](ctypes.byref(test_bigint1), sign, wordlen, src_array1)
        result = function['bi_set_from_array'](ctypes.byref(test_bigint2), sign, wordlen, src_array2)
        result = function['bi_add'](ctypes.byref(test_bigint3), test_bigint1, test_bigint2)

        if verbose:
            print("\nBigInt1: ", end='')
            function['bi_print'](test_bigint1, 16)
            print("BigInt2: ", end='')
            function['bi_print'](test_bigint2, 16)
            print("BigInt3: ", end='')
            function['bi_print'](test_bigint3, 16)
            print()

        python_result = int(src_num_from_array1, 16) + int(src_num_from_array2, 16)

        start_of_result = [test_bigint3.contents.start[i] for i in range(test_bigint3.contents.wordlen)]
        if python_result == int(''.join(format(x, '08X') for x in reversed(start_of_result)), 16):
            if verbose:
                print(python_result, "|||| ",int(''.join(format(x, '08X') for x in reversed(start_of_result)), 16))
            function['bi_delete'](ctypes.byref(test_bigint1))
            function['bi_delete'](ctypes.byref(test_bigint2))
            function['bi_delete'](ctypes.byref(test_bigint3))

        else:
            print_center(" TEST FAIL ", ' ')
            print("BigInt1: ", "0x", src_num_from_array1, sep="")
            print("BigInt2: ", "0x", src_num_from_array2, sep="")
            print("Python Result: ", hex(python_result).upper().replace('X', 'x'))
            print("C Result: ", "" if test_bigint3.contents.sign == POSITIVE else "-",
                  hex(int(''.join(format(x, '08X') for x in reversed(start_of_result)), 16)).upper().replace('X', 'x'), sep="")
            break

    if iteration == i + 1:
        print()
        print_center(f" TEST SUCCESS (Iteration: {iteration}) ", '-')
    else:
        print()
        print_center(f" TEST FAIL (Exit At: {i+1}) ", '-')
        exit(1)

def test_subtraction(function, wordlen=64, iteration=10000, verbose=False):
    print_center(" 3-9. BigInt 뺄셈 테스트 ", '-', '\n', 95)

    for i in range(iteration):
        sign = POSITIVE
        test_bigint1 = ctypes.POINTER(bigint)()
        test_bigint2 = ctypes.POINTER(bigint)()
        test_bigint3 = ctypes.POINTER(bigint)()

        src_array1 = (word * wordlen)(*(generate_random_number() for _ in range(wordlen)))
        src_array2 = (word * wordlen)(*(generate_random_number() for _ in range(wordlen)))
        
        src_num_from_array1 = ''.join(format(x, '08X') for x in reversed(src_array1))
        src_num_from_array2 = ''.join(format(x, '08X') for x in reversed(src_array2))

        result = function['bi_new'](ctypes.byref(test_bigint1), wordlen)
        result = function['bi_new'](ctypes.byref(test_bigint2), wordlen)
        result = function['bi_new'](ctypes.byref(test_bigint3), wordlen)
        result = function['bi_set_from_array'](ctypes.byref(test_bigint1), sign, wordlen, src_array1)
        result = function['bi_set_from_array'](ctypes.byref(test_bigint2), sign, wordlen, src_array2)
        result = function['bi_sub'](test_bigint1, test_bigint2, ctypes.byref(test_bigint3))

        if verbose:
            print("\nBigInt1: ", end='')
            function['bi_print'](test_bigint1, 16)
            print("BigInt2: ", end='')
            function['bi_print'](test_bigint2, 16)
            print("BigInt3: ", end='')
            function['bi_print'](test_bigint3, 16)
            print()

        python_result = int(src_num_from_array1, 16) - int(src_num_from_array2, 16)

        start_of_result = [test_bigint3.contents.start[i] for i in range(test_bigint3.contents.wordlen)]
        if python_result == int(''.join(format(x, '08X') for x in reversed(start_of_result)), 16):
            if verbose:
                print(python_result, "|||| ",int(''.join(format(x, '08X') for x in reversed(start_of_result)), 16))
            function['bi_delete'](ctypes.byref(test_bigint1))
            function['bi_delete'](ctypes.byref(test_bigint2))
            function['bi_delete'](ctypes.byref(test_bigint3))
        else:
            print_center(" TEST FAIL ", ' ')
            print("BigInt1: ", "0x", src_num_from_array1, sep="")
            print("BigInt2: ", "0x", src_num_from_array2, sep="")
            print("Python Result: ", hex(python_result).upper().replace('X', 'x'), sep="")
            print("C Result: ", "" if test_bigint3.contents.sign == POSITIVE else "-",
                  hex(int(''.join(format(x, '08X') for x in reversed(start_of_result)), 16)).upper().replace('X', 'x'), sep="")
            break

    if iteration == i + 1:
        print()
        print_center(f" TEST SUCCESS (Iteration: {iteration}) ", '-')
    else:
        print()
        print_center(f" TEST FAIL (Exit At: {i+1}) ", '-')
        exit(1)

def test():
    OS = os.uname().sysname
    
    # 테스트 사전 설정
    preset()

    # 테스트 시작
    print_line()
    print_center(" START IORA TEST ")
    print_center(f"TEST MACHINE OS: {OS}", ' ', '')
    
    start_time = datetime.now(seoul_tz)
    print_time(start_time, "START TIME: ")
    print_line()

    # 1. 프로젝트 빌드
    OS = build_project(OS)
    
    # 2. 라이브러리 로드
    lib = load_library(OS)

    # 3. 라이브러리 테스트
    print_center(" 3. TEST LIBRARY ", '=')

    # 3-1. 함수 불러오기
    function = load_function(lib)

    # 3-2 BigInt 생성 테스트
    print_center(" 3-2. BigInt 생성 테스트 ", '-', '\n', 95)
    wordlen = 3
    test_bigint = ctypes.POINTER(bigint)()
    result = function['bi_new'](ctypes.byref(test_bigint), wordlen)
    print_result(result)

    # 3-3 BigInt 삭제 테스트
    print_center(" 3-3. BigInt 삭제 테스트 ", '-', '\n', 95)
    result = function['bi_delete'](ctypes.byref(test_bigint))
    print_result(result)

    # 3-4 BigInt 배열 생성 테스트
    print_center(" 3-4. BigInt 배열 생성 테스트 ", '-', '\n', 95)
    sign = POSITIVE
    wordlen = 64
    src_array = (word * wordlen)(*(generate_random_number() for _ in range(wordlen)))
    print([hex(x).upper().replace('X', 'x') for x in src_array[:min(4, wordlen)]], end=' ...\n' if wordlen > 4 else '\n')

    result = function['bi_set_from_array'](ctypes.byref(test_bigint), sign, wordlen, src_array)
    print_result(result)

    # 3-5 BigInt 랜덤 생성 테스트
    print_center(" 3-5. BigInt 랜덤 생성 테스트 ", '-', '\n', 95)
    wordlen = 8
    sign = POSITIVE
    result = function['bi_delete'](ctypes.byref(test_bigint))
    result = function['bi_get_random'](ctypes.byref(test_bigint), sign, wordlen)
    print_result(result)
    function['bi_print'](test_bigint, 16)
    print()

    # 3-6 BigInt 문자열 생성 테스트
    print_center(" 3-6. BigInt 문자열 생성 테스트 ", '-', '\n', 95)
    base = 16
    src_string = ctypes.c_char_p("0x123456789ABCDEF0123456789ABCDE".encode('utf-8'))
    result = function['bi_delete'](ctypes.byref(test_bigint))
    result = function['bi_set_from_string'](ctypes.byref(test_bigint), src_string, base)
    print_result(result)
    function['bi_print'](test_bigint, 16)
    print()

    # 3-7 BigInt 대입 테스트
    print_center(" 3-7. BigInt 대입 테스트 ", '-', '\n', 95)
    wordlen = 8
    sign = POSITIVE
    test_bigint1 = ctypes.POINTER(bigint)()
    test_bigint2 = ctypes.POINTER(bigint)()
    result = function['bi_get_random'](ctypes.byref(test_bigint1), sign, wordlen)
    result = function['bi_get_random'](ctypes.byref(test_bigint2), sign, wordlen)
    
    print("\nBigInt1: ", end='')
    function['bi_print'](test_bigint1, 16)
    print("BigInt2: ", end='')
    function['bi_print'](test_bigint2, 16)

    result = function['bi_assign'](ctypes.byref(test_bigint1), test_bigint2)
    print_result(result)
    
    print("BigInt1: ", end='')
    function['bi_print'](test_bigint1, 16)
    print()

    # 3-8 BigInt 덧셈 테스트
    test_addtion(function)

    # 3-9 BigInt 뺄셈 테스트
    test_subtraction(function)


    # 실행 테스트
    # os.system(command=f"./build/{OS}/IORA")

    # 테스트 종료
    print_center(" FINISH IORA TEST ")
    end_time = datetime.now(seoul_tz)
    print_time(end_time, "END TIME: ", '')

    print_total_time(start_time, end_time)
    print_line()


if __name__ == "__main__":
    test()