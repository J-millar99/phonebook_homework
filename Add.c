#include "tel.h"

static void inputName(WINDOW *addwin, t_data *newContact);  // 이름입력 함수
static void inputPhone(WINDOW *addwin, t_data *newContact); // 전화번호입력 함수
static void inputMemo(WINDOW *addwin, t_data *newContact);  // 메모입력 함수

void addScreen() {
    t_data *newContact = (t_data *)malloc(sizeof(t_data));
    int addX, addY;
    getmaxyx(stdscr, addY, addX);
    WINDOW *addwin = newwin(addY, addX, 0, 0);

    box(addwin, 0, 0);
    mvwprintw(addwin, 1, (addX - strlen("Add A New Contact")) / 2, "Add A New Contact");
    inputName(addwin, newContact); inputPhone(addwin, newContact); inputMemo(addwin, newContact);
    newContact->next = NULL;        // 각필드에 따라 입력을 받고 구조체 변수에 저장
    if (!phoneBook)                 // 첫 연락처라면 헤드 노드에 연결
        phoneBook = newContact;
    else                            // 그것이 아니라면 마지막 노드까지 넘어가서 발견
        lstLast(phoneBook)->next = newContact;
    m = lstCount(phoneBook) / l;    // page갱신을 위해서 수를 반영
    werase(addwin);
    wrefresh(addwin);
    delwin(addwin);
}

// 이름입력 함수
static void inputName(WINDOW *addwin, t_data *newContact) {
    int add_x, add_y;
    char *tmp;
    getmaxyx(addwin, add_y, add_x);
    mvwprintw(addwin, 1 + add_y / 8, add_x / 8, "Name > ");
    wgetnstr(addwin, newContact->name, NAME_LEN - 1);
    if (strchr(newContact->name, ':')) {    // 이름필드에 구분자를 넣었다면 에러를 출력시키고 다시 입력받음
        errorScreen(addwin, "Can't input ':' in name");
        eraseCursor(addwin, 1 + add_y / 8, add_x / 8, strlen(newContact->name)); // 입력한 이름을 지움
        return inputName(addwin, newContact);
    }
    tmp = my_strtrim(newContact->name);
    if (!*tmp) {    // 이름필드가 비어있으면 오류를 출력시키고 다시 입력받음
        free(tmp); errorScreen(addwin, "Can't input empty name");
        eraseCursor(addwin, 1 + add_y / 8, add_x / 8, strlen(newContact->name)); // 입력한 이름을 지움
        return inputName(addwin, newContact);
    }
    strcpy(newContact->name, tmp);  //  구조체에 복사
    free(tmp);
}

// 전화번호 입력함수
static void inputPhone(WINDOW *addwin, t_data *newContact) {
    int add_x, add_y;
    char *tmp;
    getmaxyx(addwin, add_y, add_x);
    mvwprintw(addwin, 1 + add_y / 4, add_x / 8, "Phone > ");
    wgetnstr(addwin, newContact->phone, PHONE_LEN - 1);
    if (strchr(newContact->phone, ':')) {    // 이름필드에 구분자를 넣었다면 에러를 출력시키고 다시 입력받음
        errorScreen(addwin, "Can't input ':' in phone");
        eraseCursor(addwin, 1 + add_y / 4, add_x / 8, strlen(newContact->phone)); // 입력한 전화번호를 지움
        return inputPhone(addwin, newContact);
    }
    tmp = my_strtrim(newContact->phone);
    if (!*tmp) {    // 전화번호 필드가 비어있으면 오류를 출력시키고 다시 입력받음
        free(tmp); errorScreen(addwin, "Can't input empty phone number");
        eraseCursor(addwin, 1 + add_y / 4, add_x / 8, strlen(newContact->phone)); // 입력한 전화번호를 지움
        return inputPhone(addwin, newContact);
    }
    strcpy(newContact->phone, tmp);
    free(tmp);
    if (checkPhoneExists(newContact->phone)) {  // 전화번호 필드가 중복되면 오류를 출력시키고 다시 입력받음
        errorScreen(addwin, "Can't register duplicate phone number");
        eraseCursor(addwin, 1 + add_y / 4, add_x / 8, strlen(newContact->phone)); // 입력한 전화번호를 지움
        return inputPhone(addwin, newContact);
    }
}

// 메모입력함수
static void inputMemo(WINDOW *addwin, t_data *newContact) {
    int add_x, add_y;
    char *tmp;
    getmaxyx(addwin, add_y, add_x);
    mvwprintw(addwin, 1 + add_y / 2, add_x / 8, "Memo > ");
    wgetnstr(addwin, newContact->memo, MEMO_LEN - 1);
    if (strchr(newContact->memo, ':')) {    // 이름필드에 구분자를 넣었다면 에러를 출력시키고 다시 입력받음
        errorScreen(addwin, "Can't input ':' in memo");
        eraseCursor(addwin, 1 + add_y / 2, add_x / 8, strlen(newContact->memo)); // 입력한 메모를 지움
        return inputMemo(addwin, newContact);
    }
    tmp = my_strtrim(newContact->memo);
    strcpy(newContact->memo, tmp);
    free(tmp);
}