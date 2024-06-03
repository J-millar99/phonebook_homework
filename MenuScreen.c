#include "tel.h"

static void printMenu(WINDOW *menuWin, int highlight); // 명렁어메뉴 출력 함수
static void printDescription(WINDOW *descWin, int highlight);  // 메뉴 설명 출력 함수
static int command(int choice); // 선택된 메뉴로 이동하는 함수

static char *menu[] = {         // 메뉴 선택지
    "ADD",
    "SEARCH",
    "DELETE",
    "LIST",
    "EXIT",
};

static char *menuDesc[] = {     // 메뉴 설명
    "Add a new Contact.",
    "Search for an Contacts at PhoneBook.",
    "Delete an Contacts at PhoneBook.",
    "List all Contacts at PhoneBook.",
    "Exit the program."
};

void menuScreen() { // 메뉴 화면
    WINDOW *menuWin = newwin(LINES / 2 - 2, COLS / 2 - 2, 1, COLS / 2 + 1);                 // 메뉴 창
    WINDOW *descWin = newwin(LINES / 2 - 2, COLS / 2 - 2, LINES / 2 + 1, COLS / 2 + 1);     // 메뉴 설명 창
    WINDOW *pbWin = newwin(LINES - 2, COLS / 2 - 2, 1, 1);                                  // 연락처 창
    int highlight = 1;                                                                  // 현재 보고있는 메뉴
    int choice = SELECT;                                                                // 선택된 메뉴 초기값: SELECT
    int c;                                                                              // 입력 받을 문자

    l = getmaxy(pbWin) - 5;                     // 한 페이지에 표시할 연락처 개수
    m = lstCount(phoneBook) / l;                // 페이지 수

    keypad(menuWin, TRUE);                      // 키보드 입력을 받을 수 있도록 설정
    while (choice == SELECT) {                  // 선택 단계라면 계속 반복
        printMenu(menuWin, highlight);          // 메뉴 출력
        printDescription(descWin, highlight);   // 메뉴 설명 출력
        phoneBookScreen(pbWin);                 // 연락처 출력
        disabledInput();                        // 입력 비활성화
        c = wgetch(menuWin);                    // 메뉴 창에서 입력 받기
        switch (c) {
            case KEY_UP:                        // 위 방향키
                if (highlight > 1)              // 현재 선택지가 1보다 크다면
                    --highlight;                // 선택지 감소
                break;
            case KEY_DOWN:
                if (highlight < 5)              // 현재 선택지가 5보다 작다면
                    ++highlight;                // 선택지 증가
                break;
            case KEY_LEFT:                      // 왼쪽 방향키
                page--;                         // 페이지 감소
                break;
            case KEY_RIGHT:                     // 오른쪽 방향키
                page++;                         // 페이지 증가
                break;
            case ENTER:                         // 엔터키
                choice = highlight;             // 선택된 메뉴를 현재 선택지로 설정
                break;
            default:                            // 그 외의 키
                break;
        }
        if (choice != SELECT) {     // 엔터키를 눌렀을 때 루프 종료
            abledInput();       // 입력 활성화
            werase(menuWin); werase(descWin); werase(pbWin);        // 창 지우기
            wrefresh(menuWin); wrefresh(descWin); wrefresh(pbWin);  // 창 새로고침
            choice = command(choice);                         // 선택된 메뉴로 이동
        }
    }
    clrtoeol();
}

static int command(int choice) {
    if (choice == ADD)
        addScreen();    // addScreen으로 이동
    else if (choice == SEARCH)
        searchScreen(); // searchScreen으로 이동
    else if (choice == DELETE)
        deleteScreen(); // deleteScreen으로 이동
    else if (choice == LIST)
        listScreen();   // listScreen으로 이동
    else if (choice == EXIT)
        return EXIT;    // EXIT 반환
    return SELECT;      // SELECT 반환
}

//  메뉴 출력 함수
static void printMenu(WINDOW *menuWin, int highlight) {
    werase(menuWin); // 창을 지우기
    int x, y;    // x, y 좌표
    getmaxyx(menuWin, y, x);    // 창의 크기를 얻어옴
    box(menuWin, 0, 0);        // 보조 창 테두리 그리기
    for (int i = 0; i < 5; ++i) {   // 메뉴 선택지 출력
        if (highlight == i + 1) {           // 현재 선택된 항목을 하이라이트
            wattron(menuWin, A_REVERSE);    // REVERSE 속성 켜기
            mvwprintw(menuWin, i + y / 4, (x - strlen(menu[i])) / 2, "%s", menu[i]);    // 선택된 항목 출력
            wattroff(menuWin, A_REVERSE);   // REVERSE 속성 끄기
        }
        else
            mvwprintw(menuWin, i + y / 4, (x - strlen(menu[i])) / 2, "%s", menu[i]);    // 선택되지 않은 항목 출력
    }
    wrefresh(menuWin);
}

//  메뉴 설명 출력 함수
static void printDescription(WINDOW *descWin, int highlight) {
    werase(descWin); // 창을 지우기
    int x, y;       // x, y 좌표
    getmaxyx(descWin, y, x);    // 창의 크기를 얻어옴
    box(descWin, 0, 0); // 보조 창 테두리 그리기
    x = (x - strlen(menuDesc[highlight - 1])) / 2; // 가운데 정렬
    mvwprintw(descWin, y / 2, x, "%s", menuDesc[highlight - 1]);    // 메뉴 설명 출력
    wrefresh(descWin);  // 창 새로고침
}

// preview 전화번호부 출력 함수
void phoneBookScreen(WINDOW *pbWin) {
    werase(pbWin);  // 창을 지우기
    t_data *head = phoneBook;   // next로 건너뛰는 포인터 변수
    int width, height, idx = 3; // 좌표 변수와 인덱스 변수

    getmaxyx(pbWin, height, width); // 창의 크기 저장
    calPage();  // 현재 page를 계산하여 갱신
    for (int i = 0; i < page * l; i++)  // 페이지를 넘겨 연결리스트에서 표시할 시작 노드를 구함
        head = head->next;
    box(pbWin, 0, 0); // 보조 창 테두리 그리기
    mvwprintw(pbWin, 1, (width - strlen("Contact List")) / 2, "Contact List");  // 전화번호부 title
    while (head && idx < height - 2) {  // 3번째줄부터, 창 높이 - 2 까지 출력할 공간을 돌며 한줄 한줄 출력
        // preview에 적절하게 출력할 수 있도록 문자열이 너무 길면 "..."으로 표기할 수 있도록 함
        char name[15] = {0};
        char phone[20] = {0};
        stringEmit(head, name, phone);
        mvwprintw(pbWin, idx++, 2, "N: %s P: %s", name, phone);
        head = head->next;
    }
    mvwprintw(pbWin, height - 2, width / 2 - 3, "<%dpage>", page + 1); //   창의 하단에 페이지 표시
    wrefresh(pbWin);    // 창 갱신
}
