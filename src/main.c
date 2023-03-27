#include "../icl/top.h"
//#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

typedef struct {
    int id;
    float freq;
} cores;

int count_chars(const char *file) {
    int count = 0;
    int fd = 0;
    char c;

    fd = open(file, O_RDONLY);
    if (fd == -1)
        return -1;
    while (read(fd, &c, 1) == 1)
       count++; 
    close(fd);
    return count;
}

char *open_read(const char *file) {
    char *buf = NULL;
    int count = 0;
    int fd = 0;
    char c;

    if (fd == -1)
        return NULL;
    count = count_chars(file);
    if (count == -1)
        return NULL;
    buf = malloc(sizeof(char) * (count + 1));
    if (buf == NULL)
        return NULL;
    fd = open(file, O_RDONLY);
    if (fd == -1)
        return NULL;
    for (int i = 0; read(fd, &c, 1) == 1; i++)
        buf[i] = c;
    buf[count] = '\0';
    close(fd);
    return buf;
}

int nb_proc(char *cpuinfo, char **arr) {
    int rows = count_rows(cpuinfo, ":\n");
    int nb = 0;

    free(cpuinfo);
    for (int y = rows - 1; y > 0; y--) {
        if (my_strstr(arr[y], "processor")) {
            nb = atoi(arr[y+1]) + 1;
            /*
            for (int y = 0; y < rows; y++)
                free(arr[y]);    
                */
            return nb;
        }
    }
    return 0;
}

void fill_scores(cores *core, char **arr) {
    for (int y = 0, w = 0; arr[y] != NULL; y++) {
        if (my_strstr(arr[y], "processor"))
            core[w].id = atoi(arr[y+1]);
        if (my_strstr(arr[y], "cpu MHz"))
            core[w++].freq = atof(arr[y+1]);
    }
}

/*
void window(void) {
    initscr();
    mvprintw(30, 30, "nb of processors : %d", nb_proc());
    refresh();
    getch();
    endwin();
}
*/

int main(int ac, char **av) {
    (void)ac;
    (void)av;
    char *cpuinfo = open_read("/proc/cpuinfo");
    char **arr = str2arr(cpuinfo, ":\n");
    int nbproc = nb_proc(cpuinfo, arr);
    cores core[nbproc];

    fill_scores(core, arr);
    for (int y = 0; y < nbproc; y++) {
        printf("core[%d].id : %d\n", y, core[y].id); 
        printf("core[%d].freq : %f\n", y, core[y].freq); 
    }

//    window();
    return 0;
}
