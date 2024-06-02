#include "tel.h"

static void inputName(char **name);
static void inputPhone(char **phone);
static void inputMemo(char **memo);

void addScreen() {
    int add_x, add_y;
    char contactName[NAME_LEN];
    char contactPhone[PHONE_LEN];
    char contactMemo[MEMO_LEN];
    getmaxyx(stdscr, add_y, add_x);
    WINDOW *addwin = newwin(add_y / 2, add_x / 2, add_y / 4, add_x / 4);

    box(addwin, 0, 0);
    mvwprintw(addwin, 1, (add_x - strlen("Add New Contact")) / 2, "Add New Contact");
    mvwprintw(addwin, 3, (add_x - strlen("Name: ")) / 2, "Name: ");
    wgetstr(addwin, contactName);
    mvwprintw(addwin, 4, (add_x - strlen("Phone: ")) / 2, "Phone: ");
    wgetstr(addwin, contactPhone);
    mvwprintw(addwin, 5, (add_x - strlen("Memo: ")) / 2, "Memo: ");
    wgetstr(addwin, contactMemo);
    wrefresh(addwin);
}

t_data *checkPhoneExists(char *phone) {
    t_data *contact = phoneBook;
    while (contact) { // 연락처를 순회하여 중복되는 연락처를 찾음
        if (!strcmp(contact->phone, phone)) {
            printInfo("이미 존재하는 전화번호가 있습니다!");
            free(phone); return contact; // 중복에 해당하는 연락처 포인터를 반환
        }
        contact = contact->next;
    }
    return NULL; // 중복이 없을경우 널포인터 반환
}

static void inputName(char **name) {
    printInfo("이름입력");
    while ((*name = inputCommand(0))) {
        if (!**name) {  // 올바르지 않은 이름 필드
            free(*name); printInfo("아무것도 입력되지 않음");
        } else
            break ;
    }
    if (!*name) // EOF의 경우 다시 입력받음
        return inputName(name);
}

static void inputPhone(char **phone) {
    printInfo("전화번호입력");
    while ((*phone = inputCommand(0))) {
        if (!**phone) { // 올바르지 않은 전화번호 필드
            free(*phone); printInfo("아무것도 입력되지 않음");
        } else
            break ;
    }
    if (!*phone)
        return inputPhone(phone);
}

static void inputMemo(char **memo) {
    printInfo("메모입력");
    if (!(*memo = inputCommand(0)))
        *memo = strdup("");
}
