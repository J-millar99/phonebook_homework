#include "tel.h"

// 프로그램을 더 이상 진행할 수 없는 에러가 발생할 경우 표준에러로 출력문을 나타내고 프로그램을 종료 에러코드 1(general error)
void errorExit(char *errorString) {
    write(2, errorString, strlen(errorString));
    write(2, "\n", 1);
    exit(1);
}

// 프로그램에 진행에 문제없는 에러일 경우 표준에러로 출력문을 나타냄
void errorPrint(char *errorString) {
    write(2, errorString, strlen(errorString));
    write(2, "\n", 1);
}

// 입력된 필드가 이름, 전화번호가 포함되어 있는지 확인
void checkToken(char *str) {
    while (*str && isspace(*str))   // 문자열의 시작부분의 space를 건너뜀
        str++;
    if (!*str || *str == ':')       // 공백을 건너뛰었는데 문자열이 끝났거나 구분자':'를 발견하면 이름 필드가 존재하지 않음
        errorExit("올바르지 않은 이름 필드가 존재합니다");

    while (*str && *str != ':')     // 이 구간은 이름 필드이므로 구분자가 나올때 까지 넘어감
        str++;
    if (!*str)                      // 구분자가 나오지 않고 문자열이 끝났다면 전화번호 필드가 존재하지 않는 것이므로 처리
        errorExit("올바르지 않은 전화번호 필드가 존재합니다");
    
    str++;                          // 구분자 한 칸을 건너뜀 -> 이제부터 전화번호 필드
    while (*str && isspace(*str))   // 전화번호 필드의 시작 space를 건너뜀
        str++;
    if (!*str || *str == ':')       // 이름 필드와 마찬가지로 공백을 건너뛰었는데 문자열이 끝났거나 구분자가 발견되면 전화번호 필드가 존재하지 않음
        errorExit("올바르지 않은 전화번호 필드가 존재합니다");
}

// 스크린의 하단에 오류를 출력하는 함수
void errorScreen(WINDOW *to, char *error) {
    int x, y;
    getmaxyx(to, y, x);
    disabledInput();    //  커서와 입력이 표시되지 않게 함
    mvwprintw(to, y - 2, (x - strlen(error)) / 2, "%s", error);
    wgetch(to); // 아무키나 누를때 까지 에러출력문을 띄움
    eraseCursor(to, y - 2, (x - strlen(error)) / 2, strlen(error)); // 입력받을 때 에러 출력문 지우기
    abledInput();   // 커서와 입력 활성화
}