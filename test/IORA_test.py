import os
import ctypes
import random
import subprocess
from tqdm import tqdm
from datetime import datetime
from IORA_type import bigint, POSITIVE, NEGATIVE, word, byte, msg
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
    # ==64738==ERROR: Interceptors are not working. This may be because AddressSanitizer is loaded too late (e.g. via dlopen). Please launch the executable with:
    # DYLD_INSERT_LIBRARIES=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/16/lib/darwin/libclang_rt.asan_osx_dynamic.dylib
    # 위 문제 발생 시, 아래 명령어 수행(AddressSanitizer 사용을 위함)
    # export DYLD_INSERT_LIBRARIES=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/16/lib/darwin/libclang_rt.asan_osx_dynamic.dylib
    print_center(" 2. LOAD IORA PROJECT ", '=')

    lib = ""
    if OS == "mac":
        lib = ctypes.CDLL(f"./build/{OS}/IORA.so")
    elif OS == "linux":
        lib = ctypes.CDLL(f"./build/{OS}/IORA.so")
    elif OS == "windows":
        lib = ctypes.CDLL(f"./build/{OS}/IORA.dll")
    
    print_center(" LOAD FILE: " + lib._name, ' ')
    print_center(" LOAD SUCCESS ", '=')

    return lib

def load_function(lib):
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
    function['bi_mul'] = lib.bi_mul

    # 로드한 함수 목록
    print_center(f" LOAD FUNCTION: {list(function.keys())[:3]} ...", ' ')
    print_center(" LOAD SUCCESS ", '=')

    return function

def generate_random_number(variable_length = False, byte_length=4):
    if variable_length:
        byte_length = random.randint(0, byte_length)

    hex_number = hex(int.from_bytes(os.urandom(byte_length), byteorder='little'))

    return int(hex_number, 16)

def generate_random_wordlen(max_len):
    return random.randint(1, max_len)

def generate_random_sign():
    return random.choice([POSITIVE, NEGATIVE])

def bi_to_int(target):
    start_of_result = [target.contents.start[i] for i in range(target.contents.wordlen)]
    bigint_to_int = (-1 if target.contents.sign else 1) * int(''.join(format(x, '08X') for x in reversed(start_of_result)), 16)

    return bigint_to_int

def int_to_hex(target):
    return hex(target).upper().replace('X', 'x')

def test_addtion(function, wordlen=8, iteration=10000, verbose=False):
    print_center(" 3-8. BigInt 덧셈 테스트 ", '-', '\n', 95)

    for i in tqdm(range(iteration), desc="BigNum Addition Test", unit=" iter", ncols=100):
        sign1, sign2 = [generate_random_sign() for _ in range(2)]
        wordlen1, wordlen2 = [generate_random_wordlen(wordlen) for _ in range(2)]
        bigint1, bigint2, bigint3 = [ctypes.POINTER(bigint)() for _ in range(3)]

        src_array1 = (word * wordlen1)(*(generate_random_number() for _ in range(wordlen1)))
        src_array2 = (word * wordlen2)(*(generate_random_number() for _ in range(wordlen2)))
        
        src_num_from_array1 = ''.join(format(x, '08X') for x in reversed(src_array1))
        src_num_from_array2 = ''.join(format(x, '08X') for x in reversed(src_array2))

        result = function['bi_new'](ctypes.byref(bigint1), wordlen1)
        result = function['bi_new'](ctypes.byref(bigint2), wordlen2)
        result = function['bi_new'](ctypes.byref(bigint3), max(wordlen1, wordlen2))
        result = function['bi_set_from_array'](ctypes.byref(bigint1), sign1, wordlen1, src_array1)
        result = function['bi_set_from_array'](ctypes.byref(bigint2), sign2, wordlen2, src_array2)
        result = function['bi_add'](ctypes.byref(bigint3), bigint1, bigint2)

        c_result = bi_to_int(bigint3)
        python_result = (-1 if bigint1.contents.sign == NEGATIVE else 1) * int(src_num_from_array1, 16) \
                      + (-1 if bigint2.contents.sign == NEGATIVE else 1) * int(src_num_from_array2, 16)

        if c_result == python_result:
            if verbose:
                for j, bi in enumerate([bigint1, bigint2, bigint3]):
                    print(f"\nBigInt{j + 1}: ", end='')
                    function['bi_print'](bi, 16)
                print()
                print("C Result: ", int_to_hex(c_result))
                print("Python Result: ", int_to_hex(python_result))
                print_center(f" TEST SUCCESS {i + 1} ", '-')
            function['bi_delete'](ctypes.byref(bigint1))
            function['bi_delete'](ctypes.byref(bigint2))
            function['bi_delete'](ctypes.byref(bigint3))

        else:
            print_center(" TEST FAIL ", ' ')
            print("BigInt1: ", "-" if bigint1.contents.sign else "", "0x", src_num_from_array1, sep="")
            print("BigInt2: ", "-" if bigint2.contents.sign else "", "0x", src_num_from_array2, sep="")
            print("C Result: ", int_to_hex(c_result))
            print("Python Result: ", int_to_hex(python_result))
            print()
            print_center(f" TEST FAIL (Exit At: {i+1}) ", '-')
            exit(1)

    if iteration == i + 1:
        print()
        print_center(f" TEST SUCCESS (Iteration: {iteration}) ", '-')

def test_subtraction(function, wordlen=8, iteration=10000, verbose=False):
    print_center(" 3-9. BigInt 뺄셈 테스트 ", '-', '\n', 95)

    for i in tqdm(range(iteration), desc="BigNum Subtraction Test", unit=" iter", ncols=100):
        sign1, sign2 = [generate_random_sign() for _ in range(2)]
        wordlen1, wordlen2 = [generate_random_wordlen(wordlen) for _ in range(2)]
        bigint1, bigint2, bigint3 = [ctypes.POINTER(bigint)() for _ in range(3)]

        src_array1 = (word * wordlen1)(*(generate_random_number() for _ in range(wordlen1)))
        src_array2 = (word * wordlen2)(*(generate_random_number() for _ in range(wordlen2)))
        
        src_num_from_array1 = ''.join(format(x, '08X') for x in reversed(src_array1))
        src_num_from_array2 = ''.join(format(x, '08X') for x in reversed(src_array2))

        result = function['bi_new'](ctypes.byref(bigint1), wordlen1)
        result = function['bi_new'](ctypes.byref(bigint2), wordlen2)
        result = function['bi_new'](ctypes.byref(bigint3), max(wordlen1, wordlen2))
        result = function['bi_set_from_array'](ctypes.byref(bigint1), sign1, wordlen1, src_array1)
        result = function['bi_set_from_array'](ctypes.byref(bigint2), sign2, wordlen2, src_array2)
        result = function['bi_sub'](ctypes.byref(bigint3), bigint1, bigint2)

        c_result = bi_to_int(bigint3)
        python_result = (-1 if bigint1.contents.sign == NEGATIVE else 1) * int(src_num_from_array1, 16) \
                      - (-1 if bigint2.contents.sign == NEGATIVE else 1) * int(src_num_from_array2, 16)

        if c_result == python_result:
            if verbose:
                for j, bi in enumerate([bigint1, bigint2, bigint3]):
                    print(f"\nBigInt{j + 1}: ", end='')
                    function['bi_print'](bi, 16)
                print()
                print("C Result: ", int_to_hex(c_result))
                print("Python Result: ", int_to_hex(python_result))
                print_center(f" TEST SUCCESS {i + 1} ", '-')
            function['bi_delete'](ctypes.byref(bigint1))
            function['bi_delete'](ctypes.byref(bigint2))
            function['bi_delete'](ctypes.byref(bigint3))

        else:
            print_center(" TEST FAIL ", ' ')
            print("BigInt1: ", "-" if bigint1.contents.sign else "", "0x", src_num_from_array1, sep="")
            print("BigInt2: ", "-" if bigint2.contents.sign else "", "0x", src_num_from_array2, sep="")
            print("C Result: ", int_to_hex(c_result))
            print("Python Result: ", int_to_hex(python_result))
            print()
            print_center(f" TEST FAIL (Exit At: {i+1}) ", '-')
            exit(1)

    if iteration == i + 1:
        print()
        print_center(f" TEST SUCCESS (Iteration: {iteration}) ", '-')

def test_multiplication(function, wordlen=2, iteration=10000, verbose=True):
    print_center(" 3-10. BigInt 곱셈 테스트 ", '-', '\n', 95)

    for i in tqdm(range(iteration), desc="BigNum Multiplication Test", unit=" iter", ncols=100):
        sign1, sign2 = [generate_random_sign() for _ in range(2)]
        wordlen1, wordlen2 = [generate_random_wordlen(wordlen) for _ in range(2)]
        bigint1, bigint2, bigint3 = [ctypes.POINTER(bigint)() for _ in range(3)]

        src_array1 = (word * wordlen1)(*(generate_random_number() for _ in range(wordlen1)))
        src_array2 = (word * wordlen2)(*(generate_random_number() for _ in range(wordlen2)))
        
        src_num_from_array1 = ''.join(format(x, '08X') for x in reversed(src_array1))
        src_num_from_array2 = ''.join(format(x, '08X') for x in reversed(src_array2))

        result = function['bi_new'](ctypes.byref(bigint1), wordlen1)
        result = function['bi_new'](ctypes.byref(bigint2), wordlen2)
        result = function['bi_new'](ctypes.byref(bigint3), max(wordlen1, wordlen2))
        result = function['bi_set_from_array'](ctypes.byref(bigint1), sign1, wordlen1, src_array1)
        result = function['bi_set_from_array'](ctypes.byref(bigint2), sign2, wordlen2, src_array2)
        result = function['bi_mul'](ctypes.byref(bigint3), bigint1, bigint2, ctypes.c_bool(False))

        c_result = bi_to_int(bigint3)
        python_result = ((-1 if bigint1.contents.sign == NEGATIVE else 1) * int(src_num_from_array1, 16)) \
                      * ((-1 if bigint2.contents.sign == NEGATIVE else 1) * int(src_num_from_array2, 16))

        if c_result == python_result:
            if verbose:
                for j, bi in enumerate([bigint1, bigint2, bigint3]):
                    print(f"\nBigInt{j + 1}: ", end='')
                    function['bi_print'](bi, 16)
                print()
                print(f"C Result: {int_to_hex(c_result)}")
                print(f"Python Result: {int_to_hex(python_result)}\n")
                print_center(f" TEST SUCCESS {i + 1} ", '-')
            function['bi_delete'](ctypes.byref(bigint1))
            function['bi_delete'](ctypes.byref(bigint2))
            function['bi_delete'](ctypes.byref(bigint3))

        else:
            print("BigInt1: ", "-" if bigint1.contents.sign else "", "0x", src_num_from_array1, sep="")
            print("BigInt2: ", "-" if bigint2.contents.sign else "", "0x", src_num_from_array2, sep="")
            print("C Result: ", int_to_hex(c_result))
            print("Python Result: ", int_to_hex(python_result))
            print()
            print_center(f" TEST FAIL (Exit At: {i+1}) ", '-')
            exit(1)

    if iteration == i + 1:
        print()
        print_center(f" TEST SUCCESS (Iteration: {iteration}) ", '-')

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
    print_center(" 3-1. LOAD FUNCTIONS ", '-')
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
    print_center(" 3-4. BigInt 배열 생성 테스트 ", '-', '\n', 93)
    sign = generate_random_sign()
    wordlen = 64
    src_array = (word * wordlen)(*(generate_random_number() for _ in range(wordlen)))
    print([int_to_hex(x) for x in src_array[:min(4, wordlen)]], end=' ...\n' if wordlen > 4 else '\n')

    result = function['bi_set_from_array'](ctypes.byref(test_bigint), sign, wordlen, src_array)
    print_result(result)
    print()

    # 3-5 BigInt 랜덤 생성 테스트
    print_center(" 3-5. BigInt 랜덤 생성 테스트 ", '-', '\n', 93)
    wordlen = 8
    sign = generate_random_sign()
    result = function['bi_delete'](ctypes.byref(test_bigint))
    result = function['bi_get_random'](ctypes.byref(test_bigint), sign, wordlen)
    print_result(result)
    function['bi_print'](test_bigint, 16)
    print()

    # 3-6 BigInt 문자열 생성 테스트
    print_center(" 3-6. BigInt 문자열 생성 테스트 ", '-', '\n', 92)
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
    # test_addtion(function)

    # 3-9 BigInt 뺄셈 테스트
    # test_subtraction(function)

    # 3-10 BigInt 곱셈 테스트
    test_multiplication(function)

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