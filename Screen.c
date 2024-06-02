#include "tel.h"

int page = 0;
static void launchScreen(WINDOW *win);
static void initScreen();

int l, m, r;

void screen() {
    initScreen();
    menuScreen();
    endwin();        // ncurses 종료
}

static void initScreen() {
    initscr();            // ncurses 초기화
    disabledInput();      // 입력 비활성화

    /*  시작 화면 */
    // int startx, starty, width, height;
    // height = 8;
    // width = 60;
    // starty = (LINES - height) / 2;  // 화면 중앙에 위치하도록 y 좌표 설정
    // startx = (COLS - width) / 2;    // 화면 중앙에 위치하도록 x 좌표 설정

    // WINDOW *win = newwin(height, width, starty, startx); // 보조 창 생성
    // box(win, 0 , 0); // 보조 창 테두리 그리기
    // launchScreen(win);  // 보조창 실행
    // werase(win);
    // wrefresh(win);
    // delwin(win);     // 보조 창 삭제
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

    mvwprintw(win, max_y / 2, (max_x - 50) / 2, "[");
    mvwprintw(win, max_y / 2, (max_x + 50) / 2, "]");
    wrefresh(win);
    for (int i = 0; i <= 100; i++) {
        int progress = (50 * i) / 100;
        for (int j = 0; j < progress; j++)
            mvwprintw(win, max_y / 2, (max_x - 50) / 2 + 1 + j, "=");
        mvwprintw(win, max_y / 2 + 1, (max_x - 3) / 2, "%3d%%", i);

        wrefresh(win);
        usleep(50000); // 0.05초 대기
    }
}

void phoneBookScreen(WINDOW *pb_win) {
    werase(pb_win);
    t_data *head = phoneBook;
    int width, height, idx = 3;

    getmaxyx(pb_win, height, width);
    calPage();
    for (int i = 0; i < page * l; i++)
        head = head->next;
    box(pb_win, 0, 0); // 보조 창 테두리 그리기
    mvwprintw(pb_win, 1, (width - strlen("Contact List")) / 2, "Contact List");
    while (head && idx < height - 2) {
        mvwprintw(pb_win, idx++, 1, "Name: %s Phone: %s", head->name, head->phone);
        head = head->next;
    }
    mvwprintw(pb_win, height - 2, width / 2 - 3, "<%dpage>", page + 1);
    wrefresh(pb_win);
}

void calPage() {
    if (page < 0) // 페이지가 0보다 작으면
        page = m;   // 페이지를 마지막 페이지로 설정
    else if (page > m) //페이지가 마지막 페이지보다 크면
        page = 0;   // 페이지를 0으로 설정
}