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

void fill_stCores(st_cores *core) {
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

float *freq_limit(void) {
    float *liFreq = NULL;

    liFreq = malloc(sizeof(int) * 2);
    if (liFreq == NULL)
        return NULL;
    liFreq[0] = atof(open_read((const char *)
                my_strconc(freq_path, max_freq_path)));
    liFreq[1] = atof(open_read((const char *)
                my_strconc(freq_path, min_freq_path)));
    liFreq[0] = liFreq[0] / 1000000;
    liFreq[1] = liFreq[1] / 1000000;
    return liFreq;
}

/*
float get_percentage(float max, float min, float value) {
    return (((value-min) / (max - min)) * 100);
}
*/

void print_st_cores(st_cores *core, int nbProc) {
    float *fl = NULL;
    int maxx = 0;
    int maxy = 0;
    int xhalf = 0;

    (void)maxy;
    fill_stCores(core);
    getmaxyx(stdscr, maxy, maxx);
    xhalf = maxx/2;
    fl = freq_limit();
    for (int i = 0; i < nbProc; i++) {
        if (i % 2 == 0)
            mvprintw(3+i, (xhalf/2),
                    "CPU%d : %.1f GHz", core[i].id, core[i].freq/1000);
        else 
            mvprintw(3+i-1, (xhalf+xhalf/2),
                    "CPU%d : %.1f GHz", core[i].id, core[i].freq/1000);
        refresh();
    }
    free(fl);
}

void mem_usage(void) {
    char *meminfo = open_read(mem_path);
    char **arr = str2arr(meminfo, ":\n");
    char **mem = NULL;
    
    free(meminfo);
    mem = malloc(sizeof(char *) * 2);
    /*
    if (mem == NULL)
        return NULL;
        */
    for (int i = 0; arr[i] != NULL; i++) {
        if (my_strstr(arr[i], "MemTotal"))
            mem[TOTAL] = my_strdup(arr[i+1]);
        if (my_strstr(arr[i], "MemAvailable"))
            mem[AVAILABLE] = my_strdup(arr[i+1]);
        free(arr[i]);
    }
    printw("\n\n   Mem : %s/%s", mem[AVAILABLE], mem[TOTAL]);
    refresh();
    free(mem[0]);
    free(mem[1]);
}

void window(st_cores *core, int nbProc) {
    int ch = 0;

    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    while ((ch = getch()) != 113) {
        clear();
        print_st_cores(core, nbProc);
        mem_usage();
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
    st_cores core[nbProc];

    window(core, nbProc);
    return 0;
}
