#include "tel.h"

static int	isSep(char c, char sep);
static int	wordCount(const char *s, char c);
static char	*wordBlock(char const *pstr, char c, unsigned int *start);

char	**split(char const *s, char c) {
	char			**word_arr;
	int				idx = 0, word_cnt;
	unsigned int	start = 0;

	if (!s)
		return (NULL);
	word_cnt = wordCount(s, c);
	word_arr = (char **)malloc(sizeof(char *) * (word_cnt + 1));
	while (idx < word_cnt) {
		word_arr[idx] = wordBlock(s, c, &start);
		++idx;
	}
	word_arr[idx] = NULL;
	return (word_arr);
}

static int	isSep(char c, char sep)
{
	if (c == sep)
		return (1);
	return (0);
}

static int	wordCount(const char *s, char c) {
	int		cnt = 0;

	while (*s) {
		while (isSep(*s, c) && *s != '\0')
			s++;
		if (!isSep(*s, c) && *s != '\0')
			cnt++;
		while (!isSep(*s, c) && *s != '\0')
			s++;
	}
	return (cnt);
}

static char	*wordBlock(char const *pstr, char c, unsigned int *start) {
	char	*word;
	int		len;

	len = 0;
	while (pstr[*start]) {
		while (isSep(pstr[*start], c) && pstr[*start] != '\0')
			(*start)++;
		while (!isSep(pstr[*start], c) && pstr[*start] != '\0') {
			len++;
			(*start)++;
		}
		word = my_substr(pstr, *start - len, len);
		return (word);
	}
	return (NULL);
}
