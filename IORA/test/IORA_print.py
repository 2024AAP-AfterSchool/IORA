from IORA_error import result_code

weekday_names = ['월요일', '화요일', '수요일', '목요일', '금요일', '토요일', '일요일']

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