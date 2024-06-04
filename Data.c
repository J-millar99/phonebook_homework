#include "tel.h"

static void initPhoneBook(int fd); // 첫 번째 연락처를 연결리스트에 저장하는 함수

// data.txt 파일을 데이터구조로 옮기는 함수
void saveContact(int fd) {
    char *line;         // 파일의 한 줄을 저장할 문자열
    char **lines;       // 한 줄을 필드로 나누어 담을 문자열
    char *tlines[4];    // strtrim을 통해 데이터 필드에 필요없는 앞, 뒤 공백을 제거

    initPhoneBook(fd);                      // 초기 데이터를 저장한 이후, 이어지는 데이터 구조의 저장 방식은 initPhoneBook함수와 유사합니다
    while ((line = readFd(fd))) {     // fd를 통해 파일에서 한 줄을 읽어옵니다.
        if (!*line) {                       // 빈 줄(의미있는 문자가 없는 경우 ex. 공백문자)인 경우 다음 줄로 넘어갑니다
            free(line); continue;
        }
        checkToken(line);           // 읽은 한 줄이 이름과 전화번호가 비어있지 않은지 확인합니다
        lines = split(line, ':');   // 구분자 ':'를 기준으로 문자열을 잘라
        if (!isValidPhone(lines[1]))
            errorExit("연락처의 전화번호가 올바르지 않습니다"); // 전화번호가 올바르지 않은 경우 에러처리
        tlines[0] = my_strtrim(lines[0]); tlines[1] = my_strtrim(lines[1]); tlines[2] = my_strtrim(lines[2]); // 각 필드에 보관
        tlines[3] = NULL;           // 문자배열의 끝을 표기 -> 메모리 해제의 끝을 알기 위함
        freeArr(lines); free(lines); free(line); // 사용한 문자열들을 해제
        if (checkPhoneExists(tlines[1])) // 전화번호 중복시 에러처리
            errorExit("중복된 전화번호가 존재합니다 data.txt를 다시 확인해주세요"); // 에러 출력문 -> 프로그램 종료
        lstLast(phoneBook)->next = makeContact(tlines); // 연결리스트의 마지막에 데이터를 저장합니다
        freeArr(tlines);    // 다듬은 문자 배열도 해제합니다
    }
}

// 첫 번째 연락처를 연결리스트에 저장하는 함수
// saveContact 함수와 구조가 매우 유사합니다
static void initPhoneBook(int fd) {
    char *line;
    char **lines;
    char *tlines[4];

    if ((line = readFd(fd))) {
        if (!*line) {
            free(line);
            return initPhoneBook(fd);
        }
        checkToken(line);
        lines = split(line, ':');
        if (!isValidPhone(lines[1]))
            errorExit("연락처의 전화번호가 올바르지 않습니다");
        free(line);
        tlines[0] = my_strtrim(lines[0]);
        tlines[1] = my_strtrim(lines[1]);
        tlines[2] = my_strtrim(lines[2]);
        tlines[3] = NULL;
        freeArr(lines); free(lines);
        phoneBook = makeContact(tlines);
        freeArr(tlines);
    }
}

// 연락처 구조체를 동적할당하는 함수
t_data *makeContact(char **lines) {
    t_data *temp = (t_data *)malloc(sizeof(t_data)); // 구조체 1개 동적할당
    // 각각의 필드를 각각의 필드 길이의 맞게 복사한다
    strncpy(temp->name, lines[0], NAME_LEN - 1);        
    strncpy(temp->phone, lines[1], PHONE_LEN - 1);
    if (lines[2]) // memo 필드는 존재할수도 없을수도 있기 때문에 분기점을 구분
        strncpy(temp->memo, lines[2], MEMO_LEN - 1);
    else
        temp->memo[0] = '\0';
    temp->next = NULL;  // 새로운 노드의 끝을 표시
    return temp;
}