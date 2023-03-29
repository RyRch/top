#include "../icl/top.h"

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

    count = count_chars(file);
    if (count == -1 || count == 0)
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
            return nb;
        }
    }
    for (int y = 0; arr[y] != NULL; y++)
        free(arr[y]);
    return 0;
}

void fill_stCores(cores *core) {
    char *cpuinfo = open_read(cpu_path);
    char **arr = str2arr(cpuinfo, ":\n");

    free(cpuinfo);
    for (int y = 0, w = 0; arr[y] != NULL; y++) {
        if (my_strstr(arr[y], "processor"))
            core[w].id = atoi(arr[y+1]);
        if (my_strstr(arr[y], "cpu MHz"))
            core[w++].freq = atof(arr[y+1]);
        free(arr[y]);
    }
}

int *freq_limit(void) {
    int *liFreq = NULL;

    liFreq = malloc(sizeof(int) * 2);
    if (liFreq == NULL)
        return NULL;
    liFreq[0] = atoi(open_read((const char *)my_strconc(freq_path, max_freq_path)));
    liFreq[1] = atoi(open_read((const char *)my_strconc(freq_path, min_freq_path)));
    return liFreq;
}

void print_cores(cores *core, int nbProc) {
    int maxx = 0;
    int maxy = 0;
    int xhalf = 0;

    (void)maxy;
    fill_stCores(core);
    getmaxyx(stdscr, maxy, maxx);
    xhalf = maxx/2;
    int *fl = freq_limit();
    printw("max = %d, min = %d", fl[0], fl[1]);
    refresh();
    for (int i = 0; i < nbProc; i++) {
        if (i % 2 == 0)
            mvprintw(3+i, (xhalf/2),
                    "CPU%d : %f MHz", core[i].id, core[i].freq);
        else 
            mvprintw(3+i-1, (xhalf+xhalf/2),
                    "CPU%d : %f MHz", core[i].id, core[i].freq);
        refresh();
    }
}

void window(cores *core, int nbProc) {
    int ch = 0;

    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    while ((ch = getch()) != 113) {
        clear();
        print_cores(core, nbProc);
        sleep(1);
    }
    refresh();
    endwin();
}

int main(int ac, char **av) {
    (void)ac;
    (void)av;
    char *cpuinfo = open_read(cpu_path);
    char **arr = str2arr(cpuinfo, ":\n");
    int nbProc = nb_proc(cpuinfo, arr);
    cores core[nbProc];

    window(core, nbProc);
    return 0;
}
