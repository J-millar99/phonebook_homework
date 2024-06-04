#include "tel.h"

// 문자열의 일부를 반환하는 함수
char *my_substr(char const *str, unsigned int start, size_t len) {
	char		*result;
	int			idx = 0;
	size_t		str_len;

	if (!str)
        return (NULL); // 널포인터라면 널포인터 반환
	str_len = strlen(str);
	if (str_len < start)
        return (strdup("")); // 문자열 길이보다 시작 지점이 크면 빈 문자열 반환
	if (str_len - start < len) // 자를 문자열 길이가 남은 문자열 길이보다 크면 길이 조정
		len = str_len - start;
	result = (char *)malloc(sizeof(char) * (len + 1));
	while (len-- > 0 && str[start] != '\0')
		result[idx++] = str[start++];
	result[idx] = '\0';
	return (result);
}

// 두 문자열을 붙여 새로운 문자열을 만드는 함수
char	*my_strjoin(char *s1, char *s2) {
	size_t	s1_len, s2_len;
	char	*jstr;

	if (!s1 || !s2) // 둘 중 하나라도 널포인터라면 널포인터 반환
		return (NULL);
	s1_len = strlen(s1); s2_len = strlen(s2);
	jstr = (char *)malloc(sizeof(char) * (s1_len + s2_len + 1)); // +1은 널문자 자리
	if (s1)
		memcpy(jstr, s1, s1_len); // s1 복사
	if (s2)
		memcpy(jstr + s1_len, s2, s2_len); // s2 복사
	jstr[s1_len + s2_len] = '\0';   // 널문자 추가
	return (jstr);
}

// 문자열 앞 뒤 공백을 잘라주는 함수
char *my_strtrim(char *str) {
    if (!str)
        return NULL; // 널 포인터라면 널반환
    while (isspace(*str))
        str++; // 앞 공백 건너뛰기
    if (!*str) // 공백 문자만 있었다면 빈 문자열 반환
        return strdup("");

    char *end = str + strlen(str) - 1;	// 문자열 끝 지점 계산
    while (end > str && isspace(*end))
        end--; // 뒤에서부터 공백 문자를 건너 앞으로 옮김

    size_t len = end - str + 1;	    // 새로운 문자열 길이 계산

    // 결과 문자열 할당 및 복사
    char* trimmed_str = (char*)malloc(len + 1);
    strncpy(trimmed_str, str, len);
    trimmed_str[len] = '\0';
    return trimmed_str;
}

// 전화번호가 유효한지 검사하는 함수
int isValidPhone(const char *str) {
    while (*str) {
        if (!isdigit(*str) && *str != ' ') {
            return 0; // 문자열에 숫자나 공백이 아닌 문자가 있음
        }
        str++;
    }
    return 1; // 문자열이 유효한 전화번호 형식임
}