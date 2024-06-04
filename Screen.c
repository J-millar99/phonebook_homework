#include "tel.h"

int page = 0;                           // 연락처 preview의 page를 표시할 변수
static void initScreen();               // 화면 초기화 함수
static void loadingScreen();            // 로딩창을 표현할 함수1
static void launchScreen(WINDOW *win);  // 로딩창을 표현할 함수2

int l, m;    // 한 페이지에 표시할 연락처 개수, 페이지 수

void screen() {
    initScreen();   // 초기 nucurse 설정
    menuScreen();   // menuScreen부터 시작 
    endwin();       // ncurses 종료
}

static void initScreen() {
    initscr();              // ncurses 초기화
    disabledInput();        // 입력 비활성화
    loadingScreen();        // 로딩 창 표혐
}

static void loadingScreen() {
    int startx, starty, width, height;
    height = 8;
    width = 60;
    starty = (LINES - height) / 2;  // 화면 중앙에 위치하도록 y 좌표 설정
    startx = (COLS - width) / 2;    // 화면 중앙에 위치하도록 x 좌표 설정

    WINDOW *win = newwin(height, width, starty, startx); // 보조 창 생성
    box(win, 0 , 0);    // 보조 창 테두리 그리기
    launchScreen(win);  // 보조창 실행
    werase(win);        // 보조 창 지우기
    wrefresh(win);      // 보조 창 갱신 
    delwin(win);        // 보조 창 삭제  
}

static void launchScreen(WINDOW *win) {
    int max_x, max_y;
    getmaxyx(win, max_y, max_x); // 보조 창의 크기를 얻어옴

    char *message = "PhoneBook Program";
    char *version = "V1.0";
    mvwprintw(win, max_y / 2 - 2, (max_x - strlen(message) - strlen(version)) / 2, "%s", message);
    wattron(win, A_BOLD);   // BOLD 속성 켜기
    mvwprintw(win, max_y / 2 - 2, (max_x - strlen(message) - strlen(version)) / 2 + strlen(message), " %s", version);
    wattroff(win, A_BOLD);  // BOLD 속성 끄기

    // 로딩바 표현
    mvwprintw(win, max_y / 2, (max_x - 50) / 2, "[");
    mvwprintw(win, max_y / 2, (max_x + 50) / 2, "]");
    wrefresh(win);
    for (int i = 0; i <= 100; i++) {
        int progress = (50 * i) / 100;
        for (int j = 0; j < progress; j++)
            mvwprintw(win, max_y / 2, (max_x - 50) / 2 + 1 + j, "=");
        mvwprintw(win, max_y / 2 + 1, (max_x - 3) / 2, "%3d%%", i);

        wrefresh(win);
        usleep(50000); //  0.05초 대기
    }
}