#include "tel.h"

char *inputCommand(int fd) { // (ctrl+d)EOF -> NULL, (Enter or spaceString)"\n" -> "\0"
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

int commandScreen(int choice) {
    if (choice == ADD)
        addScreen();
    else if (choice == SEARCH)
        searchScreen();
    else if (choice == DELETE)
        deleteScreen();
    else if (choice == LIST)
        listScreen();
    else if (choice == EXIT)
        return EXIT;
    return SELECT;
}