#include "tel.h"

// 연결리스트의 마지막 노드를 반환하는 함수
t_data *lstLast(t_data *node) {
    if (!node)
        return node;
    while (node->next)
        node = node->next;
    return node;
}

// 연결리스트의 데이터 개수를 반환하는 함수
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

// 연결리스트의 노드를 삭제하는 함수
// 연결리스트의 연결만 끊어줄 뿐 할당 해제는 호출부에서
void lstDelete(t_data *del) {
    t_data *prev = phoneBook;
    t_data *temp = phoneBook;

    if (!del)
        return ;
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

// 연결리스트를 순회하며 노드를 해제하고 삭제
void lstClear(t_data *node) {
    t_data *temp;

    while (node) {
        temp = node;
        node = node->next;
        free(temp);
    }
}

// 2차원 배열을 메모리 해제하는 함수
void freeArr(char **arr) {
    while (*arr)
        free(*arr++);
}

// 이름존재 확인 함수
t_data *checkNameExists(char *name) {
    t_data *contact = phoneBook;
    while (contact) { // 연락처를 순회하여 이름을 찾음
        if (!strcmp(contact->name, name))
            return contact; // 해당하는 연락처 포인터를 반환
        contact = contact->next;
    }
    return NULL; // 없을경우 널포인터 반환
}

// 전화번호존재 확인 함수
t_data *checkPhoneExists(char *phone) {
    t_data *contact = phoneBook;
    while (contact) { // 연락처를 순회하여 연락처를 찾음
        if (!strcmp(contact->phone, phone))
            return contact; // 해당하는 연락처 포인터를 반환
        contact = contact->next;
    }
    return NULL; // 없을경우 널포인터 반환
}

// 중복되는 이름 연결리스트를 만드는 함수
t_data *nameContactList(char *name) {
    t_data *head = phoneBook;
    t_data *new_head = NULL; // 새 리스트의 헤드
    t_data *new_tail = NULL; // 새 리스트의 꼬리

    while (head) {
        if (!strcmp(head->name, name)) {
            t_data *new_node = (t_data *)malloc(sizeof(t_data));
            strncpy(new_node->name, head->name, NAME_LEN - 1); // 새 노드에 데이터 복사
            strncpy(new_node->phone, head->phone, PHONE_LEN - 1);   // 새 노드에 데이터 복사      
            strncpy(new_node->memo, head->memo, MEMO_LEN - 1);     // 새 노드에 데이터 복사 
            new_node->next = NULL;

            // 새 리스트에 새 노드를 추가
            if (new_head == NULL) { // 새 리스트가 비어있다면
                new_head = new_node;    // 새 노드를 헤드로 설정
                new_tail = new_node;    // 새 노드를 꼬리로 설정
            } else {
                new_tail->next = new_node;  // 새 노드를 꼬리에 연결
                new_tail = new_node;    // 새 노드를 꼬리로 설정
            }
        }
        head = head->next;
    }
    return new_head;
}

// 중복되는 전화번호 연결리스트를 만드는 함수
t_data *phoneContactList(char *phone) {
    t_data *head = phoneBook;
    t_data *new_head = NULL; // 새 리스트의 헤드
    t_data *new_tail = NULL; // 새 리스트의 꼬리

    while (head) {
        if (!strcmp(head->phone, phone)) {
            t_data *new_node = (t_data *)malloc(sizeof(t_data));    // 새 노드 생성
            strcpy(new_node->name, head->name); // 새 노드에 데이터 복사
            strcpy(new_node->phone, head->phone);   // 새 노드에 데이터 복사
            strcpy(new_node->memo, head->memo); // 새 노드에 데이터 복사
            new_node->next = NULL;  // 새 노드의 next를 NULL로 초기화

            // 새 리스트에 새 노드를 추가
            if (new_head == NULL) { // 새 리스트가 비어있다면
                new_head = new_node;    // 새 노드를 헤드로 설정
                new_tail = new_node;    // 새 노드를 꼬리로 설정
            } else {
                new_tail->next = new_node;  // 새 노드를 꼬리에 연결
                new_tail = new_node;    // 새 노드를 꼬리로 설정
            }
        }
        head = head->next;
    }
    return new_head;        // 새 리스트의 헤드 반환
}

void stringEmit(t_data *node, char *name, char *phone) {
    strncat(name, node->name, 7);
    if (strlen(node->name) > 7)
        strcat(name, "...");
    strncat(phone, node->phone, 14);
    if (strlen(node->phone) > 14)
        strcat(phone, "...");
}
