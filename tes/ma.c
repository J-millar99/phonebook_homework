#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#define MAXLINE 100

typedef struct rec
{
    char name[10];
    double s1, s2, s3;
    double sum;
} rec;

int comparesum(const void *p, const void *q) {
    double i = (*(struct rec *)p).sum;
    double j = (*(struct rec *)q).sum;

    if (i - j > 0)
        return -1;
    else if (i - j < 0)
        return 1;
    else
        return 0;
}

int rcomparesum(const void *p, const void *q) {
    double i = (*(struct rec *)p).sum;
    double j = (*(struct rec *)q).sum;

    if (i - j > 0)
        return 1;
    else if (i - j < 0)
        return -1;
    else
        return 0;
}

int comparename(const void *p, const void *q) {
    char *i = (*(struct rec *)p).name;
    char *j = (*(struct rec *)q).name;

    return strcmp(i,j);
}

int rcomparename(const void *p, const void *q) {
    char *i = (*(struct rec *)p).name;
    char *j = (*(struct rec *)q).name;

    return strcmp(j, i);
}

void printInfo(char *string, struct rec *st, int i, int (*f)(const void *, const void *)) {
    printf("\n%s\n", string);

    rec temp[10];
    memcpy(temp, st, sizeof(rec) * 10);
    qsort(temp, i, sizeof(struct rec), *f);
    for (int j = 0; j < 10; j++)
        printf("%-10s %5.1f %5.1f %5.1f %5.1f\n", temp[j].name, temp[j].s1, temp[j].s2, temp[j].s3, temp[j].sum);
}

int binsearchName(struct rec *st, int n, char *find) {
    int low, high;
    int mid;
 
    low = 0;
    high = n - 1;
    while (low <= high) {
        mid = (low + high) / 2;
        if (strcmp(find, st[mid].name) == 0) {            //탐색 성공
            return mid;        
        }
        else if (strcmp(find, st[mid].name) < 0) {        //탐색 범위를 아래쪽으로
            high = mid - 1;
        }
        else if (strcmp(find, st[mid].name) > 0) {        //탐색 범위를 위쪽으로
            low = mid + 1;
        }
    }
    return -1;                            //탐색 실패
}


int main(void) {
    struct rec s;
    struct rec st[10];

    FILE *fp;
    char *lp;
    char line[MAXLINE];
    int i = 0;
    fp = fopen("data.txt", "r");

    printf("원 데이터와 합\n");
    while (fgets(line, MAXLINE, fp) != NULL) {
        sscanf(line, "%s  %lf %lf %lf", s.name, &s.s1, &s.s2, &s.s3);
        st[i] = s;
        st[i].sum = st[i].s1 + st[i].s2 + st[i].s3;
        printf("%-10s %5.1f %5.1f %5.1f %5.1f\n", st[i].name, st[i].s1, st[i].s2, st[i].s3, st[i].sum);
        ++i;
    }

    printInfo("성적 순", st, i, comparesum);
    printInfo("성적 역순", st, i, rcomparesum);
    printInfo("이름 순", st, i, comparename);
    printInfo("이름 역순", st, i, rcomparename);

    printf("원 데이터와 합\n");
    for (int j = 0; j < 10; j++)
        printf("%-10s %5.1f %5.1f %5.1f %5.1f\n", st[j].name, st[j].s1, st[j].s2, st[j].s3, st[j].sum);
    
    printf("%s", st[binsearchName(st, 10, "name1")].name);
}