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
typedef enum {
    SELECT = 0,
    ADD,
    SEARCH,
    DELETE,
    LIST,
    EXIT
} command;

typedef enum {
    NAME = 1,
    PHONENUMBER,
    GOBACK
} search;

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
void    fileAppend();

// ErrorUtils
void    errorExit(char *errorString);
void    errorPrint(char *errorString);

// StringUtils
char *my_substr(char const *str, unsigned int start, size_t len);
char *my_strjoin(char *s1, char *s2);
char *my_strtrim(char *str);

//  Command
int     commandScreen(int choice);
void    addScreen();
void    deleteScreen();
void    searchScreen();
void    listScreen();
char    *inputCommand(int fd);

//  Screen
void    abledInput();
void    disabledInput();
void    screen();
void    timeScreen();
void    calPage();
void    menuScreen();
int     commandScreen(int choice);
void    addScreen();
void    phoneBookScreen(WINDOW *pb_win);
void    eraseCursor(WINDOW *win, int y, int x, int length);
//  Functions
t_data  *lstLast(t_data *node);
int     lstCount(t_data *node);
void    lstDelete(t_data *del);
void    lstClear(t_data *node);
void    freeArr(char **arr);
char	*getNextLine(int fd);
char    **split(char const *str, char c);
void    lstPrint(t_data *node);
int     printInfo(char *str);
t_data  *makeContact(char **lines);
t_data *checkNameExists(char *name);
t_data *checkPhoneExists(char *phone);

#endif
