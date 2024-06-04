#include "tel.h"

static void printDescription(WINDOW *desc_win); // 설명 출력
static void pageScreen(WINDOW *pbWin, int highlight);  // 페이지 출력
static void delContact(int idx);    // 연락처 삭제
static void yesOrNo(WINDOW *dcwin, int highlight);  // 삭제 여부
static void deleteContactScreen(int idx);  // 연락처 삭제 화면
static void showDetail(WINDOW *detailWin, int idx);  // 상세 정보 출력

static char *decision[] = { // YES, NO
    "YES",
    "NO",
};

static char *menuDel[] = { // 설명
    "KEY_UP : Move up",
    "KEY_DOWN : Move down",
    "KEY_LEFT : Previous page",
    "KEY_RIGHT : Next page",
    "ENTER : Delete",
};

void deleteScreen() {
    WINDOW *delWin = newwin(LINES / 2, COLS / 2 - 2, 1, COLS / 2 + 1);
    WINDOW *pbWin = newwin(LINES - 2, COLS / 2 - 2, 1, 1);
    WINDOW *detailWin = newwin(LINES / 2 - 2, COLS / 2 - 2, LINES / 2 + 1, COLS / 2 + 1);
    int choice = SELECT;
    int highlight = 1;
    int c;

    printDescription(delWin);   // 설명 출력
    while (choice == SELECT) {
        keypad(pbWin, TRUE);   // 키보드 입력을 받을 수 있도록
        calPage();  // 페이지 계산
        pageScreen(pbWin, highlight);  // 삭제할 연락처 페이지 출력
        showDetail(detailWin, highlight - 1 + page * l);    // 상세 정보 출력
        disabledInput();    // 입력 비활성화
        c = wgetch(pbWin);
        switch (c) {
            case KEY_UP:
                if (highlight > 1)
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight < l && highlight < lstCount(phoneBook) - page * l)    // 현재 페이지의 연락처 수보다 작다면
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
            case ENTER: // 엔터키
                choice = highlight;
                break;
            default:
                break;
        }
        if (choice != SELECT) { // 엔터키를 눌렀을 때 루프 종료
            keypad(pbWin, FALSE);
            if (lstCount(phoneBook) == 0)    // 연락처가 없을 때
                errorScreen(detailWin, "No contacts to delete."); // 에러 출력
            else
                deleteContactScreen((highlight - 1) + (page * l));  // 연락처 삭제 화면
        }
    }
    werase(delWin); werase(pbWin); werase(detailWin);
    wrefresh(delWin); wrefresh(pbWin); wrefresh(detailWin);
    delwin(delWin); delwin(pbWin); delwin(detailWin);   // 창 지우기
}

// 설명 출력
static void printDescription(WINDOW *desc_win) {
    box(desc_win, 0, 0);
    for (int i = 0; i < sizeof(menuDel) / sizeof(char *); i++) {
        mvwprintw(desc_win, i + 1, 1, "%s", menuDel[i]);
    }
    wrefresh(desc_win);
}

static void pageScreen(WINDOW *pbWin, int highlight) {
    t_data *head = phoneBook;
    int width, height, idx = 3, i = 0;

    werase(pbWin);
    getmaxyx(pbWin, height, width);
    for (int i = 0; i < page * l; i++)
        head = head->next;
    box(pbWin, 0, 0); // 보조 창 테두리 그리기
    mvwprintw(pbWin, 1, (width - strlen("Contact List")) / 2, "Contact List");
    while (head && idx < height - 2) {
        char name[15] = {0};
        char phone[20] = {0};
        stringEmit(head, name, phone);
        if (highlight == i + 1) {
            wattron(pbWin, A_REVERSE);
            mvwprintw(pbWin, idx++, 2, "N: %s P: %s", name, phone);
            wattroff(pbWin, A_REVERSE);
        } else {
            mvwprintw(pbWin, idx++, 2, "N: %s P: %s", name, phone);
        }
        i++;
        head = head->next;
    }
    mvwprintw(pbWin, height - 2, width / 2 - 3, "<%dpage>", page + 1);
    wrefresh(pbWin);
}

// 연락처 삭제 화면
static void deleteContactScreen(int idx) {
    WINDOW *dcwin = newwin(LINES / 2 - 2, COLS / 2 - 2, LINES / 2 + 1, COLS / 2 + 1);
    int choice = SELECT;
    int highlight = 1;
    int c;
    keypad(dcwin, TRUE);
    while (choice == SELECT) {
        yesOrNo(dcwin, highlight);
        c = wgetch(dcwin);
        switch (c)
        {
        case KEY_RIGHT: // 오른쪽 방향키
            highlight = NO; // NO로 이동
            break;
        case KEY_LEFT:  // 왼쪽 방향키
            highlight = YES;        // YES로 이동
            break;
        case ENTER: // 엔터키
            choice = highlight;
            break;
        default:
            break;
        }
        if (choice != SELECT) {
            if (choice == YES && lstCount(phoneBook) != 0)  // YES를 선택했을 때
                delContact(idx);    // 연락처 삭제
            werase(dcwin); wrefresh(dcwin); delwin(dcwin);
            break;
        }
    }
}

static void delContact(int idx) {   // 연락처 삭제
    t_data *head = phoneBook;
    for (int i = 0; i < idx; i++)
        head = head->next;
    lstDelete(head);
    free(head);
    m = lstCount(phoneBook) / l;    // 페이지 계산
}

// 삭제의 여부를 묻는 창 + 하이라이트 효과
static void yesOrNo(WINDOW *dcwin, int highlight) {
    int width, height;
    getmaxyx(dcwin, height, width);
    werase(dcwin);
    box(dcwin, 0, 0); // 보조 창 테두리 그리기
    mvwprintw(dcwin, 1, (width - strlen("Choice")) / 2, "Choice");
    mvwprintw(dcwin, height / 2, width / 2, "/");
    if (highlight == YES) { // highlight == YES
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

// 상세 정보 출력
static void showDetail(WINDOW *detailWin, int idx) {
    t_data *head = phoneBook;
    int width, height, i = 0;

    werase(detailWin);
    if (lstCount(phoneBook) == 0) { // 연락처가 없을 때
        return ;
    }
    getmaxyx(detailWin, height, width);
    for (int i = 0; i < idx; i++)
        head = head->next;
    box(detailWin, 0, 0); // 보조 창 테두리 그리기
    mvwprintw(detailWin, 1, (width - strlen("Detail")) / 2, "Detail");
    mvwprintw(detailWin, 2, 2, "Name: %s", head->name);
    mvwprintw(detailWin, 3, 2, "Phone: %s", head->phone);
    mvwprintw(detailWin, 4, 2, "Memo: %s", head->memo);
    wrefresh(detailWin);
}