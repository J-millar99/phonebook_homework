#include "tel.h"

t_data *phoneBook = NULL;
static void initPhoneBook(int fd); // 첫번째 전화번호부에 들어갈 연락처를 저장하는 함수
static void saveContact(int fd); // 이후의 연락처들이 들어갈 함수
static void checkToken(char *str);

// "data.txt"파일을 읽고 데이터 구조에 데이터들을 삽입하는 함수
void inputData() {
    int fd = open("data.txt", O_RDONLY); // 파일을 읽기 전용으로 오픈
    if (fd == -1)
        errorExit("file open error!"); // 오픈 실패시 프로그램을 종료
    saveContact(fd);
    close(fd);
}

// 프로그램으로 추가, 삭제한 전화번호부를 파일에 반영하는 함수
void fileAppend() {
    t_data *node = phoneBook;
    unlink("data.txt"); // 기존파일을 삭제
    int fd = open("data.txt", O_WRONLY | O_CREAT, 0644); // 파일을 쓰기 전용으로 새롭게 만듦
    while (node) { // 전화번호부 형식에 맞게 작성
        write(fd, node->name, strlen(node->name)); write(fd, ":", 1);
        write(fd, node->phone, strlen(node->phone)); write(fd, ":", 1);
        write(fd, node->memo, strlen(node->memo));
        node = node->next;
        if (node) // 다음 연락처가 존재하는 경우에만 줄바꿈 문자를 붙임
            write(fd, "\n", 1);
    }
    close(fd); // 작성이 끝났으면 close
}

// 첫 번째 연락처를 연결리스트에 저장하는 함수
static void initPhoneBook(int fd) {
    char *line;
    char **lines;
    char *tlines[3]; // strtrim을 통해 데이터 필드에 필요없는 공백을 제거

    if ((line = inputCommand(fd))) { // fd를 통해 파일에서 한 줄을 받아오기
        if (!*line) { // 해당하는 경우는 data.txt 파일의 빈 줄인 경우 -> 다음 줄을 읽을 수 있도록 넘어감
            free(line);
            return initPhoneBook(fd);
        }
        checkToken(line);
        lines = split(line, ':'); // split함수를 통해 문자열을 구분자 ':'로 나누어 문자 배열로 만듦
        free(line);
        tlines[0] = my_strtrim(lines[0]);
        tlines[1] = my_strtrim(lines[1]);
        tlines[2] = my_strtrim(lines[2]);
        freeArr(lines); free(lines);
        phoneBook = makeContact(tlines);
        freeArr(tlines);
    }
}

// data.txt 파일을 데이터구조로 옮기는 함수
static void saveContact(int fd) {
    char *line;
    char **lines;
    char *tlines[3]; // strtrim을 통해 데이터 필드에 필요없는 공백을 제거

    initPhoneBook(fd); // 초기 데이터를 저장한 이후, 이어지는 데이터 구조의 저장 방식은 initPhoneBook함수와 유사합니다
    while ((line = inputCommand(fd))) {
        if (!*line) { // 빈 줄인경우 다음 줄로 넘깁니다
            free(line); continue;
        }
        checkToken(line);
        lines = split(line, ':');
        tlines[0] = my_strtrim(lines[0]);
        tlines[1] = my_strtrim(lines[1]);
        tlines[2] = my_strtrim(lines[2]);
        freeArr(lines); free(lines); free(line);
        // if (checkPhoneExists(tlines[1])) // 전화번호 중복시 에러처리
        //     errorExit("data.txt를 다시 확인해주세요!");
        lstLast(phoneBook)->next = makeContact(tlines);
        freeArr(tlines);
    }
}

// 입력된 필드가 이름, 전화번호가 포함되어 있는지 확인
static void checkToken(char *str) {
    while (*str && isspace(*str))
        str++;
    if (!*str || *str == ':')
        errorExit("올바르지 않은 이름 필드가 존재합니다");

    while (*str && *str != ':')
        str++;
    if (!*str)
        errorExit("올바르지 않은 전화번호 필드가 존재합니다");
    str++;
    while (*str && isspace(*str))
        str++;
    if (!*str || *str == ':')
        errorExit("올바르지 않은 전화번호 필드가 존재합니다");
}