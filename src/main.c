#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

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

    fd = open(file, O_RDONLY);
    if (fd == -1) {
        return NULL;
    }
    count = count_chars(file);
    if (count == -1) {
        close(fd);
        return NULL;
    }
    buf = malloc(sizeof(char) * (count + 1));
    if (buf == NULL) {
        close(fd);
        return NULL;
    }
    for (int i = 0; read(fd, &c, 1) == 1; i++)
        buf[i] = c;
    buf[count] = '\0';
    close(fd);
    return buf;
}

int nb_proc(void) {
    printf("%s\n", open_read("/proc/cpuinfo"));
    return 0;
}
/*
void window(void) {
    initscr();
    printw("Hello, World!");
    refresh();
    getch();
    endwin();
}
*/

int main(int ac, char **av) {
    (void)ac;
    (void)av;
    //window();
    nb_proc();
    return 0;
}
