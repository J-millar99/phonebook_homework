#include "tel.h"

static int searchName(WINDOW *shwin);
static int searchPhone(WINDOW *swin);
static void searchMenu(WINDOW *shwin);
static void printMenu(WINDOW *menu_win, int highlight);

static char *choices[] = {
    "Search Name",
    "Search PhoneNumber",
    "Go Back",
};

void searchScreen() {
    int sh_x, sh_y;
    getmaxyx(stdscr, sh_y, sh_x);
    WINDOW *shwin = newwin(sh_y, sh_x, 0, 0);

    searchMenu(shwin);
}

static int n_choices = sizeof(choices) / sizeof(char*);

static void searchMenu(WINDOW *shwin) {
    int sh_x, sh_y;
    getmaxyx(shwin, sh_y, sh_x);
    int highlight = 1;
    int choice = SELECT;
    int c;
    keypad(shwin, TRUE);
    while (choice == SELECT) {
        printMenu(shwin, highlight);
        disabledInput();    // 입력 비활성화
        c = wgetch(shwin);
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
                case 10: // 엔터키
                    choice = highlight;
                    break;
                default:
                    break;
            }
            if (choice != SELECT) {// 엔터키를 눌렀을 때 루프 종료
                abledInput();       // 입력 활성화
                werase(shwin);
                wrefresh(shwin);
                if (choice == NAME) 
                    choice = searchName(shwin);
                else if (choice == PHONENUMBER)
                    choice = searchPhone(shwin);
                else if (choice == GOBACK)
                    choice = GOBACK;
            }
        }
    delwin(shwin);
    return;
}

static int searchName(WINDOW *shwin) {
    int sh_x, sh_y, found = 0;
    char Name[NAME_LEN];
    getmaxyx(shwin, sh_y, sh_x);
    box(shwin, 0, 0);
    mvwprintw(shwin, 1, (sh_x - strlen("Search Contacts By Name")) / 2, "Search Contacts By Name");
    mvwprintw(shwin, 3, 2, "Enter name: ");
    wgetnstr(shwin, Name, NAME_LEN - 1);
    mvwprintw(shwin, 5, 2, "Searching...");
    wrefresh(shwin);
    sleep(1);
    t_data *head = phoneBook;
    while (head) {
        if (!strcmp(head->name, Name)) {
            mvwprintw(shwin, 7 + found, 3, "Name: %s", head->name);
            mvwprintw(shwin, 8 + found, 3, "Phone: %s", head->phone);
            mvwprintw(shwin, 9 + found, 3, "Memo: %s", head->memo);
            found+=3;
        }
        head = head->next;
    }
    if (!head && !found)
        mvwprintw(shwin, 7, 2, "No such contact");
    wrefresh(shwin);
    wgetch(shwin);
    werase(shwin);
    return SELECT;
}

static int searchPhone(WINDOW *shwin) {
    int sh_x, sh_y;
    char phone[PHONE_LEN];
    getmaxyx(shwin, sh_y, sh_x);
    box(shwin, 0, 0);
    mvwprintw(shwin, 1, (sh_x - strlen("Search Contacts By PhoneNumber")) / 2, "Search Contacts By PhoneNumber");
    mvwprintw(shwin, 3, 2, "Enter phone number: ");
    wgetnstr(shwin, phone, PHONE_LEN - 1);
    mvwprintw(shwin, 5, 2, "Searching...");
    wrefresh(shwin);
    sleep(1);
    t_data *head = phoneBook;
    while (head) {
        if (!strcmp(head->phone, phone)) {
            mvwprintw(shwin, 7, 3, "Name: %s", head->name);
            mvwprintw(shwin, 8, 3, "Phone: %s", head->phone);
            mvwprintw(shwin, 9, 3, "Memo: %s", head->memo);
            break;
        }
        head = head->next;
    }
    if (!head)
        mvwprintw(shwin, 7, 2, "No such contact");
    wrefresh(shwin);
    wgetch(shwin);
    werase(shwin);
    return SELECT;
}

static void printMenu(WINDOW *shwin, int highlight) {
    int sh_x, sh_y;
    getmaxyx(shwin, sh_y, sh_x);
    mvwprintw(shwin, 1, (sh_x - strlen("Search Contacts")) / 2, "Search Contacts");
    int x, y, i;
    x = 2; y = 1;
    box(shwin, 0, 0);
    for (i = 0; i < n_choices; ++i) {
        if (highlight == i + 1) { // 현재 선택된 항목을 하이라이트
            wattron(shwin, A_REVERSE);
            mvwprintw(shwin, sh_y / 4 + y, x, "%s", choices[i]);
            wattroff(shwin, A_REVERSE);
        } else {
            mvwprintw(shwin, sh_y / 4 + y, x, "%s", choices[i]);
        }
        ++y;
    }
    wrefresh(shwin);
}