#include <stdbool.h>

int count_chars(const char *file);
char *open_read(const char *file);
int nb_proc(char *cpuinfo, char **arr);
bool is_charset(char c, const char *charset);
int count_rows(char *str, const char *charset);
int *count_cols(char *str, int rows, const char *charset);
char **str2arr(char *str, const char *charset);
bool my_strcmp(char *s1, char *s2);
bool my_strstr(char *src, char *str);
