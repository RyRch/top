#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

char *open_read(const char *file) {
    char *buf = NULL;
    long count = 0;
    long r;
    FILE *fp = NULL;

    fp = fopen(file, "ra");
    if (fp == NULL) {
        return NULL;
    }
	if (fseek(fp, 0, SEEK_END) != 0) {
		fclose(fp);
		return NULL;
	}
	count = ftell(fp);
	if (count == -1) {
		fclose(fp);
		return NULL;
	}
	if (fseek(fp, 0, SEEK_SET) != 0) {
		fclose(fp);
		return NULL;
	}
    buf = malloc(sizeof(char) * (count + 2));
    if (buf == NULL) {
        fclose(fp);
        return NULL;
    }
    r = fread(buf, 1, count, fp);
    if (r != count) {
        fclose(fp);
        free(buf);
        return NULL;
    }
    buf[count] = '\0';
    fclose(fp);
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
