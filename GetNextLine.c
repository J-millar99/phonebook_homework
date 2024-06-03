#include "tel.h"

// 파일 디스크립터를 받아 한 줄씩 읽어오는 함수
char	*getNextLine(int fd) {
	char	*buff, *backup;
	int		rbyte = 1;
	char	*tptr;

	buff = (char *)malloc(sizeof(char) * 1 + 1);	// 한 글자씩 읽어오기 위해 1바이트 할당
	backup = strdup("");							// 읽어온 문자열을 저장할 문자열
	while (!strchr(backup, '\n')) {					// 개행 문자가 나올 때까지 반복
		rbyte = read(fd, buff, 1);					// 한 글자씩 읽어옴
		if (rbyte == 0)								// 파일의 끝이면 반복문 탈출
			break ;									
		buff[rbyte] = '\0';							// buff에 NULL 문자 추가
		tptr = backup;								// backup을 임시로 저장 -> 해제를 위함
		backup = my_strjoin(backup, buff);			// backup에 buff를 이어붙임
		free(tptr);									// 임시 포인터 해제
	}
	free(buff);
	if (!*backup) {									// backup이 비어있으면
		free(backup);								// backup 해제
		return NULL;								// 널 반환
	}
	return backup;									// 이어붙인 문자열 반환
}
