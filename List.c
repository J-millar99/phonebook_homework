#include "tel.h"

static void initNewPhoneBook(t_data **newPhoneBook, t_data **phoneBook);
static t_data *findNodeByName(t_data *node, char *minName);
static char *getMinNameNode(t_data *node);

void listScreen() {
    // t_data *newPhoneBook;
    // t_data *find;

    // initNewPhoneBook(&newPhoneBook, phoneBook);
    // while (lstCount(*phoneBook)) { // Contact의 개수가 존재하면 아직 정렬이 완료되지 않음
    //     find = findNodeByName(*phoneBook, getMinNameNode(*phoneBook));
    //     lstDelete(phoneBook, find);
    //     lstLast(newPhoneBook)->next = find;
    // }
    // *phoneBook = newPhoneBook;
    // lstPrint(*phoneBook);
    // printf("\n");
    sleep(10);
}

static void initNewPhoneBook(t_data **newPhoneBook, t_data **phoneBook) {
    t_data *find;

    if (lstCount(*phoneBook)) {
        find = findNodeByName(*phoneBook, getMinNameNode(*phoneBook));
        lstDelete(phoneBook, find);
        *newPhoneBook = find;
    }
}

static t_data *findNodeByName(t_data *node, char *minName) {
    while (node) {
        if (!strcmp(minName, node->name))
            break;
        node = node->next;
    }
    return node;
}

static char *getMinNameNode(t_data *node) {
    char *minName;

    minName = node->name;
    while (node) {
        if (strcmp(minName, node->name) > 0)
            minName = node->name;
        node = node->next;
    }
    return minName;
}