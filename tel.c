#include "tel.h"
#include <locale.h>

int main() {
    inputData();    // 실행 전 단계 - 파일의 데이터를 코드의 데이터 구조(연결리스트)로 옮김
    setlocale(LC_ALL, "");  // 한글 출력을 위한 환경 설정
    screen();       // Screen - GUI 시작
    outputData();   // 종료 단계 - 갱신한 전화번호부를 파일에 반영하고 할당한 메모리를 정리
    lstClear(phoneBook);
    return 0;
}