#include "tel.h"

void f() {
    char command[256];
    snprintf(command, sizeof(command), "leaks %d", getpid());
    system(command);
}

int main() {

    // atexit(f);
    inputData(); // 실행 전 단계 - 파일의 데이터를 코드의 데이터 구조(연결리스트)로 옮김  
    screen();
    fileAppend(phoneBook); // 종료 단계 - 갱신한 전화번호부를 파일에 반영하고 할당한 메모리를 정리
    lstClear(phoneBook);
    return 0;
}