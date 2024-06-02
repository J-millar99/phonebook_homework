#include "tel.h"

static void printDescription(WINDOW *desc_win);
static void pageScreen(WINDOW *pb_win, int highlight);
static void delContact(int idx);
static void yesOrNo(WINDOW *dcwin, int highlight);
void deleteContactScreen(int idx);

static char *decision[] = {
    "YES",
    "NO",
};

static char *descriptions[] = {
    "KEY_UP : Move up",
    "KEY_DOWN : Move down",
    "KEY_LEFT : Previous page",
    "KEY_RIGHT : Next page",
    "ENTER : Delete",
};

void deleteScreen() {
    WINDOW *delwin = newwin(LINES / 2, COLS / 2 - 2, 1, COLS / 2 + 1);
    WINDOW *pb_win = newwin(LINES - 5, COLS / 2, 1, 1);
    int choice = SELECT;
    int highlight = 1;
    int c;

    printDescription(delwin);
    while (choice == SELECT) {
        keypad(pb_win, TRUE);
        calPage();
        pageScreen(pb_win, highlight);
        disabledInput();    // 입력 비활성화
        c = wgetch(pb_win);
        switch (c) {
            case KEY_UP:
                if (highlight > 1)
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight < l && highlight < lstCount(phoneBook) - page * l)
                    ++highlight;
                break;
            case KEY_LEFT:
                page--;
                highlight = 1;
                break;
            case KEY_RIGHT:
                page++;
                highlight = 1;
                break;
            case 10: // 엔터키
                choice = highlight;
                break;
            default:
                break;
        }
        if (choice != SELECT) { // 엔터키를 눌렀을 때 루프 종료
            keypad(pb_win, FALSE);
            deleteContactScreen((highlight - 1) + (page * l));
        }
    }
}

static void printDescription(WINDOW *desc_win) {
    box(desc_win, 0, 0);
    for (int i = 0; i < sizeof(descriptions) / sizeof(char *); i++) {
        mvwprintw(desc_win, i + 1, 1, "%s", descriptions[i]);
    }
    wrefresh(desc_win);
}

static void pageScreen(WINDOW *pb_win, int highlight) {
    t_data *head = phoneBook;
    int width, height, idx = 3, i = 0;

    werase(pb_win);
    getmaxyx(stdscr, height, width);
    width /= 2;
    height -= 5;
    for (int i = 0; i < page * l; i++)
        head = head->next;
    box(pb_win, 0, 0); // 보조 창 테두리 그리기
    mvwprintw(pb_win, 1, (width - strlen("Contact List")) / 2, "Contact List");
    while (head && idx < height - 2) {
        if (highlight == i + 1) {
            wattron(pb_win, A_REVERSE);
            mvwprintw(pb_win, idx++, 1, "Name: %s Phone: %s", head->name, head->phone);
            wattroff(pb_win, A_REVERSE);
        } else {
            mvwprintw(pb_win, idx++, 1, "Name: %s Phone: %s", head->name, head->phone);
        }
        i++;
        head = head->next;
    }
    mvwprintw(pb_win, height - 2, width / 2 - 3, "<%dpage>", page + 1);
    wrefresh(pb_win);
}

void deleteContactScreen(int idx) {
    WINDOW *dcwin = newwin(LINES / 2 - 4, COLS / 2 - 2, LINES / 2 + 1, COLS / 2 + 1);
    int choice = SELECT;
    int highlight = 1;
    int c;
    keypad(dcwin, TRUE);
    while (choice == SELECT) {
        yesOrNo(dcwin, highlight);
        c = wgetch(dcwin);
        switch (c)
        {
        case KEY_RIGHT:
            highlight = NO;
            break;
        case KEY_LEFT:
            highlight = YES;
            break;
        case 10: // 엔터키
            choice = highlight;
            break;
        default:
            break;
        }
        if (choice != SELECT) {
            if (choice == YES)
                delContact(idx);
            werase(dcwin);
            wrefresh(dcwin);
            delwin(dcwin);
            break;
        }
    }
}

static void delContact(int idx) {
    t_data *head = phoneBook;
    for (int i = 0; i < idx; i++)
        head = head->next;
    lstDelete(head);
    free(head);
}

static void yesOrNo(WINDOW *dcwin, int highlight) {
    int width, height;
    getmaxyx(dcwin, height, width);
    werase(dcwin);
    box(dcwin, 0, 0); // 보조 창 테두리 그리기
    mvwprintw(dcwin, 1, (width - strlen("Choice")) / 2, "Choice");
    mvwprintw(dcwin, height / 2, width / 2, "/");
    if (highlight == YES) {
        wattron(dcwin, A_REVERSE);
        mvwprintw(dcwin, height / 2, width / 4, decision[0]);
        wattroff(dcwin, A_REVERSE);
        mvwprintw(dcwin, height / 2, width / 4 + width / 2, decision[1]);
    } else {    // highlight == NO
        mvwprintw(dcwin, height / 2, width / 4, decision[0]);
        wattron(dcwin, A_REVERSE);
        mvwprintw(dcwin, height / 2, width / 4 + width / 2, decision[1]);
        wattroff(dcwin, A_REVERSE);
    }
    wrefresh(dcwin);
}