import os
import gc
import psutil
import timeit
import ctypes
import random
import platform
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from tqdm import tqdm
from datetime import datetime
from IORA_error import result_code
from IORA_type import bigint, POSITIVE, NEGATIVE, word, res
from IORA_print import print_line, print_center, print_time, print_total_time, print_result

try:
    from zoneinfo import ZoneInfo
except ImportError:
    # pip install backports.zoneinfo
    from backports.zoneinfo import ZoneInfo

seoul_tz = ZoneInfo('Asia/Seoul')

INTENAL_ITERATION = 1

def preset():

    gc.disable()

    current_path = os.path.dirname(os.path.realpath(__file__))
    os.chdir(current_path)
    os.chdir("../")

def build_project(OS):
    print_center(" 1. BUILD IORA PROJECT ", '=')

    pid = os.getpid()
    p = psutil.Process(pid)

    if OS == "Darwin":
        OS = "mac"
        os.system(command=f"./build/{OS}/build.sh")
        p.nice(20)
    elif OS == "Linux":
        OS = "linux"
        os.system(command=f"./build/{OS}/build.sh")
        p.cpu_affinity([0])
    elif OS == "Windows":
        OS = "windows"
        os.system(command=f"./build/{OS}/build.ps1")
        p.cpu_affinity([0])
    
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
    if OS == 'mac' or OS == 'linux':
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
    function['bi_square'] = lib.bi_square
    function['bi_div_bit'] = lib.bi_div_bit
    function['bi_div'] = lib.bi_div
    function['bi_bit_left_shift'] = lib.bi_bit_left_shift
    function['bi_exp_left_to_right'] = lib.bi_exp_left_to_right
    function['bi_exp_right_to_left'] = lib.bi_exp_right_to_left
    function['bi_exp_montgomery'] = lib.bi_exp_montgomery
    function['bi_exp_left_to_right_mod'] = lib.bi_exp_left_to_right_mod
    function['bi_exp_right_to_left_mod'] = lib.bi_exp_right_to_left_mod
    function['bi_exp_montgomery_mod'] = lib.bi_exp_montgomery_mod
    function['bi_mod'] = lib.bi_mod
    
    # 함수 반환 타입 설정
    for key in function.keys():
        function[key].restype = res

    # 로드한 함수 목록
    print_center(f" LOAD FUNCTION: {list(function.keys())[:3]} ...", ' ')
    print_center(" LOAD SUCCESS ", '=')

    return function

def generate_random_number(variable_length = False, byte_length=1):
    if variable_length:
        byte_length = random.randint(0, byte_length)

    hex_number = hex(int.from_bytes(os.urandom(byte_length), byteorder='little'))

    return int(hex_number, 16)

def generate_random_wordlen(max_len):
    return random.randint(1, max_len)
    # return max_len

def generate_random_sign():
    return random.choice([POSITIVE, NEGATIVE])

def bi_to_int(target):
    start_of_result = [target.contents.start[i] for i in range(target.contents.wordlen)]
    bigint_to_int = (-1 if target.contents.sign else 1) * int(''.join(format(x, '02X') for x in reversed(start_of_result)), 16)

    return bigint_to_int

def int_to_hex(target):
    return hex(target).upper().replace('X', 'x')

def drow_and_save_graph(data, title, filename):
    
    execution_times = [round(x, 6) for x in data]
    avg_time = round(np.mean(execution_times), 6)
    max_time, min_time = max(execution_times), min(execution_times)
    max_index, min_index = execution_times.index(max_time), execution_times.index(min_time)
    
    upper_99 = np.percentile(execution_times, 99)
    lower_99 = np.percentile(execution_times, 1)

    # 그래프 생성
    fig, ax = plt.subplots(figsize=(12, 10))

    # 산점도
    ax.scatter(range(len(execution_times)), execution_times, color='#BFECFF', alpha=1, s=25, edgecolors='#555555', linewidths=0.7)

    # 최고/최저 시간 및 평균선 표시
    ax.scatter(max_index, max_time, color='#D91656', label='Worst Case', zorder=5, marker='*', alpha=1, s=100)
    ax.scatter(min_index, min_time, color='#006A67', label='Best Case', zorder=5, marker='*', alpha=1, s=100)
    ax.axhline(y=avg_time, color='#FFB200', linestyle='--', label='Average', linewidth=2, alpha=1)

    # 상위 98%와 하위 98% 점선
    ax.axhline(y=upper_99, color='#F7418F', linestyle='--', label='99th Percentile', linewidth=2, alpha=1)
    ax.axhline(y=lower_99, color='#116A7B', linestyle='--', label='1th Percentile', linewidth=2, alpha=1)

    # 그래프 디자인
    ax.set_title(f"{title} Execution Time Graph", fontsize=16, fontweight='bold', color='#333333')
    ax.set_xlabel("Iteration Index", fontsize=12, color='#333333',
                bbox=dict(facecolor='#EEEEEE', edgecolor='#000000', alpha=0.7))
    ax.set_ylabel("Execution Time (ms)", fontsize=12, color='#333333',
                bbox=dict(facecolor='#EEEEEE', edgecolor='#000000', alpha=0.7))
    ax.xaxis.set_major_formatter(ticker.ScalarFormatter(useMathText=False))
    ax.xaxis.get_offset_text().set_visible(False)
    ax.yaxis.set_major_formatter(plt.FuncFormatter(lambda x, _: f'{x:.6f}'))
    ax.grid(color='#AAAAAA', linestyle='--', linewidth=0.7, alpha=0.7)
    ax.legend(loc='upper right', fontsize=10, frameon=False)
    
    # 텍스트 설정
    fig.text(0.065, 0.99, f"Worst Case: {max_time:.6f} ms (Index: {max_index})", fontsize=12, ha='left', va='top', color='#D91656',
            bbox=dict(facecolor='#FFE6EA', edgecolor='#D91656', alpha=0.5))
    fig.text(0.065, 0.95, f"Best Case: {min_time:.6f} ms (Index: {min_index})", fontsize=12, ha='left', va='top', color='#006A67',
            bbox=dict(facecolor='#E6F9F8', edgecolor='#006A67', alpha=0.5))
    fig.text(0.065, 0.91, f"Average Time: {avg_time:.6f} ms", fontsize=12, ha='left', va='top', color='#000000',
            bbox=dict(facecolor='#FFF7E6', edgecolor='#FFB200', alpha=0.5))
    fig.text(0.065, 0.87, f"99th Percentile: {upper_99:.6f} ms", fontsize=12, ha='left', va='top', color='#F7418F',
            bbox=dict(facecolor='#FFE6F1', edgecolor='#F7418F', alpha=0.5))
    fig.text(0.065, 0.83, f"1th Percentile: {lower_99:.6f} ms", fontsize=12, ha='left', va='top', color='#116A7B',
            bbox=dict(facecolor='#E6F7F0', edgecolor='#006A67', alpha=0.5))

    # 그래프 간격 조정
    plt.subplots_adjust(top=0.8)

    # 그래프 저장
    output_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), "graph", filename)
    plt.savefig(output_path, format='png', dpi=450)
    print(f"Graph saved to {output_path}\n")

def test_addtion(function, wordlen=8, iteration=100, verbose=False):
    print_center(" 3-8. BigInt 덧셈 테스트 ", '-', '\n', 95)

    test_cases = []
    sign1, sign2 = [generate_random_sign() for _ in range(2)]
    for i in tqdm(range(iteration), desc="BigNum Addition Test", unit=" iter", ncols=100):

        src_array1 = (word * wordlen)(*(generate_random_number() for _ in range(wordlen)))
        src_array2 = (word * wordlen)(*(generate_random_number() for _ in range(wordlen)))

        # sign1, sign2 = [generate_random_sign() for _ in range(2)]
        
        wordlen1, wordlen2 = [generate_random_wordlen(wordlen) for _ in range(2)]
        bigint1, bigint2, bigint3 = [ctypes.POINTER(bigint)() for _ in range(3)]

        # src_array1 = (word * wordlen1)(*(generate_random_number() for _ in range(wordlen1)))
        # src_array2 = (word * wordlen2)(*(generate_random_number() for _ in range(wordlen2)))

        src_num_from_array1 = ''.join(format(x, '02X') for x in reversed(src_array1))
        src_num_from_array2 = ''.join(format(x, '02X') for x in reversed(src_array2))

        result = function['bi_new'](ctypes.byref(bigint1), wordlen1)
        result = function['bi_new'](ctypes.byref(bigint2), wordlen2)
        result = function['bi_new'](ctypes.byref(bigint3), max(wordlen1, wordlen2))
        result = function['bi_set_from_array'](ctypes.byref(bigint1), sign1, wordlen1, src_array1)
        result = function['bi_set_from_array'](ctypes.byref(bigint2), sign2, wordlen2, src_array2)

        python_result = (-1 if bigint1.contents.sign == NEGATIVE else 1) * int(src_num_from_array1, 16) \
                      + (-1 if bigint2.contents.sign == NEGATIVE else 1) * int(src_num_from_array2, 16)
        
        internal_execution_times = []
        for j in range(INTENAL_ITERATION):

            result = function['bi_add'](ctypes.byref(bigint3), bigint1, bigint2)
            internal_execution_times.append(result.time)

            c_result = bi_to_int(bigint3)
            if c_result != python_result:
                print()
                print_center(" TEST FAIL ", '-')
                print("BigInt1: ", "-" if bigint1.contents.sign else "", "0x", src_num_from_array1, sep="")
                print("BigInt2: ", "-" if bigint2.contents.sign else "", "0x", src_num_from_array2, sep="")
                print("C Result: ", int_to_hex(c_result))
                print("Python Result: ", int_to_hex(python_result))
                print()
                print_center(f" TEST FAIL (Exit At: {i+1}) ", '-')
                exit(1)

        test_cases.append({
            'execution_times': np.mean(internal_execution_times),
            'bigint1': int_to_hex(bi_to_int(bigint1)),
            'bigint2': int_to_hex(bi_to_int(bigint2)),
            'bigint3': int_to_hex(bi_to_int(bigint3)),
        })

        execution_times = [test_case['execution_times'] for test_case in test_cases]

        if verbose:
            print()
            for j, bi in enumerate([bigint1, bigint2, bigint3]):
                print(f"\nBigInt{j + 1}: ", end='')
                function['bi_print'](bi, 16)
            print("C Result: ", int_to_hex(c_result))
            print("Python Result: ", int_to_hex(python_result))
            print_center(f" TEST SUCCESS {i + 1} ", '-')

    function['bi_delete'](ctypes.byref(bigint1))
    function['bi_delete'](ctypes.byref(bigint2))
    function['bi_delete'](ctypes.byref(bigint3))

    if iteration == i + 1:
        print()
        print_center(f" TEST SUCCESS (Iteration: {iteration}) ", '-')
        drow_and_save_graph(data=execution_times, title="Bigint Addition", filename="1.bi_addition_execution_time.png")

        worst_case_index = np.argmax(execution_times)
        best_case_index = np.argmin(execution_times)

        print(f"worst_case_index: {worst_case_index}")
        print(f"best_case_index: {best_case_index}\n")

        print("Worst case:")
        print(f"Execution time: {execution_times[worst_case_index]:.6f}")
        print(f"Bigint1: {test_cases[worst_case_index]['bigint1']}")
        print(f"Bigint2: {test_cases[worst_case_index]['bigint2']}")
        print(f"Bigint3: {test_cases[worst_case_index]['bigint3']}")
        print()

        print("\nBest case:")
        print(f"Execution time: {execution_times[best_case_index]:.6f}")
        print(f"Bigint1: {test_cases[best_case_index]['bigint1']}")
        print(f"Bigint2: {test_cases[best_case_index]['bigint2']}")
        print(f"Bigint3: {test_cases[best_case_index]['bigint3']}")
        print()

def test_subtraction(function, wordlen=64, iteration=1000, verbose=False):
    print_center(" 3-9. BigInt 뺄셈 테스트 ", '-', '\n', 95)

    execution_times = []
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
        python_result = (-1 if bigint1.contents.sign == NEGATIVE else 1) * int(src_num_from_array1, 16) \
                      - (-1 if bigint2.contents.sign == NEGATIVE else 1) * int(src_num_from_array2, 16)
        
        result = function['bi_sub'](ctypes.byref(bigint1), bigint1, bigint2)
        execution_times.append(result.time)

        c_result = bi_to_int(bigint1)

        if c_result == python_result:
            if verbose:
                print()
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
            print()
            print_center(" TEST FAIL ", '-')
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
        drow_and_save_graph(data=execution_times, title="Bigint Subtraction", filename="2.bi_subtraction_execution_time.png")

def test_multiplication(function, wordlen=32, iteration=500, verbose=False):
    print_center(" 3-10. BigInt 곱셈 테스트 ", '-', '\n', 95)

    execution_times = []
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
        python_result = ((-1 if bigint1.contents.sign == NEGATIVE else 1) * int(src_num_from_array1, 16)) \
                      * ((-1 if bigint2.contents.sign == NEGATIVE else 1) * int(src_num_from_array2, 16))
        
        result = function['bi_mul'](ctypes.byref(bigint3), bigint1, bigint2, ctypes.c_bool(False))
        execution_times.append(result.time)

        c_result = bi_to_int(bigint3)

        if c_result == python_result:
            if verbose:
                print()
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
            print()
            print_center(" TEST FAIL ", '-')
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
        drow_and_save_graph(data=execution_times, title="Bigint Multiplication", filename="3.bi_multiplication_execution_time.png")

def test_multiplication_karatsuba(function, wordlen=32, iteration=500, verbose=False):
    print_center(" 3-11. BigInt 곱셈(karatsuba) 테스트 ", '-', '\n', 95)

    execution_times = []
    for i in tqdm(range(iteration), desc="BigNum Multiplication(Karatsuba) Test", unit=" iter", ncols=100):
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
        python_result = ((-1 if bigint1.contents.sign == NEGATIVE else 1) * int(src_num_from_array1, 16)) \
                      * ((-1 if bigint2.contents.sign == NEGATIVE else 1) * int(src_num_from_array2, 16))
        
        result = function['bi_mul'](ctypes.byref(bigint3), bigint1, bigint2, ctypes.c_bool(True))
        execution_times.append(result.time)

        c_result = bi_to_int(bigint3)

        if c_result == python_result:
            if verbose:
                print()
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
            print()
            print_center(" TEST FAIL ", '-')
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
        drow_and_save_graph(data=execution_times, title="Bigint Multiplication(Karatsuba)", filename="4.bi_multiplication_karatsuba_execution_time.png")

def test_squaring(function, wordlen=64, iteration=500, verbose=False):
    print_center(" 3-12. BigInt 제곱 테스트 ", '-', '\n', 95)

    execution_times = []
    for i in tqdm(range(iteration), desc="BigNum Sqauring Test", unit=" iter", ncols=100):
        sign = generate_random_sign()
        wordlen = generate_random_wordlen(wordlen)
        bigint1, bigint2, bigint3 = [ctypes.POINTER(bigint)() for _ in range(3)]

        src_array = (word * wordlen)(*(generate_random_number() for _ in range(wordlen)))
        src_num_from_array = ''.join(format(x, '08X') for x in reversed(src_array))

        result = function['bi_new'](ctypes.byref(bigint1), wordlen)
        result = function['bi_new'](ctypes.byref(bigint2), 2 * wordlen)
        result = function['bi_new'](ctypes.byref(bigint3), 2 * wordlen)
        result = function['bi_set_from_array'](ctypes.byref(bigint1), sign, wordlen, src_array)
        python_result = int(src_num_from_array, 16) ** 2
        
        result = function['bi_square'](ctypes.byref(bigint2), bigint1, ctypes.c_bool(False))
        execution_times.append(result.time)

        c_result = bi_to_int(bigint2)

        if c_result == python_result:
            if verbose:
                print()
                for j, bi in enumerate([bigint1, bigint2]):
                    print(f"\nBigInt{j + 1}: ", end='')
                    function['bi_print'](bi, 16)
                print()
                print(f"C Result: {int_to_hex(c_result)}")
                print(f"Python Result: {int_to_hex(python_result)}\n")
                print_center(f" TEST SUCCESS {i + 1} ", '-')
            function['bi_delete'](ctypes.byref(bigint1))
            function['bi_delete'](ctypes.byref(bigint2))

        else:
            print()
            print_center(" TEST FAIL ", '-')
            print("BigInt1: ", "-" if bigint1.contents.sign else "", "0x", src_num_from_array, sep="")
            print("C Result: ", int_to_hex(c_result))
            print("Python Result: ", int_to_hex(python_result))
            print()
            print_center(f" TEST FAIL (Exit At: {i+1}) ", '-')
            exit(1)

    if iteration == i + 1:
        print()
        print_center(f" TEST SUCCESS (Iteration: {iteration}) ", '-')
        drow_and_save_graph(data=execution_times, title="Bigint Squaring", filename="5.bi_squaring_execution_time.png")

def test_squaring_karatsuba(function, wordlen=64, iteration=500, verbose=False):
    print_center(" 3-13. BigInt 제곱(karatsuba) 테스트 ", '-', '\n', 95)

    execution_times = []
    for i in tqdm(range(iteration), desc="BigNum Sqauring(Karatsuba) Test", unit=" iter", ncols=100):
        sign = generate_random_sign()
        wordlen = generate_random_wordlen(wordlen)
        bigint1, bigint2 = [ctypes.POINTER(bigint)() for _ in range(2)]

        src_array = (word * wordlen)(*(generate_random_number() for _ in range(wordlen)))
        src_num_from_array = ''.join(format(x, '08X') for x in reversed(src_array))

        result = function['bi_new'](ctypes.byref(bigint1), wordlen)
        result = function['bi_new'](ctypes.byref(bigint2), 2 * wordlen)
        result = function['bi_set_from_array'](ctypes.byref(bigint1), sign, wordlen, src_array)
        python_result = (((-1 if bigint1.contents.sign == NEGATIVE else 1) * int(src_num_from_array, 16))) ** 2
        
        result = function['bi_square'](ctypes.byref(bigint2), bigint1, ctypes.c_bool(True))
        execution_times.append(result.time)

        c_result = bi_to_int(bigint2)

        if c_result == python_result:
            if verbose:
                print()
                for j, bi in enumerate([bigint1, bigint2]):
                    print(f"\nBigInt{j + 1}: ", end='')
                    function['bi_print'](bi, 16)
                print()
                print(f"C      Result: {int_to_hex(c_result)}")
                print(f"Python Result: {int_to_hex(python_result)}\n")
                print_center(f" TEST SUCCESS {i + 1} ", '-')
            function['bi_delete'](ctypes.byref(bigint1))
            function['bi_delete'](ctypes.byref(bigint2))

        else:
            print()
            print_center(" TEST FAIL ", '-')
            print("BigInt1: ", "-" if bigint1.contents.sign else "", "0x", src_num_from_array, sep="")
            print("C      Result: ", int_to_hex(c_result))
            print("Python Result: ", int_to_hex(python_result))
            print()
            print_center(f" TEST FAIL (Exit At: {i+1}) ", '-')
            exit(1)

    if iteration == i + 1:
        print()
        print_center(f" TEST SUCCESS (Iteration: {iteration}) ", '-')
        drow_and_save_graph(data=execution_times, title="Bigint Squaring(Karatsuba)", filename="6.bi_squaring_karatsuba_execution_time.png")

def test_division_bit(function, wordlen=64, iteration=500, verbose=False):
    print_center(" 3-14. BigInt 나눗셈 테스트(bit) ", '-', '\n', 95)

    execution_times = []
    for i in tqdm(range(iteration), desc="BigNum Division(Bit) Test", unit=" iter", ncols=100):
        sign1, sign2 = generate_random_sign(), POSITIVE
        wordlen1, wordlen2 = [generate_random_wordlen(wordlen) for _ in range(2)]
        bigint1, bigint2, bigint3, bigint4 = [ctypes.POINTER(bigint)() for _ in range(4)]

        src_array1 = (word * wordlen1)(*(generate_random_number() for _ in range(wordlen1)))
        src_array2 = (word * wordlen2)(*(generate_random_number() for _ in range(wordlen2)))
        
        src_num_from_array1 = ''.join(format(x, '08X') for x in reversed(src_array1))
        src_num_from_array2 = ''.join(format(x, '08X') for x in reversed(src_array2))

        result = function['bi_new'](ctypes.byref(bigint1), wordlen1)
        result = function['bi_new'](ctypes.byref(bigint2), wordlen2)
        result = function['bi_new'](ctypes.byref(bigint3), max(wordlen1, wordlen2))
        result = function['bi_new'](ctypes.byref(bigint4), max(wordlen1, wordlen2))
        result = function['bi_set_from_array'](ctypes.byref(bigint1), sign1, wordlen1, src_array1)
        result = function['bi_set_from_array'](ctypes.byref(bigint2), sign2, wordlen2, src_array2)
        python_result_Q = ((-1 if bigint1.contents.sign == NEGATIVE else 1) * int(src_num_from_array1, 16)) \
                        // ((-1 if bigint2.contents.sign == NEGATIVE else 1) * int(src_num_from_array2, 16))
        
        python_result_R = ((-1 if bigint1.contents.sign == NEGATIVE else 1) * int(src_num_from_array1, 16)) \
                        % ((-1 if bigint2.contents.sign == NEGATIVE else 1) * int(src_num_from_array2, 16))

        result = function['bi_div_bit'](ctypes.byref(bigint3), ctypes.byref(bigint4), bigint1, bigint2)
        execution_times.append(result.time)

        c_result_Q = bi_to_int(bigint3)
        c_result_R = bi_to_int(bigint4)

        if c_result_Q == python_result_Q and c_result_R == python_result_R:
            if verbose:
                print()
                for j, bi in enumerate([bigint1, bigint2, bigint3, bigint4]):
                    print(f"\nBigInt{j + 1}: ", end='')
                    function['bi_print'](bi, 16)
                print()
                print(f"C Result(Q): {int_to_hex(c_result_Q)}")
                print(f"Python Result(Q): {int_to_hex(python_result_Q)}\n")
                print(f"C Result(R): {int_to_hex(c_result_R)}")
                print(f"Python Result(R): {int_to_hex(python_result_R)}\n")
                print_center(f" TEST SUCCESS {i + 1} ", '-')
            function['bi_delete'](ctypes.byref(bigint1))
            function['bi_delete'](ctypes.byref(bigint2))
            function['bi_delete'](ctypes.byref(bigint3))
            function['bi_delete'](ctypes.byref(bigint4))

        else:
            print()
            print_center(" TEST FAIL ", '-')
            print("BigInt1: ", "-" if bigint1.contents.sign else "", "0x", src_num_from_array1, sep="")
            print("BigInt2: ", "-" if bigint2.contents.sign else "", "0x", src_num_from_array2, sep="")
            print("C Result(Q): ", int_to_hex(c_result_Q))
            print("Python Result(Q): ", int_to_hex(python_result_Q))
            print("C Result(R): ", int_to_hex(c_result_R))
            print("Python Result(R): ", int_to_hex(python_result_R))
            print()
            print_center(f" TEST FAIL (Exit At: {i+1}) ", '-')
            exit(1)

    if iteration == i + 1:
        print()
        print_center(f" TEST SUCCESS (Iteration: {iteration}) ", '-')
        drow_and_save_graph(data=execution_times, title="Bigint Division(Bit)", filename="7.bi_division_bit_execution_time.png")

def test_division_word(function, wordlen=64, iteration=50000, verbose=True): 
    print_center(" 3-15. BigInt 나눗셈 테스트(word) ", '-', '\n', 95)

    execution_times = []
    for i in tqdm(range(iteration), desc="BigNum Division(Word) Test", unit=" iter", ncols=100):
        sign1, sign2 = POSITIVE, POSITIVE
        wordlen1, wordlen2 =  [generate_random_wordlen(wordlen) for _ in range(2)]
        bigint1, bigint2, bigint3, bigint4 = [ctypes.POINTER(bigint)() for _ in range(4)]

        src_array1 = (word * wordlen1)(*(generate_random_number() for _ in range(wordlen1)))
        src_array2 = (word * wordlen2)(*(generate_random_number() for _ in range(wordlen2)))
        
        src_num_from_array1 = ''.join(format(x, '02X') for x in reversed(src_array1))
        src_num_from_array2 = ''.join(format(x, '02X') for x in reversed(src_array2))

        result = function['bi_new'](ctypes.byref(bigint1), wordlen1)
        result = function['bi_new'](ctypes.byref(bigint2), wordlen2)
        result = function['bi_new'](ctypes.byref(bigint3), max(wordlen1, wordlen2))
        result = function['bi_new'](ctypes.byref(bigint4), max(wordlen1, wordlen2))
        result = function['bi_set_from_array'](ctypes.byref(bigint1), sign1, wordlen1, src_array1)
        result = function['bi_set_from_array'](ctypes.byref(bigint2), sign2, wordlen2, src_array2)
        python_result_Q = ((-1 if bigint1.contents.sign == NEGATIVE else 1) * int(src_num_from_array1, 16)) \
                        // ((-1 if bigint2.contents.sign == NEGATIVE else 1) * int(src_num_from_array2, 16))
        
        python_result_R = ((-1 if bigint1.contents.sign == NEGATIVE else 1) * int(src_num_from_array1, 16)) \
                        % ((-1 if bigint2.contents.sign == NEGATIVE else 1) * int(src_num_from_array2, 16))

        result = function['bi_div'](ctypes.byref(bigint3), ctypes.byref(bigint4), bigint1, bigint2)
        execution_times.append(result.time)

        c_result_Q = bi_to_int(bigint3)
        c_result_R = bi_to_int(bigint4)

        if c_result_Q == python_result_Q and c_result_R == python_result_R:
            if verbose:
                print()
                for j, bi in enumerate([bigint1, bigint2, bigint3, bigint4]):
                    print(f"\nBigInt{j + 1}: ", end='')
                    function['bi_print'](bi, 16)
                print()
                print(f"C Result(Q): {int_to_hex(c_result_Q)}")
                print(f"Python Result(Q): {int_to_hex(python_result_Q)}\n")
                print(f"C Result(R): {int_to_hex(c_result_R)}")
                print(f"Python Result(R): {int_to_hex(python_result_R)}\n")
                print_center(f" TEST SUCCESS {i + 1} ", '-')
            function['bi_delete'](ctypes.byref(bigint1))
            function['bi_delete'](ctypes.byref(bigint2))
            function['bi_delete'](ctypes.byref(bigint3))
            function['bi_delete'](ctypes.byref(bigint4))

        else:
            print()
            print_center(" TEST FAIL ", '-')
            print("BigInt1: ", "-" if bigint1.contents.sign else "", "0x", src_num_from_array1, sep="")
            print("BigInt2: ", "-" if bigint2.contents.sign else "", "0x", src_num_from_array2, sep="")
            print("C Result(Q): ", int_to_hex(c_result_Q))
            print("Python Result(Q): ", int_to_hex(python_result_Q))
            print("C Result(R): ", int_to_hex(c_result_R))
            print("Python Result(R): ", int_to_hex(python_result_R))
            print()
            print_center(f" TEST FAIL (Exit At: {i+1}) ", '-')
            exit(1)

    if iteration == i + 1:
        print()
        print_center(f" TEST SUCCESS (Iteration: {iteration}) ", '-')
        drow_and_save_graph(data=execution_times, title="Bigint Division(Word)", filename="8.bi_division_word_execution_time.png")

def test_shift_left(function, iteration=1, verbose=False):
    print_center(" 3-16. BigInt 시프트(비트) 테스트 ", '-', '\n', 95)

    execution_times = []
    for i in tqdm(range(iteration), desc="BigNum Shift(Bit) Test", unit=" iter", ncols=100):
        sign = generate_random_sign()
        
        for j in range(1024):
            src_array = (word * 1)(*[1])
            bigint1 = ctypes.POINTER(bigint)()
            src_num_from_array = ''.join(format(x, '08X') for x in reversed(src_array))
            result = function['bi_new'](ctypes.byref(bigint1), 1)
            result = function['bi_set_from_array'](ctypes.byref(bigint1), sign, 1, src_array)
            result = function['bi_bit_left_shift'](ctypes.byref(bigint1), (j*4))
            execution_times.append(result.time)
            python_result = int(src_num_from_array, 16) << (j*4)
            
            if verbose:
                print('C Result: ', end='')
                function['bi_print'](bigint1, 16)
                print('Python Result: ', int_to_hex(python_result))

            function['bi_delete'](ctypes.byref(bigint1))

    if iteration == i + 1:
        print()
        print_center(f" TEST SUCCESS (Iteration: {iteration}) ", '-')
        drow_and_save_graph(data=execution_times, title="Bigint Shift(Bit)", filename="9.bi_shift_bit_execution_time.png")

def test_exponential_ltr(function, wordlen=8, iteration=1000, verbose=True):
    print_center(" 3-17. BigInt 거듭제곱(L-to-R) 테스트 ", '-', '\n', 95)

    execution_times = []
    for i in tqdm(range(iteration), desc="BigNum Exponential(Left-to-Right) Test", unit=" iter", ncols=100):
        sign1, sign2, sign3 = POSITIVE, POSITIVE, POSITIVE
        wordlen1, wordlen2, wordlen3 = [generate_random_wordlen(wordlen) for _ in range(3)]
        bigint1, bigint2, bigint3, bigint4 = [ctypes.POINTER(bigint)() for _ in range(4)]

        src_array1 = (word * wordlen1)(*(generate_random_number() for _ in range(wordlen1)))
        src_array2 = (word * wordlen2)(*(generate_random_number() for _ in range(wordlen2)))
        src_array3 = (word * wordlen3)(*(generate_random_number() for _ in range(wordlen3)))
        
        src_num_from_array1 = ''.join(format(x, '02X') for x in reversed(src_array1))
        src_num_from_array2 = ''.join(format(x, '02X') for x in reversed(src_array2))
        src_num_from_array3 = ''.join(format(x, '02X') for x in reversed(src_array3))

        result = function['bi_new'](ctypes.byref(bigint1), wordlen1)
        result = function['bi_new'](ctypes.byref(bigint2), wordlen2)
        result = function['bi_new'](ctypes.byref(bigint3), wordlen3)
        result = function['bi_new'](ctypes.byref(bigint4), max(wordlen1, wordlen2))
        result = function['bi_set_from_array'](ctypes.byref(bigint1), sign1, wordlen1, src_array1)
        result = function['bi_set_from_array'](ctypes.byref(bigint2), sign2, wordlen2, src_array2)
        result = function['bi_set_from_array'](ctypes.byref(bigint3), sign3, wordlen3, src_array3)

        python_result = pow(
                        ((-1 if bigint1.contents.sign == NEGATIVE else 1) * int(src_num_from_array1, 16))
                      , ((-1 if bigint2.contents.sign == NEGATIVE else 1) * int(src_num_from_array2, 16))
                      , ((-1 if bigint3.contents.sign == NEGATIVE else 1) * int(src_num_from_array3, 16))
                      )

        result = function['bi_exp_left_to_right_mod'](ctypes.byref(bigint4), bigint1, bigint2, bigint3)
        execution_times.append(result.time)

        c_result = bi_to_int(bigint4)

        if c_result == python_result:
            if verbose:
                print()
                for j, bi in enumerate([bigint1, bigint2, bigint3, bigint4]):
                    print(f"\nBigInt{j + 1}:   ", end='')
                    function['bi_print'](bi, 16)
                print()
                print(f"C Result: {int_to_hex(c_result)}")
                print(f"Python Result: {int_to_hex(python_result)}\n")
                print_center(f" TEST SUCCESS {i + 1} ", '-')
            function['bi_delete'](ctypes.byref(bigint1))
            function['bi_delete'](ctypes.byref(bigint2))
            function['bi_delete'](ctypes.byref(bigint3))
            function['bi_delete'](ctypes.byref(bigint4))

        else:
            print()
            print_center(" TEST FAIL ", '-')
            print("BigInt1: ", "-" if bigint1.contents.sign else "", "0x", src_num_from_array1, sep="")
            print("BigInt2: ", "-" if bigint2.contents.sign else "", "0x", src_num_from_array2, sep="")
            print("BigInt3: ", "-" if bigint3.contents.sign else "", "0x", src_num_from_array3, sep="")
            print("C Result: ", int_to_hex(c_result))
            print("Python Result: ", int_to_hex(python_result))
            print()
            print_center(f" TEST FAIL (Exit At: {i+1}) ", '-')
            exit(1)

    if iteration == i + 1:
        print()
        print_center(f" TEST SUCCESS (Iteration: {iteration}) ", '-')
        drow_and_save_graph(data=execution_times, title="Bigint Exponential(Left-to-Right)", filename="10.bi_exponential_ltr_execution_time.png")

def test_exponential_rtl(function, wordlen=2, iteration=30, verbose=False):
    print_center(" 3-18. BigInt 거듭제곱(R-to-L) 테스트 ", '-', '\n', 95)

    execution_times = []
    for i in tqdm(range(iteration), desc="BigNum Exponential(Right-to-Left) Test", unit=" iter", ncols=100):
        sign1, sign2, sign3 = POSITIVE, POSITIVE, POSITIVE
        wordlen1, wordlen2, wordlen3 = [generate_random_wordlen(wordlen) for _ in range(3)]
        bigint1, bigint2, bigint3, bigint4 = [ctypes.POINTER(bigint)() for _ in range(4)]

        src_array1 = (word * wordlen1)(*(generate_random_number() for _ in range(wordlen1)))
        src_array2 = (word * wordlen2)(*(generate_random_number() for _ in range(wordlen2)))
        src_array3 = (word * wordlen3)(*(generate_random_number() for _ in range(wordlen3)))
        
        src_num_from_array1 = ''.join(format(x, '08X') for x in reversed(src_array1))
        src_num_from_array2 = ''.join(format(x, '08X') for x in reversed(src_array2))
        src_num_from_array3 = ''.join(format(x, '08X') for x in reversed(src_array3))

        result = function['bi_new'](ctypes.byref(bigint1), wordlen1)
        result = function['bi_new'](ctypes.byref(bigint2), wordlen2)
        result = function['bi_new'](ctypes.byref(bigint3), wordlen3)
        result = function['bi_new'](ctypes.byref(bigint4), max(wordlen1, wordlen2))
        result = function['bi_set_from_array'](ctypes.byref(bigint1), sign1, wordlen1, src_array1)
        result = function['bi_set_from_array'](ctypes.byref(bigint2), sign2, wordlen2, src_array2)
        result = function['bi_set_from_array'](ctypes.byref(bigint3), sign3, wordlen3, src_array3)
        python_result = pow(
                        ((-1 if bigint1.contents.sign == NEGATIVE else 1) * int(src_num_from_array1, 16))
                      , ((-1 if bigint2.contents.sign == NEGATIVE else 1) * int(src_num_from_array2, 16))
                      , ((-1 if bigint3.contents.sign == NEGATIVE else 1) * int(src_num_from_array3, 16))
                      )
        
        result = function['bi_exp_right_to_left_mod'](ctypes.byref(bigint4), bigint1, bigint2, bigint3)
        execution_times.append(result.time)

        c_result = bi_to_int(bigint4)

        if c_result == python_result:
            if verbose:
                print()
                for j, bi in enumerate([bigint1, bigint2, bigint3, bigint4]):
                    print(f"\nBigInt{j + 1}: ", end='')
                    function['bi_print'](bi, 16)
                print()
                print(f"C Result: {int_to_hex(c_result)}")
                print(f"Python Result: {int_to_hex(python_result)}\n")
                print_center(f" TEST SUCCESS {i + 1} ", '-')
            function['bi_delete'](ctypes.byref(bigint1))
            function['bi_delete'](ctypes.byref(bigint2))
            function['bi_delete'](ctypes.byref(bigint3))
            function['bi_delete'](ctypes.byref(bigint4))

        else:
            print()
            print_center(" TEST FAIL ", '-')
            print("BigInt1: ", "-" if bigint1.contents.sign else "", "0x", src_num_from_array1, sep="")
            print("BigInt2: ", "-" if bigint2.contents.sign else "", "0x", src_num_from_array2, sep="")
            print("BigInt3: ", "-" if bigint3.contents.sign else "", "0x", src_num_from_array3, sep="")
            print("C Result: ", int_to_hex(c_result))
            print("Python Result: ", int_to_hex(python_result))
            print()
            print_center(f" TEST FAIL (Exit At: {i+1}) ", '-')
            exit(1)

    if iteration == i + 1:
        print()
        print_center(f" TEST SUCCESS (Iteration: {iteration}) ", '-')
        drow_and_save_graph(data=execution_times, title="Bigint Exponential(Right-to-Left)", filename="11.bi_exponential_rtl_execution_time.png")

def test_exponential_montgomery(function, wordlen=2, iteration=30, verbose=False):
    print_center(" 3-19. BigInt 거듭제곱(Montgomery) 테스트 ", '-', '\n', 95)

    execution_times = []
    for i in tqdm(range(iteration), desc="BigNum Exponential(Montgomery) Test", unit=" iter", ncols=100):
        sign1, sign2, sign3 = POSITIVE, POSITIVE, POSITIVE
        wordlen1, wordlen2, wordlen3 = [generate_random_wordlen(wordlen) for _ in range(3)]
        bigint1, bigint2, bigint3, bigint4 = [ctypes.POINTER(bigint)() for _ in range(4)]

        wordlen1 = 64
        src_array1 = (word * wordlen1)(*(generate_random_number() for _ in range(wordlen1)))
        src_array2 = (word * wordlen2)(*(generate_random_number() for _ in range(wordlen2)))
        src_array3 = (word * wordlen3)(*(generate_random_number() for _ in range(wordlen3)))
        
        src_num_from_array1 = ''.join(format(x, '08X') for x in reversed(src_array1))
        src_num_from_array2 = ''.join(format(x, '08X') for x in reversed(src_array2))
        src_num_from_array3 = ''.join(format(x, '08X') for x in reversed(src_array3))

        result = function['bi_new'](ctypes.byref(bigint1), wordlen1)
        result = function['bi_new'](ctypes.byref(bigint2), wordlen2)
        result = function['bi_new'](ctypes.byref(bigint3), wordlen3)
        result = function['bi_new'](ctypes.byref(bigint4), max(wordlen1, wordlen2))
        result = function['bi_set_from_array'](ctypes.byref(bigint1), sign1, wordlen1, src_array1)
        result = function['bi_set_from_array'](ctypes.byref(bigint2), sign2, wordlen2, src_array2)
        result = function['bi_set_from_array'](ctypes.byref(bigint3), sign3, wordlen3, src_array3)
        python_result = pow(
                        ((-1 if bigint1.contents.sign == NEGATIVE else 1) * int(src_num_from_array1, 16))
                      , ((-1 if bigint2.contents.sign == NEGATIVE else 1) * int(src_num_from_array2, 16))
                      , ((-1 if bigint3.contents.sign == NEGATIVE else 1) * int(src_num_from_array3, 16))
                      )
        
        result = function['bi_exp_montgomery_mod'](ctypes.byref(bigint4), bigint1, bigint2, bigint3)
        execution_times.append(result.time)

        c_result = bi_to_int(bigint4)

        if c_result == python_result:
            if verbose:
                print()
                for j, bi in enumerate([bigint1, bigint2, bigint3, bigint4]):
                    print(f"\nBigInt{j + 1}: ", end='')
                    function['bi_print'](bi, 16)
                print()
                print(f"C Result: {int_to_hex(c_result)}")
                print(f"Python Result: {int_to_hex(python_result)}\n")
                print_center(f" TEST SUCCESS {i + 1} ", '-')
            function['bi_delete'](ctypes.byref(bigint1))
            function['bi_delete'](ctypes.byref(bigint2))
            function['bi_delete'](ctypes.byref(bigint3))
            function['bi_delete'](ctypes.byref(bigint4))

        else:
            print()
            print_center(" TEST FAIL ", '-')
            print("BigInt1: ", "-" if bigint1.contents.sign else "", "0x", src_num_from_array1, sep="")
            print("BigInt2: ", "-" if bigint2.contents.sign else "", "0x", src_num_from_array2, sep="")
            print("BigInt3: ", "-" if bigint3.contents.sign else "", "0x", src_num_from_array3, sep="")
            print("C Result: ", int_to_hex(c_result))
            print("Python Result: ", int_to_hex(python_result))
            print()
            print_center(f" TEST FAIL (Exit At: {i+1}) ", '-')
            exit(1)

    if iteration == i + 1:
        print()
        print_center(f" TEST SUCCESS (Iteration: {iteration}) ", '-')
        drow_and_save_graph(data=execution_times, title="Bigint Exponential(Montgomery)", filename="12.bi_exponential_montgomery_execution_time.png")

def test_reduction(function, wordlen=1, iteration=1, verbose=False): 
    print_center(" 3-20. BigInt 나머지 연산 테스트 ", '-', '\n', 95)

    execution_times = []
    for i in tqdm(range(iteration), desc="BigNum Reduction Test", unit=" iter", ncols=100):
        sign1, sign2 = POSITIVE, POSITIVE
        wordlen1, wordlen2 =  [generate_random_wordlen(wordlen) for _ in range(2)]
        bigint1, bigint2, bigint3, bigint4 = [ctypes.POINTER(bigint)() for _ in range(4)]

        src_array1 = (word * wordlen1)(*[2])
        src_array2 = (word * wordlen2)(*[3])
        
        src_num_from_array1 = ''.join(format(x, '08X') for x in reversed(src_array1))
        src_num_from_array2 = ''.join(format(x, '08X') for x in reversed(src_array2))

        result = function['bi_new'](ctypes.byref(bigint1), wordlen1)
        result = function['bi_new'](ctypes.byref(bigint2), wordlen2)
        result = function['bi_new'](ctypes.byref(bigint3), wordlen1 * 2)
        result = function['bi_new'](ctypes.byref(bigint4), max(wordlen1, wordlen2))
        result = function['bi_set_from_array'](ctypes.byref(bigint1), sign1, wordlen1, src_array1)
        result = function['bi_set_from_array'](ctypes.byref(bigint2), sign2, wordlen2, src_array2)
        python_result = ((-1 if bigint1.contents.sign == NEGATIVE else 1) * int(src_num_from_array1, 16)) \
                      % ((-1 if bigint2.contents.sign == NEGATIVE else 1) * int(src_num_from_array2, 16))

        # w=wordlen1, W=2^w, T=W^(2*bigint2) 일때 C 함수로 계산하기
        
        # w를 bigint 타입으로 선언
        base = 16
        temp_bigintA = ctypes.POINTER(bigint)()
        temp_bigintN = ctypes.POINTER(bigint)()
        temp_bigintT = ctypes.POINTER(bigint)()

        A_int = 1382058139289313913155088058062394732797793233889
        N_int = 699000786352191248570217
        T_int = 2090844053206151817602386
        AmodN= 309030206782452567950816
        A = hex(A_int)
        N = hex(N_int)
        T = hex(T_int)
        print(f"\nAnswer(10진수): {AmodN}")
        print(f"Answer(16진수): {hex(AmodN)}")
        src_stringA = ctypes.c_char_p(A.encode('utf-8'))
        src_stringN = ctypes.c_char_p(N.encode('utf-8'))
        src_stringT = ctypes.c_char_p(T.encode('utf-8'))
        result = function['bi_set_from_string'](ctypes.byref(temp_bigintA), src_stringA, base)
        result = function['bi_set_from_string'](ctypes.byref(temp_bigintN), src_stringN, base)
        result = function['bi_set_from_string'](ctypes.byref(temp_bigintT), src_stringT, base)
        python_result = int(A, 16) \
                      % int(N, 16)
        print(f"Python Result(10진수): {python_result}")
        print(f"Python Result(16진수): {int_to_hex(python_result)}")
        result = function['bi_mod'](ctypes.byref(bigint4), temp_bigintA, temp_bigintN, temp_bigintT)
        execution_times.append(result.time)


        function['bi_delete'](ctypes.byref(temp_bigintA))
        function['bi_delete'](ctypes.byref(temp_bigintN))
        function['bi_delete'](ctypes.byref(temp_bigintT))

        c_result = bi_to_int(bigint4)

        if c_result == python_result:
            if verbose:
                print()
                for j, bi in enumerate([bigint1, bigint2, bigint3, bigint4]):
                    print(f"\nBigInt{j + 1}: ", end='')
                    function['bi_print'](bi, 16)
                print()
                print(f"C Result: {int_to_hex(c_result)}")
                print(f"Python Result: {int_to_hex(python_result)}\n")
                print_center(f" TEST SUCCESS {i + 1} ", '-')
            function['bi_delete'](ctypes.byref(bigint1))
            function['bi_delete'](ctypes.byref(bigint2))
            function['bi_delete'](ctypes.byref(bigint3))
            function['bi_delete'](ctypes.byref(bigint4))

        else:
            print()
            print_center(" TEST FAIL ", '-')
            print("BigInt1: ", "-" if bigint1.contents.sign else "", "0x", src_num_from_array1, sep="")
            print("BigInt2: ", "-" if bigint2.contents.sign else "", "0x", src_num_from_array2, sep="")
            print("C Result(R): ", int_to_hex(c_result))
            print("Python Result(R): ", int_to_hex(python_result))
            print()
            print_center(f" TEST FAIL (Exit At: {i+1}) ", '-')
            exit(1)

    if iteration == i + 1:
        print()
        print_center(f" TEST SUCCESS (Iteration: {iteration}) ", '-')
        drow_and_save_graph(data=execution_times, title="Bigint Reduction", filename="13.bi_reduction_execution_time.png")

def test_shift_left(function, iteration=1, verbose=False):
    print_center(" 3-16. BigInt 시프트(비트) 테스트 ", '-', '\n', 95)

    for i in tqdm(range(iteration), desc="BigNum Shift(Bit) Test", unit=" iter", ncols=100):
        sign = generate_random_sign()
        
        for j in range(516):
            src_array = (word * 1)(*[0xFE])
            bigint1 = ctypes.POINTER(bigint)()
            src_num_from_array = ''.join(format(x, '02X') for x in reversed(src_array))
            result = function['bi_new'](ctypes.byref(bigint1), 1)
            result = function['bi_set_from_array'](ctypes.byref(bigint1), sign, 1, src_array)
            function['bi_bit_left_shift'](ctypes.byref(bigint1), (j*4))
            python_result = int(src_num_from_array, 16) << (j*4)

            print('\nK bit: ', j*4)
            print('C Result: ', end='')
            function['bi_print'](bigint1, 16)
            print('Python Result: ', int_to_hex(python_result))

            function['bi_delete'](ctypes.byref(bigint1))

    if iteration == i + 1:
        print()
        print_center(f" TEST SUCCESS (Iteration: {iteration}) ", '-')

def test():
    OS = platform.system()

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
    # test_multiplication(function)

    # 3-11 BigInt 곱셈(karatsuba) 테스트
    # test_multiplication_karatsuba(function)

    # 3-12 BigInt 제곱 테스트
    # test_squaring(function)

    # 3-13 BigInt 제곱 테스트
    # test_squaring_karatsuba(function)

    # 3-14 BigInt 나눗셈(Bit) 테스트
    # test_division_bit(function)

    # 3-15 BigInt 나눗셈(Word) 테스트
    test_division_word(function)

    # 3-16 BigInt 시프트(비트) 테스트
    # test_shift_left(function)

    # 3-17 BigInt 거듭제곱(L-to-R) 테스트
    # test_exponential_ltr(function)

    # 3-18 BigInt 거듭제곱(R-to-L) 테스트
    # test_exponential_rtl(function)

    # 3-19 BigInt 거듭제곱(Montgomery) 테스트
    # test_exponential_montgomery(function)

    # 3-20 BigInt 나머지 연산 테스트
    # test_reduction(function)

    # 실행 테스트
    # os.system(command=f"./build/{OS}/IORA")

    # 테스트 종료

    gc.enable()

    print_center(" FINISH IORA TEST ")
    end_time = datetime.now(seoul_tz)
    print_time(end_time, "END TIME: ", '')

    print_total_time(start_time, end_time)
    print_line()

if __name__ == "__main__":
    test()