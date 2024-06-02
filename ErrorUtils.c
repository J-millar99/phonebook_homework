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