#include "tel.h"

static void printMenu(WINDOW *menu_win, int highlight);
static void printDescription(WINDOW *desc_win, int highlight);

char *choices[] = {
    "ADD",
    "SEARCH",
    "DELETE",
    "LIST",
    "EXIT",
};

char *descriptions[] = {
    "Add a new Contact.",
    "Search for an Contacts at PhoneBook.",
    "Delete an Contacts at PhoneBook.",
    "List all Contacts at PhoneBook.",
    "Exit the program."
};

int n_choices = sizeof(choices) / sizeof(char*);

void menuScreen() {
    WINDOW *menu_win;
    WINDOW *desc_win;
    WINDOW *pb_win;
    int highlight = 1;
    int choice = SELECT;
    int c;

    menu_win = newwin(LINES / 2, COLS / 2 - 1, 1, COLS / 2 + 1);
    desc_win = newwin(LINES / 2 - 5, COLS / 2 - 1, LINES / 2 + 1, COLS / 2 + 1);
    keypad(menu_win, TRUE);
    refresh();

    printMenu(menu_win, highlight);
    printDescription(desc_win, highlight);
    phoneBookScreen(&pb_win);
    while (choice == SELECT) {
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
            werase(menu_win);
            werase(desc_win);
            werase(pb_win);
            wrefresh(menu_win);
            wrefresh(desc_win);
            wrefresh(pb_win);
            choice = commandScreen(choice);
        }
        printMenu(menu_win, highlight);
        printDescription(desc_win, highlight);
        phoneBookScreen(&pb_win);
    }

    mvprintw(LINES - 2, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
    clrtoeol();
    refresh();
    getch();
}

static void printMenu(WINDOW *menu_win, int highlight) {
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