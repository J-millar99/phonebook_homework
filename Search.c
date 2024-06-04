#include "tel.h"

static int searchName(WINDOW *shWin);   //이름으로 찾기
static int searchPhone(WINDOW *swin);   //전화번호로 찾기
static void printMenu(WINDOW *menu_win, int highlight); // 검색메뉴출력
static void findNameResult(char *name); // 이름으로 찾은 결과 출력
static void nameBookScreen(WINDOW *retWin, t_data *tphoneBook); // 이름으로 찾은 결과 출력
static void calTPage(); // 페이지 계산
static int tpage = 0, tl, tm;   // 현재 페이지, 페이지당 출력할 행, 총 페이지

static char *menu[] = { // 검색 메뉴
    "Search Name",
    "Search PhoneNumber",
    "Go Back",
};

void searchScreen() {
    int shX, shY; // 다음은 menuScreen 함수와 매우 유사합니다 (반복 부분은 주석 생략)
    WINDOW *shWin = newwin(LINES - 2, COLS - 2, 1, 1);
    int highlight = 1;
    int choice = SELECT;
    int c;

    keypad(shWin, TRUE);    // 키보드 입력을 받을 수 있도록 
    while (choice == SELECT) {
        printMenu(shWin, highlight);
        disabledInput(); // 입력 비활성화
        c = wgetch(shWin);
        switch (c)
        {
        case KEY_UP:           // 위 방향키
            if (highlight > 1) // 현재 선택지가 1보다 크다면
                --highlight;   // 선택지 감소
            break;
        case KEY_DOWN:
            if (highlight < 3) // 현재 선택지가 3보다 작다면
                ++highlight;   // 선택지 증가
            break;
        case ENTER:             // 엔터키
            choice = highlight;
            break;
        default:
            break;
        }
        if (choice != SELECT) {
            abledInput();   //  이름 혹은 전화번호로 연락처를 찾기위해 활성화
            if (choice == NAME) // 이름으로 검색
                choice = searchName(shWin);
            else if (choice == PHONENUMBER) // 전화번호로 검색
                choice = searchPhone(shWin);
            else if (choice == GOBACK)  // 되돌아가기
                choice = GOBACK;
        }
    }
    werase(shWin); wrefresh(shWin), delwin(shWin);
}

// 이름으로 검색
static int searchName(WINDOW *shWin) {
    werase(shWin); wrefresh(shWin);
    int shX, shY;
    char name[NAME_LEN];
    char *tmp = NULL;
    getmaxyx(shWin, shY, shX);
    box(shWin, 0, 0);
    mvwprintw(shWin, 1, (shX - strlen("Search Contacts By Name")) / 2, "Search Contacts By Name");
    mvwprintw(shWin, 3, 2, "Enter name: ");
    wgetnstr(shWin, name, NAME_LEN - 1);
    tmp = my_strtrim(name);
    if (!*tmp) {
        free(tmp); errorScreen(shWin, "Can't input empty name");
        return searchName(shWin);
    }
    mvwprintwEffect(shWin, 5, 2, "Searching...");
    wrefresh(shWin); sleep(1);
    if (checkNameExists(tmp)) { // 이름이 존재한다면 결과 창을 띄움
        findNameResult(tmp); 
    }
    else // 존재하지 않는다면 에러출력문을 만들고 창을 지움
        errorScreen(shWin, "No such contact");
    free(tmp); werase(shWin); wrefresh(shWin);
    return SELECT;
}

static int searchPhone(WINDOW *shWin) {
    werase(shWin); wrefresh(shWin);
    int shX, shY;
    char phone[PHONE_LEN];
    char *tmp = NULL;
    getmaxyx(shWin, shY, shX);
    box(shWin, 0, 0);
    mvwprintw(shWin, 1, (shX - strlen("Search Contacts By PhoneNumber")) / 2, "Search Contacts By PhoneNumber");
    mvwprintw(shWin, 3, 2, "Enter phone number: ");
    wgetnstr(shWin, phone, PHONE_LEN - 1);
    tmp = my_strtrim(phone);
    if (!*tmp) {
        free(tmp); errorScreen(shWin, "Can't input empty phone number");
        return searchPhone(shWin);
    }
    mvwprintwEffect(shWin, 5, 2, "Searching...");
    wrefresh(shWin);
    sleep(1);
    t_data *head = phoneBook;
    while (head) {
        if (!strcmp(head->phone, phone)) {  // 전화번호가 같은 연락처를 찾았다면
            mvwprintw(shWin, shY / 4 + 3, (shX - 6 - strlen(head->name)) / 4, "Name: %s", head->name);
            mvwprintw(shWin, shY / 4 + 4, (shX - 7 - strlen(head->phone)) / 4, "Phone: %s", head->phone);
            mvwprintw(shWin, shY / 4 + 5, (shX - 6 - strlen(head->memo)) / 4, "Memo: %s", head->memo);
            mvwprintw(shWin, shY - 4, (shX - strlen("Enter is quit")) / 2, "Enter is quit");
            break;
        }
        head = head->next;
    }
    if (!head)  // 찾는 전화번호가 없다면
        mvwprintw(shWin, shY - 4, (shX - strlen("No such contact")) / 2, "No such contact");
    disabledInput(); wgetch(shWin); werase(shWin); free(tmp);
    return SELECT;
}

// 검색 메뉴를 하이라이트로 출력
static void printMenu(WINDOW *shWin, int highlight) {
    int shX, shY;
    getmaxyx(shWin, shY, shX);
    mvwprintw(shWin, 1, (shX - strlen("Search Contacts")) / 2, "Search Contacts");
    box(shWin, 0, 0);
    for (int i = 0; i < 3; ++i) {
        if (highlight == i + 1) { // 현재 선택된 항목을 하이라이트
            wattron(shWin, A_REVERSE);
            mvwprintw(shWin, shY / 4 + i, (shX - strlen(menu[i])) / 2, "%s", menu[i]);
            wattroff(shWin, A_REVERSE);
        }
        else
            mvwprintw(shWin, shY / 4 + i, (shX - strlen(menu[i])) / 2, "%s", menu[i]);
    }
    wrefresh(shWin);
}

//problem
static void findNameResult(char *name) {
    t_data *tphoneBook = nameContactList(name); // 찾는 이름으로만 된 리스트 새롭게 생성하여 반환
    WINDOW *retWin = newwin(LINES - 2, COLS - 2, 1, 1);
    int highlight = 1;                         // 현재 보고있는 메뉴
    int choice = SELECT;                       // 선택된 메뉴 초기값: SELECT
    int c;

    tl = getmaxy(retWin) - 5;                   // 한 페이지당 출력할 행 계산
    tm = lstCount(tphoneBook) / tl;             // 총 페이지 계산
    disabledInput();
    keypad(retWin, TRUE);                      // 키보드 입력을 받을 수 있도록
    while (choice == SELECT) {
        nameBookScreen(retWin, tphoneBook); //problem
        c = wgetch(retWin);                    // 메뉴 창에서 입력 받기
        switch (c) {
        case KEY_LEFT: // 왼쪽 방향키
            tpage--;    // 페이지 감소
            break;
        case KEY_RIGHT: // 오른쪽 방향키
            tpage++;     // 페이지 증가
            break;
        case ENTER:     // ENTER키 입력시 종료
            choice = EXIT;
            break;
        default:
            break;
        }
    }
    lstClear(tphoneBook); werase(retWin); wrefresh(retWin); delwin(retWin);
}

static void nameBookScreen(WINDOW *retWin, t_data *tphoneBook) {
    werase(retWin);  // 창을 지우기
    t_data *head = tphoneBook;   // next로 건너뛰는 포인터 변수
    int width, height, idx = 3; // 좌표 변수와 인덱스 변수
    getmaxyx(retWin, height, width); // 창의 크기 저장
    calTPage();  // 현재 page를 계산하여 갱신
    for (int i = 0; i < tpage * tl; i++)  // 페이지를 넘겨 연결리스트에서 표시할 시작 노드를 구함
        head = head->next;
    box(retWin, 0, 0); // 보조 창 테두리 그리기
    mvwprintw(retWin, 1, (width - strlen("Search Name List")) / 2, "Search Name List");  // 전화번호부 title
    mvwprintw(retWin, 2, (width - 11 - strlen(head->name)) / 2, "Searching: %s", head->name);
    while (head && idx < height - 2) {  // 4번째줄부터, 창 높이 - 1 까지 출력할 공간을 돌며 한줄 한줄 출력
        mvwprintw(retWin, idx++, 2, "Phone: %s, Memo: %s", head->phone, head->memo);
        head = head->next;
    }
    mvwprintw(retWin, height - 2, width / 2 - 3, "<%dpage> Enter is quit", tpage + 1); //   창의 하단에 페이지 표시
    wrefresh(retWin);    // 창 갱신
}

void calTPage() {
    if (tpage < 0) // 페이지가 0보다 작으면
        tpage = tm;   // 페이지를 마지막 페이지로 설정
    else if (tpage > tm) //페이지가 마지막 페이지보다 크면
        tpage = 0;   // 페이지를 0으로 설정
}