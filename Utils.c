#include "tel.h"

// 연결리스트의 마지막 노드를 반환하는 함수
t_data *lstLast(t_data *node) {
    if (!node)
        return node;
    while (node->next)
        node = node->next;
    return node;
}

int lstCount(t_data *node) {
    int cnt = 0;

    if (!node)
        return 0;
    while (node) {
        ++cnt;
        node = node->next;
    }
    return cnt;
}

void lstDelete(t_data *del) {
    t_data *prev = phoneBook;
    t_data *temp = phoneBook;

    while (temp) {
        if (prev == del) {
            phoneBook = del->next;
            del->next = NULL;
            return ;
        } else if (temp == del) {
            prev->next = del->next;
            del->next = NULL;
            return ;
        }
        prev = temp;
        temp = temp->next;
    }
}

void lstClear() {
    t_data *temp;

    while (phoneBook) {
        temp = phoneBook;
        phoneBook = phoneBook->next;
        free(temp);
    }
}

int printInfo(char *str) {
    write(1, str, strlen(str));
    write(1, "\n", 1);
    return 1;
}

// 2차원 배열을 메모리 해제하는 함수
void freeArr(char **arr) {
    while (*arr)
        free(*arr++);
}

// 연락처 구조체를 동적할당하는 함수
t_data *makeContact(char **lines) {
    t_data *temp = (t_data *)malloc(sizeof(t_data));
    strncpy(temp->name, lines[0], NAME_LEN);
    strncpy(temp->phone, lines[1], PHONE_LEN);
    if (lines[2]) // memo 필드는 존재할수도 없을수도 있기 때문에 분기점을 구분
        strncpy(temp->memo, lines[2], MEMO_LEN);
    else
        temp->memo[0] = '\0';
    temp->next = NULL;
    return temp;
}

void disabledInput() {
    cbreak();             // 라인 버퍼링을 비활성화, 즉시 입력을 받을 수 있게 함
    noecho();             // 입력된 문자를 화면에 표시하지 않음
    curs_set(FALSE);      // 커서를 보이지 않게 함
}

void abledInput() {
    nocbreak();            // 라인 버퍼링 활성화
    echo();                // 입력된 문자를 화면에 표시
    curs_set(TRUE);        // 커서를 보이게 함
}

void eraseCursor(WINDOW *win, int y, int x, int length) {
    wmove(win, y, x);
    for (int i = 0; i < length; i++)
        waddstr(win, " ");
    wrefresh(win);
}
