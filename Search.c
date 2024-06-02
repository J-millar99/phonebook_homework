#include "tel.h"

static void searchName(t_data *node);
static void searchPhone(t_data *node);

void searchScreen() {
    // t_data *temp = *phoneBook;
    // char *cmd;
    // int n;

    // printf("1. 이름으로 검색\n2. 전화번호로 검색\n이외의 입력은 검색을 종료합니다.\n입력 : ");
    // cmd = inputCommand(0);
    // n = atoi(cmd);
    // switch (n)
    // {
    // case 1:
    //     searchName(temp);
    //     break;
    // case 2:
    //     searchPhone(temp);
    //     break;
    // default:
    //     errorPrint("잘못된 입력으로 검색을 종료합니다.");
    // }
    // free(cmd);
    sleep(10);
}

static void searchName(t_data *node) {
    int idx = 1;
    char *name = inputCommand(0);
    if (!name) // EOF
        return errorPrint("찾으려는 이름을 입력하지 않았습니다.\n");
    if (!*name) {
        free(name); return errorPrint("찾으려는 이름을 입력하지 않았습니다.\n");
    }
    while (node) {
        if (!strcmp(node->name, name))
            printf("%d. <검색 결과>\n이름: %s\n전화번호: %s\n메모: %s\n\n", idx++, node->name, node->phone, node->memo);
        node = node->next;
    }
    free(name);
    if (!node)
        return errorPrint("해당하는 이름이 없습니다.\n");
}

static void searchPhone(t_data *node) {
    char *input = inputCommand(0);
    if (!input) // EOF
        return errorPrint("찾으려는 전화번호를 입력하지 않았습니다.\n");
    if (!*input) { // "\0"
        free(input); return errorPrint("찾으려는 전화번호를 입력하지 않았습니다.\n");
    }
    while (node && strcmp(node->phone, input))
        node = node->next;
    free(input);
    if (!node)
        return errorPrint("해당하는 전화번호가 없습니다.\n");
    printf("<검색 결과>\n이름: %s\n전화번호: %s\n메모: %s\n\n", node->name, node->phone, node->memo);
}