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

void lstDelete(t_data **node, t_data *del) {
    t_data *prev = *node;
    t_data *temp = *node;

    while (temp) {
        if (prev == del) {
            *node = del->next;
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

void lstClear(t_data *node) {
    t_data *temp;

    while (node) {
        temp = node;
        node = node->next;
        free(temp);
    }
}

void lstPrint(t_data *node) {
    while (node) {
        write(1, node->name, strlen(node->name)); write(1, ":", 1);
        write(1, node->phone, strlen(node->phone)); write(1, ":", 1);
        write(1, node->memo, strlen(node->memo));
        node = node->next;
        if (node) // 다음 연락처가 존재하는 경우에만 줄바꿈 문자를 붙임
            write(1, "\n", 1);
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