#include "tel.h"

t_data *phoneBook = NULL;           // 첫 데이터 연결리스트를 NULL 포인터로 시작

// "data.txt"파일을 읽고 데이터 구조에 데이터들을 삽입하는 함수
void inputData() {
    int fd = open("data.txt", O_RDONLY);    // 파일을 읽기 전용으로 오픈
    if (fd == -1)
        errorExit("file open error!");      // 오픈 실패시 프로그램을 종료
    saveContact(fd);
    close(fd);                              // 파일의 내용을 메모리 데이터에 옮겼으므로 파일 디스크립터 해제
}

// 프로그램으로 추가, 삭제, 정렬등을 전화번호부를 파일에 반영하는 함수
void outputData() {
    t_data *node = phoneBook;
    unlink("data.txt"); // 기존파일을 삭제
    int fd = open("data.txt", O_WRONLY | O_CREAT, 0644); // 파일을 쓰기 전용으로 새롭게 만듦
    while (node) {      // 연결리스트의 노드를 탐색하면서, 전화번호부 형식에 맞게 작성
        write(fd, node->name, strlen(node->name)); write(fd, ":", 1);
        write(fd, node->phone, strlen(node->phone)); write(fd, ":", 1);
        write(fd, node->memo, strlen(node->memo));
        node = node->next;
        if (node) // 다음 연락처가 존재하는 경우에만 줄바꿈 문자를 붙임
            write(fd, "\n", 1);
    }
    close(fd); // 작성이 끝났으면 close
}

// fd에서 읽어온 한 줄을 조작하는 함수
char *readFd(int fd) { // (ctrl+d)EOF -> NULL, (Enter or spaceString)"\n" -> "\0"
    char *cmd = getNextLine(fd);
    char *temp = NULL;
    
    if (cmd && cmd[strlen(cmd) - 1] == '\n') {
        cmd[strlen(cmd) - 1] = 0; // 줄바꿈 문자를 제거하여 strcmp를 용이하게 사용
        temp = my_strtrim(cmd);
        free(cmd);
        cmd = temp;
    }
    return cmd;
}