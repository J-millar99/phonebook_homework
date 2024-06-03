#include "tel.h"

static int	isSep(char c, char sep);									//	구분자인지 확인하는 함수
static int	wordCount(const char *s, char c);							//	단어의 개수를 세는 함수
static char	*wordBlock(char const *pstr, char c, unsigned int *start);	//	단어를 블록으로 만드는 함수

char	**split(char const *s, char c) { 								//	문자열을 구분자로 나누는 함수
	char			**word_arr;										//	나눈 문자열을 저장할 이중 포인터
	int				idx = 0, word_cnt;							//	인덱스, 단어의 개수
	unsigned int	start = 0;								//	시작 지점

	if (!s)									//	문자열이 비어있으면
		return (NULL);						//	NULL 반환
	word_cnt = wordCount(s, c);					//	단어의 개수를 세어 word_cnt에 저장
	word_arr = (char **)malloc(sizeof(char *) * (word_cnt + 1));	//	단어의 개수만큼 이중 포인터 할당
	while (idx < word_cnt) {					//	단어의 개수만큼 반복
		word_arr[idx] = wordBlock(s, c, &start);	//	단어를 블록으로 만들어 word_arr에 저장
		++idx;							//	인덱스 증가
	}
	word_arr[idx] = NULL;					//	마지막에 NULL 추가
	return (word_arr);					//	나눈 문자열 반환
}

//	구분자인지 확인하는 함수
static int	isSep(char c, char sep)
{
	if (c == sep)
		return (1);
	return (0);
}

//	단어의 개수를 세는 함수
static int	wordCount(const char *s, char c) {
	int		cnt = 0;

	while (*s) {					//	문자열이 끝날 때까지 반복
		while (isSep(*s, c) && *s != '\0')	//	구분자가 나올 때까지 반복
			s++;				//	다음 문자로 이동
		if (!isSep(*s, c) && *s != '\0')	//	구분자가 아니면
			cnt++;						//	단어의 개수 증가
		while (!isSep(*s, c) && *s != '\0')	//	구분자가 나올 때까지 반복
			s++;					//	다음 문자로 이동
	}
	return (cnt);					//	단어의 개수 반환
}

//	단어를 블록으로 만드는 함수
static char	*wordBlock(char const *pstr, char c, unsigned int *start) {
	char	*word;
	int		len;

	len = 0;
	while (pstr[*start]) {				//	문자열이 끝날 때까지 반복
		while (isSep(pstr[*start], c) && pstr[*start] != '\0')	//	구분자가 나올 때까지 반복
			(*start)++;				//	다음 문자로 이동
		while (!isSep(pstr[*start], c) && pstr[*start] != '\0') {		//	구분자가 나올 때까지 반복
			len++;			//	단어의 길이 증가
			(*start)++;				//	다음 문자로 이동
		}
		word = my_substr(pstr, *start - len, len);		//	단어를 블록으로 만듦
		return (word);				//	블록 반환
	}
	return (NULL);				//	블록이 비어있으면 NULL 반환
}
