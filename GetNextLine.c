#include "tel.h"

char	*getNextLine(int fd) {
	char	*buff, *backup;
	int		rbyte = 1;
	char	*tptr;

	buff = (char *)malloc(sizeof(char) * 1 + 1);
	backup = strdup("");
	while (!strchr(backup, '\n')) {
		rbyte = read(fd, buff, 1);
		if (rbyte == 0)
			break ;
		buff[rbyte] = '\0';
		tptr = backup;
		backup = my_strjoin(backup, buff);
		free(tptr);
	}
	free(buff);
	if (!*backup) {
		free(backup);
		return NULL;
	}
	return backup;
}
