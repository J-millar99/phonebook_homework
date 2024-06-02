#include "tel.h"

static void deleteName(t_data **head);
static void deletePhone(t_data **head);

void deleteScreen() {
    // char *cmd;
    // int n;

    // printf("1. 이름으로 삭제\n2. 전화번호로 삭제\n이외의 입력은 삭제을 종료합니다.\n입력 : ");
    // cmd = inputCommand(0);
    // n = atoi(cmd);
    // switch (n)
    // {
    // case 1:
    //     deleteName(phoneBook);
    //     break;
    // case 2:
    //     deletePhone(phoneBook);
    //     break;
    // default:
    //     errorPrint("잘못된 입력으로 삭제을 종료합니다.");
    // }
    // free(cmd);
    sleep(10);
}

static void deleteName(t_data **head) {
    t_data *node = *head;
    t_data *prev = NULL;
    char *input = inputCommand(0);
    if (!*input)
        return errorPrint("찾으려는 이름을 입력하지 않았습니다.\n");
    while (node && strcmp(node->name, input)) {
        prev = node;
        node = node->next;
    }
    free(input);
    if (!node) return errorPrint("해당하는 이름이 없습니다.\n");
    if (!prev)
        *head = node->next;
    else
        prev->next = node->next;
    printf("<삭제 결과>\n%s:%s:%s 데이터를 전화번호부에서 삭제했습니다!\n\n", node->name, node->phone, node->memo);
    free(node);
}

static void deletePhone(t_data **head) {
    t_data *node = *head;
    t_data *prev = NULL;
    char *input = inputCommand(0);
    if (!*input)
        return errorPrint("찾으려는 이름을 입력하지 않았습니다.\n");
    while (node && strcmp(node->phone, input)) {
        prev = node;
        node = node->next;
    }
    free(input);
    if (!node) return errorPrint("해당하는 전화번호가 없습니다.\n");
    if (!prev)
        *head = node->next;
    else
        prev->next = node->next;
    printf("<삭제 결과>\n%s:%s:%s 데이터를 전화번호부에서 삭제했습니다!\n\n", node->name, node->phone, node->memo);
    free(node);
}