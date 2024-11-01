import os
import ctypes
import subprocess
from datetime import datetime
from IORA_error import result_code
from IORA_type import bigint, POSITIVE, NEGATIVE, byte, word, msg


try:
    from zoneinfo import ZoneInfo
except ImportError:
    # pip install backports.zoneinfo
    from backports.zoneinfo import ZoneInfo

# 시간대 설정
seoul_tz = ZoneInfo('Asia/Seoul')
weekday_names = ['월요일', '화요일', '수요일', '목요일', '금요일', '토요일', '일요일']

def preset():
    current_path = os.path.dirname(os.path.realpath(__file__))
    os.chdir(current_path)
    os.chdir("../")

def print_line(length=100):
    print("#" * length, end="\n\n")

def print_center(text, symbol="#", end="\n", length=100):
    print(text.center(length, symbol), end)

def print_time(time, prefix, end="\n"):
    weekday = weekday_names[time.weekday()]
    formatted_time = time.strftime(f'%Y년 %m월 %d일 {weekday} %H시 %M분 %S초')
    print_center(prefix + formatted_time, ' ', end)

def print_total_time(start_time, end_time, end="\n"):
    total_time = (end_time - start_time).total_seconds()
    total_hour = int(total_time // 3600)
    total_minute = int((total_time % 3600) // 60)
    total_second = int(total_time % 60)

    print_center(f"TOTAL TIME: {total_hour}시간 {total_minute}분 {total_second}초", ' ', end)

def print_result(result, verbose=False):
    print()
    if verbose:
        print_center(result_code[result], ' ', end="\n\n")

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

    # function['bi_sub'] = lib.bi_sub
    
    # 로드한 함수 목록
    print_center(f" LOAD FUNCTION: {list(function.keys())[:3]} ...", ' ')
    print_center(" LOAD SUCCESS ", '-')

    return function

def generate_random_number(byte_length):
    hex_number = hex(int.from_bytes(os.urandom(byte_length), byteorder='little'))

    return int(hex_number, 16)

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
    src_array = (word * wordlen)(*(generate_random_number(4) for _ in range(wordlen)))
    print([hex(x).upper().replace('X', 'x') for x in src_array[:min(4, wordlen)]], end=' ...\n' if wordlen > 4 else '\n')

    result = lib.bi_set_from_array(ctypes.byref(test_bigint), sign, wordlen, src_array)
    print_result(result)

    # 3-5 BigInt 랜덤 생성 테스트
    print_center(" 3-5. BigInt 랜덤 생성 테스트 ", '-', '\n', 95)
    wordlen = 1
    result = function['bi_delete'](ctypes.byref(test_bigint))
    result = lib.bi_get_random(ctypes.byref(test_bigint), wordlen)
    print_result(result)
    function['bi_print'](test_bigint, 16)

    # 3-6 BigInt 문자열 생성 테스트
    print_center(" 3-6. BigInt 문자열 생성 테스트 ", '-', '\n', 95)
    base = 16
    src_string = "-0x1234567890123456789012345678901234567890"
    result = lib.bi_set_from_string(ctypes.byref(test_bigint), src_string, base)
    print_result(result)



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