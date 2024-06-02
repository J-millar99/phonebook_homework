#include "tel.h"

static void initNewPhoneBook(t_data **newPhoneBook, char *f(t_data *node));
static t_data *findNodeByName(t_data *node, char *minName);
static char *getMinNameNode(t_data *node);
static char *getMaxNameNode(t_data *node);
static void sortPhoneBook(char *f(t_data *node));
static void forwardOrReverse(WINDOW *listwin, int highlight);

static char *sequence[] = {
    "FORWARD",
    "REVERSE",
};

void listScreen() {
    WINDOW *listwin = newwin(LINES / 2, COLS / 2 - 2, 1, COLS / 2 + 1);
    WINDOW *pb_win = newwin(LINES - 5, COLS / 2, 1, 1);
    int choice = SELECT;
    int highlight = 1;
    int c;
    keypad(listwin, TRUE);
    while (choice == SELECT) {
        disabledInput();
        phoneBookScreen(pb_win);
        forwardOrReverse(listwin, highlight);
        c = wgetch(listwin);
        switch (c)
        {
        case KEY_RIGHT:
            highlight = REVERSE;
            break;
        case KEY_LEFT:
            highlight = FORWARD;
            break;
        case 10: // 엔터키
            choice = highlight;
            break;
        default:
            break;
        }
        if (choice != SELECT) {
            if (choice == FORWARD)
                sortPhoneBook(getMinNameNode);
            else    // choice == REVERSE
                sortPhoneBook(getMaxNameNode);
            werase(listwin);
            wrefresh(listwin);
            delwin(listwin);
            break;
        }
    }
}

static void forwardOrReverse(WINDOW *listwin, int highlight) {
    int width, height;
    getmaxyx(listwin, height, width);
    werase(listwin);
    box(listwin, 0, 0); // 보조 창 테두리 그리기
    mvwprintw(listwin, 1, (width - strlen("Choice")) / 2, "Choice");
    mvwprintw(listwin, height / 2, width / 2, "/");
    if (highlight == FORWARD) {
        wattron(listwin, A_REVERSE);
        mvwprintw(listwin, height / 2, width / 4, sequence[0]);
        wattroff(listwin, A_REVERSE);
        mvwprintw(listwin, height / 2, width / 4 + width / 2, sequence[1]);
    } else {    // highlight == REVERSE
        mvwprintw(listwin, height / 2, width / 4, sequence[0]);
        wattron(listwin, A_REVERSE);
        mvwprintw(listwin, height / 2, width / 4 + width / 2, sequence[1]);
        wattroff(listwin, A_REVERSE);
    }
    wrefresh(listwin);
}

static void sortPhoneBook(char *f(t_data *node)) {
    t_data *newPhoneBook;
    t_data *find;

    initNewPhoneBook(&newPhoneBook, f);
    while (lstCount(phoneBook)) { // Contact의 개수가 존재하면 아직 정렬이 완료되지 않음
        find = findNodeByName(phoneBook, f(phoneBook));
        lstDelete(find);
        lstLast(newPhoneBook)->next = find;
    }
    phoneBook = newPhoneBook;
}

static void initNewPhoneBook(t_data **newPhoneBook, char *f(t_data *node)) {
    t_data *find;

    if (lstCount(phoneBook)) {
        find = findNodeByName(phoneBook, f(phoneBook));
        lstDelete(find);
        *newPhoneBook = find;
    }
}

static t_data *findNodeByName(t_data *node, char *minName) {
    while (node) {
        if (!strcmp(minName, node->name))
            break;
        node = node->next;
    }
    return node;
}

static char *getMinNameNode(t_data *node) {
    char *minName;

    minName = node->name;
    while (node) {
        if (strcmp(minName, node->name) > 0)
            minName = node->name;
        node = node->next;
    }
    return minName;
}

static char *getMaxNameNode(t_data *node) {
    char *minName;

    minName = node->name;
    while (node) {
        if (strcmp(minName, node->name) < 0)
            minName = node->name;
        node = node->next;
    }
    return minName;
}