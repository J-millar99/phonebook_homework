#include "tel.h"

// 입력을 활성화 하는 함수
void abledInput() {
    nocbreak();            // 라인 버퍼링 활성화
    echo();                // 입력된 문자를 화면에 표시
    curs_set(TRUE);        // 커서를 보이게 함
}

// 입력을 비활성화하는 함수
void disabledInput() {
    cbreak();             // 라인 버퍼링을 비활성화, 즉시 입력을 받을 수 있게 함
    noecho();             // 입력된 문자를 화면에 표시하지 않음
    curs_set(FALSE);      // 커서를 보이지 않게 함
}

// 특정 윈도우의 특정 위치에서 길이만큼 출력을 지우는 함수
void eraseCursor(WINDOW *win, int y, int x, int length) {
    wmove(win, y, x);                   // 윈도우에서 커서를 이동
    for (int i = 0; i < length; i++)    // 공백문자를 통해 화면에서 지우는 효과
        waddstr(win, " ");
    wrefresh(win);                      // 윈도우 갱신
}

//  현재 페이지를 계산하는 함수
void calPage() {
    if (page < 0) // 페이지가 0보다 작으면
        page = m;   // 페이지를 마지막 페이지로 설정
    else if (page > m) //페이지가 마지막 페이지보다 크면
        page = 0;   // 페이지를 0으로 설정
}

// string을 출력하는 함수
int printInfo(char *str) {
    write(1, str, strlen(str));
    write(1, "\n", 1);
    return 1;
}

// 출력효과를 주는 함수
void mvwprintwEffect(WINDOW *win, int y, int x, char *str) {
    int idx = 0;
    while (str[idx]) {
        mvwprintw(win, y, x + idx, "%c", str[idx]);
        usleep(100000);
        wrefresh(win);
        idx++;
    }
}