#include "tel.h"

static void initNewPhoneBook(t_data **newPhoneBook, char *f(t_data *node)); // 새로운 전화번호부 초기화
static t_data *findNodeByName(t_data *node, char *minName); // 이름으로 노드 찾기
static char *getMinNameNode(t_data *node);  // 이름이 가장 작은 노드 찾기
static char *getMaxNameNode(t_data *node);  // 이름이 가장 큰 노드 찾기
static void sortPhoneBook(char *f(t_data *node));   // 전화번호부 정렬
static void forwardOrReverse(WINDOW *listwin, int highlight);   // 정렬 방향 선택

static char *sequence[] = { // 정렬 방향
    "FORWARD",
    "REVERSE",
};

void listScreen() {
    WINDOW *listwin = newwin(LINES / 2, COLS / 2 - 2, 1, COLS / 2 + 1); // 목록 창
    WINDOW *pb_win = newwin(LINES - 2, COLS / 2 - 2, 1, 1); // 연락처 창
    int choice = SELECT;
    int highlight = 1;
    int c;
    keypad(listwin, TRUE);
    while (choice == SELECT) {  // 선택 단계라면 계속 반복
        disabledInput();    // 입력 비활성화
        phoneBookScreen(pb_win);    // 연락처 출력
        forwardOrReverse(listwin, highlight);   // 정렬 방향 선택
        c = wgetch(listwin);    // 목록 창에서 입력 받기
        switch (c)  
        {
        case KEY_RIGHT:
            highlight = REVERSE;    // 오른쪽 방향키
            break;  
        case KEY_LEFT:
            highlight = FORWARD;    // 왼쪽 방향키
            break;
        case 10: // 엔터키
            choice = highlight;
            break;
        default:
            break;
        }
        if (choice != SELECT) {
            if (choice == FORWARD)
                sortPhoneBook(getMinNameNode);  // 이름이 가장 작은 노드를 찾아 정렬
            else    // choice == REVERSE
                sortPhoneBook(getMaxNameNode);  // 이름이 가장 큰 노드를 찾아 정렬
            werase(listwin); wrefresh(listwin); delwin(listwin);
            break;
        }
    }
}

 // 정렬 방향 선택
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
        mvwprintw(listwin, height / 2, width / 4 + width / 3, sequence[1]);
    } else {    // highlight == REVERSE
        mvwprintw(listwin, height / 2, width / 4, sequence[0]);
        wattron(listwin, A_REVERSE);
        mvwprintw(listwin, height / 2, width / 4 + width / 3, sequence[1]);
        wattroff(listwin, A_REVERSE);
    }
    wrefresh(listwin);
}

// 전화번호부 정렬
static void sortPhoneBook(char *f(t_data *node)) {
    t_data *newPhoneBook = NULL;
    t_data *find;

    if (lstCount(phoneBook) < 2)    // 연락처가 2개 미만이면 종료
        return ;
    initNewPhoneBook(&newPhoneBook, f); // 새로운 전화번호부 초기화
    while (lstCount(phoneBook)) { // Contact의 개수가 존재하면 아직 정렬이 완료되지 않음
        find = findNodeByName(phoneBook, f(phoneBook)); // 이름으로 노드 찾기
        lstDelete(find);    // 노드 삭제
        lstLast(newPhoneBook)->next = find; // 마지막 노드에 연결
    }
    phoneBook = newPhoneBook;   // 전화번호부 갱신
}

static void initNewPhoneBook(t_data **newPhoneBook, char *f(t_data *node)) {    // 새로운 전화번호부 초기화
    t_data *find;

    if (lstCount(phoneBook)) {  // 연락처가 존재하면
        find = findNodeByName(phoneBook, f(phoneBook)); // 이름으로 노드 찾기
        lstDelete(find);    // 노드 삭제
        *newPhoneBook = find;   // 새로운 전화번호부에 연결
    }
}

// 이름으로 노드 찾기
static t_data *findNodeByName(t_data *node, char *minName) { 
    if (!minName)
        return NULL;
    while (node) {
        if (!strcmp(minName, node->name))
            break;
        node = node->next;
    }
    return node;
}

static char *getMinNameNode(t_data *node) {   // 이름이 가장 작은 노드 찾기
    char *minName;

    if (!node)
        return NULL;
    minName = node->name;
    while (node) {
        if (strcmp(minName, node->name) > 0)
            minName = node->name;
        node = node->next;
    }
    return minName;
}

static char *getMaxNameNode(t_data *node) {  // 이름이 가장 큰 노드 찾기
    char *maxName;

    if (!node)
        return NULL;
    maxName = node->name;
    while (node) {
        if (strcmp(maxName, node->name) < 0)
            maxName = node->name;
        node = node->next;
    }
    return maxName;
}