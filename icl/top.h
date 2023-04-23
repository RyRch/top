#ifndef _TOP_H_
#define _TOP_H_

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>

#define mem_path "/proc/meminfo"
#define cpu_path "/proc/cpuinfo"
#define cpu_stat "/proc/stat"
#define IDLE 3

typedef enum {
    TOTAL = 0,
    AVAILABLE
} en_mem;

typedef struct {
    int id;
    int stats[10];
	long long int total;
} st_cores;

int count_chars(const char *file);
char *open_read(const char *file);
int nb_proc(void);
bool is_charset(char c, const char *charset);
int count_rows(char *str, const char *charset);
int *count_cols(char *str, int rows, const char *charset);
char **str2arr(char *str, const char *charset);
bool my_strcmp(char *s1, char *s2);
bool my_strstr(char *src, char *str);
char *my_strconc(char *s1, char *s2);
char *my_strdup(char *src);
int my_strlen(char *str);

#endif
