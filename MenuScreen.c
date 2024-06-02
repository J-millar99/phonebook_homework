#include "tel.h"

static void printMenu(WINDOW *menu_win, int highlight);
static void printDescription(WINDOW *desc_win, int highlight);

static char *choices[] = {
    "ADD",
    "SEARCH",
    "DELETE",
    "LIST",
    "EXIT",
};

static char *descriptions[] = {
    "Add a new Contact.",
    "Search for an Contacts at PhoneBook.",
    "Delete an Contacts at PhoneBook.",
    "List all Contacts at PhoneBook.",
    "Exit the program."
};

static int n_choices = sizeof(choices) / sizeof(char*);

void menuScreen() {
    WINDOW *menu_win = newwin(LINES / 2, COLS / 2 - 2, 1, COLS / 2 + 1);
    WINDOW *desc_win = newwin(LINES / 2 - 5, COLS / 2 - 3, LINES / 2 + 1, COLS / 2 + 1);
    WINDOW *pb_win = newwin(LINES - 5, COLS / 2, 1, 1);
    int highlight = 1;
    int choice = SELECT;
    int c;

    l = getmaxy(pb_win) - 5;
    m = lstCount(phoneBook) / l;
    r = lstCount(phoneBook) % l;

    keypad(menu_win, TRUE);
    while (choice == SELECT) {
        printMenu(menu_win, highlight);
        printDescription(desc_win, highlight);
        phoneBookScreen(pb_win);
        disabledInput();    // 입력 비활성화
        c = wgetch(menu_win);
        switch (c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = n_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == n_choices)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case KEY_LEFT:
                page--;
                break;
            case KEY_RIGHT:
                page++;
                break;
            case 10: // 엔터키
                choice = highlight;
                break;
            default:
                break;
        }
        if (choice != SELECT) {// 엔터키를 눌렀을 때 루프 종료
            abledInput();       // 입력 활성화
            werase(menu_win);
            werase(desc_win);
            werase(pb_win);
            wrefresh(menu_win);
            wrefresh(desc_win);
            wrefresh(pb_win);
            choice = commandScreen(choice);
        }
    }

    mvprintw(LINES - 2, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
    clrtoeol();
}

static void printMenu(WINDOW *menu_win, int highlight) {
    werase(menu_win); // 창을 지우기
    int x, y, i;
    x = 2; y = 1;
    box(menu_win, 0, 0);
    for (i = 0; i < n_choices; ++i) {
        if (highlight == i + 1) { // 현재 선택된 항목을 하이라이트
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        } else {
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        }
        ++y;
    }
    wrefresh(menu_win);
}

static void printDescription(WINDOW *desc_win, int highlight) {
    werase(desc_win); // 창을 지우기
    box(desc_win, 0, 0);
    int desc_x = (getmaxx(desc_win) - strlen(descriptions[highlight - 1])) / 2; // 가운데 정렬
    mvwprintw(desc_win, 1, 1, "%s", descriptions[highlight - 1]);
    wrefresh(desc_win);
}