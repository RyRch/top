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
#define freq_path "/sys/devices/system/cpu/cpufreq/policy0/"
#define max_freq_path "cpuinfo_max_freq" 
#define min_freq_path "cpuinfo_min_freq" 

typedef struct {
    int id;
    float freq;
} cores;

int count_chars(const char *file);
char *open_read(const char *file);
int nb_proc(char *cpuinfo, char **arr);
bool is_charset(char c, const char *charset);
int count_rows(char *str, const char *charset);
int *count_cols(char *str, int rows, const char *charset);
char **str2arr(char *str, const char *charset);
bool my_strcmp(char *s1, char *s2);
bool my_strstr(char *src, char *str);
int my_strlen(char *str);
char *my_strconc(char *s1, char *s2);

#endif
