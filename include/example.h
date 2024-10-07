/**
 * @file example.h
 * @author 조동후 : 작성자
 * @date 2024-10-07 : 최초 작성일자
 * @brief 이 파일은 Doxygen 문서화 주석의 이해를 돕기 위한 예제 파일입니다.
 */

/**
 * @struct Person
 * @brief 사람 정보를 저장하는 구조체
 *
 * @details 구조체는 사람의 이름, 나이, 키와 같은 기본 정보를 저장합니다. 구조체 맴버 설명 작성하기.
 * 여러 줄 작성해도됨
 */
typedef struct {
    char name[50];   ///< 사람의 이름
    int age;         ///< 사람의 나이
    float height;    ///< 사람의 키 (단위: 미터)
} Person;


/**
 * @struct Point
 * @brief 2차원 좌표를 저장하는 구조체
 * @details 구조체는 x, y 좌표를 저장합니다. 구조체 맴버 설명 작성하기
 */
typedef struct {
    int x; ///< x 좌표
    int y; ///< y 좌표
} Point;

/**
 * @typedef unsigned int uint
 * @brief unsigned int의 별칭(이건 구조체가 아니므로 detail 작성할 필요 없음)
 */
typedef unsigned int uint; ///< unsigned int의 별칭. WORD와 같은 역할을 합니다.