#ifndef TEL_H
#define TEL_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ncurses.h>
#include <strings.h>
#include <ctype.h>
#include <pthread.h>
#include <signal.h>

// 각 필드의 크기를 필요에 따라 변형하여 쓸 수 있음
#define NAME_LEN 30
#define PHONE_LEN 20
#define MEMO_LEN 50

#define FORWARD 1
#define REVERSE 2
#define YES 1
#define NO 2

#define ENTER 10

typedef enum {
    SELECT = 0,
    ADD,
    SEARCH,
    DELETE,
    LIST,
    EXIT
} e_command;

typedef enum {
    NAME = 1,
    PHONENUMBER,
    GOBACK
} e_search;

// 3개의 전화번호부 연락처 필드를 가지는 구조체
typedef struct t_data {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char memo[MEMO_LEN];
    struct t_data *next;
} t_data;

extern t_data *phoneBook;
extern int page;
extern int l, m, r;

//  FileUtils
void    inputData();
void    outputData();
char    *readFd(int fd);

//  Data
void    saveContact(int fd);
t_data  *makeContact(char **lines);

// ErrorUtils
void    errorExit(char *errorString);
void    errorPrint(char *errorString);
void    checkToken(char *str);
void    errorScreen(WINDOW *to, char *error);

// StringUtils
char    *my_substr(char const *str, unsigned int start, size_t len);
char    *my_strjoin(char *s1, char *s2);
char    *my_strtrim(char *str);

//  Screen
void    screen();
void    menuScreen();
void    addScreen();
void    searchScreen();
void    deleteScreen();
void    listScreen();
void    phoneBookScreen(WINDOW *pb_win);

//  ScreenUtils
void    calPage();
void    abledInput();
void    disabledInput();
void    eraseCursor(WINDOW *win, int y, int x, int length);
void    mvwprintwEffect(WINDOW *win, int y, int x, char *str);
void    stringEmit(t_data *node, char *name, char *phone);
//  Functions
t_data  *lstLast(t_data *node);
int     lstCount(t_data *node);
void    lstDelete(t_data *del);
void    lstClear(t_data *node);
t_data  *nameContactList(char *name);
t_data  *phoneContactList(char *phone);
void    freeArr(char **arr);
char	*getNextLine(int fd);
char    **split(char const *str, char c);
void    lstPrint(t_data *node);
int     printInfo(char *str);
t_data *checkNameExists(char *name);
t_data *checkPhoneExists(char *phone);

#endif
