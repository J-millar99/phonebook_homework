#include "tel.h"

static void *displayTime(void *arg);

void timeScreen() {
    int time_win_height = 3;
    int time_win_width = 22;
    int time_win_starty = LINES - time_win_height;      // 화면의 최하단
    int time_win_startx = (COLS - time_win_width) / 2;  // 화면의 중앙

    WINDOW *time_win = newwin(time_win_height, time_win_width, time_win_starty, time_win_startx); // 시간 창 생성

    pthread_t time_thread;
    pthread_create(&time_thread, NULL, displayTime, (void *)time_win);
    pthread_detach(time_thread);
}

static void *displayTime(void *arg) {
    WINDOW *time_win = (WINDOW *)arg;
    while (1) {
        // 현재 시각을 얻어옴
        time_t rawtime;
        struct tm *timeinfo;
        char buffer[30];

        time(&rawtime);
        timeinfo = localtime(&rawtime);

        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
        // 보조 창에 현재 시각 출력
        werase(time_win);
        mvwprintw(time_win, 1, 2, "%s", buffer);
        wrefresh(time_win);

        sleep(1); // 1초 대기
    }
    return NULL;
}