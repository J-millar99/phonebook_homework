#include "tel.h"

static void inputName(WINDOW *addwin, t_data *newContact);
static void inputPhone(WINDOW *addwin, t_data *newContact);
static void inputMemo(WINDOW *addwin, t_data *newContact);

void addScreen() {
    t_data *newContact = (t_data *)malloc(sizeof(t_data));
    int add_x, add_y;
    getmaxyx(stdscr, add_y, add_x);
    WINDOW *addwin = newwin(add_y, add_x, 0, 0);

    box(addwin, 0, 0);
    mvwprintw(addwin, 1, (add_x - strlen("Add A New Contact")) / 2, "Add A New Contact");
    inputName(addwin, newContact);
    inputPhone(addwin, newContact);
    inputMemo(addwin, newContact);
    newContact->next = NULL;
    if (!phoneBook)
        phoneBook = newContact;
    else
        lstLast(phoneBook)->next = newContact;
    werase(addwin);
    wrefresh(addwin);
    delwin(addwin);
}

static void inputName(WINDOW *addwin, t_data *newContact) {
    int add_x, add_y;
    char *tmp;
    getmaxyx(addwin, add_y, add_x);
    mvwprintw(addwin, 1 + add_y / 8, add_x / 8, "Name > ");
    wgetnstr(addwin, newContact->name, NAME_LEN - 1);
    tmp = my_strtrim(newContact->name);
    if (!*tmp) {
        free(tmp);
        return inputName(addwin, newContact);
    }
    strcpy(newContact->name, tmp);
    free(tmp);
}

static void inputPhone(WINDOW *addwin, t_data *newContact) {
    int add_x, add_y;
    char *tmp;
    getmaxyx(addwin, add_y, add_x);
    mvwprintw(addwin, 1 + add_y / 4, add_x / 8, "Phone > ");
    wgetnstr(addwin, newContact->phone, PHONE_LEN - 1);
    tmp = my_strtrim(newContact->phone);
    if (!*tmp) {
        free(tmp);
        return inputPhone(addwin, newContact);
    }
    strcpy(newContact->phone, tmp);
    free(tmp);
    if (checkPhoneExists(newContact->phone)) {
        eraseCursor(addwin, 1 + add_y / 4, add_x / 8 + 8, strlen(newContact->phone));
        return inputPhone(addwin, newContact);
    }
}

static void inputMemo(WINDOW *addwin, t_data *newContact) {
    int add_x, add_y;
    char *tmp;
    getmaxyx(addwin, add_y, add_x);
    mvwprintw(addwin, 1 + add_y / 2, add_x / 8, "Memo > ");
    wgetnstr(addwin, newContact->memo, MEMO_LEN - 1);
    tmp = my_strtrim(newContact->memo);
    strcpy(newContact->memo, tmp);
    free(tmp);
}


t_data *checkPhoneExists(char *phone) {
    t_data *contact = phoneBook;
    while (contact) { // 연락처를 순회하여 중복되는 연락처를 찾음
        if (!strcmp(contact->phone, phone))
            return contact; // 중복에 해당하는 연락처 포인터를 반환
        contact = contact->next;
    }
    return NULL; // 중복이 없을경우 널포인터 반환
}