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

int nb_proc(void) {
    char *cpuinfo = open_read(cpu_path);
    char **arr = str2arr(cpuinfo, ":\n");
    int rows = count_rows(cpuinfo, ":\n");
    int nb = 0;

    free(cpuinfo);
    for (int y = rows - 1; y > 0; y--) {
        if (my_strstr(arr[y], "processor")) {
            nb = atoi(arr[y+1]) + 1;
            break;
        }
    }
    for (int y = 0; arr[y] != NULL; y++)
        free(arr[y]);
    return nb;
}

void fill_stCores(st_cores *core, en_state STATE) {
    char *cpustat = open_read(cpu_stat);
    char **arr = str2arr(cpustat, "\n");
    char **tab = NULL;
    int nbProc = nb_proc() + 1;

    free(cpustat);
    for (int y = 1, w = 0, u = 0; y < nbProc; y++) {
        tab = str2arr(arr[y], " ");
        if (my_strstr(tab[0], "cpu"))
            core[w++].id = atoi(&tab[0][my_strlen(tab[0]) - 1]);
        for (int i = 1, p = 0; atoi(tab[i]) != 0; i++) {
            core[u].stats[p] = atoi(tab[i]);
			core[u].total[STATE] += core[u].stats[p++];
            free(tab[i]);
        }
        core[u].idle[STATE] = core[u].stats[IDLE];
        u++;
        free(arr[y]);
    }
}

int main(int ac, char **av) {
    (void)ac;
    (void)av;
    st_cores core[nb_proc()];
	long double usage;
    long int diff_total;
    long int diff_idle;

    fill_stCores(core, PREV);
    while (1) {
        fill_stCores(core, CUR);
        for (int u = 0; u < nb_proc(); u++) {
            diff_total = core[u].total[CUR] - core[u].total[PREV];
            diff_idle = core[u].idle[CUR] - core[u].idle[PREV];
            usage = 100.0 * ((diff_total - diff_idle) / diff_total);
            printf("cpu%d : %.1Lf%%\n", core[u].id, usage);
            core[u].total[PREV] = core[u].total[CUR];
            core[u].idle[PREV] = core[u].idle[CUR]; 
        }
        sleep(1);
        printf("\n");
    }
    //window(core);
    return 0;
}

/*
void print_st_cores(st_cores *core) {
    int maxx = 0;
    int maxy = 0;
    int xhalf = 0;
    int nbProc = nb_proc();

    (void)maxy;
    fill_stCores(core);
    getmaxyx(stdscr, maxy, maxx);
    xhalf = maxx/2;
    for (int i = 0; i < nbProc; i++) {
        if (i % 2 == 0) {
            mvprintw(3+i, (xhalf/2),
                    "CPU %d : %ld", core[i].id, core[i].prev_iddle);
            refresh();
        } else {
            mvprintw(3+i-1, (xhalf+xhalf/2), 
                    "CPU %d : %ld", core[i].id, core[i].prev_iddle);
            refresh();
        }
    }
}

void mem_usage(void) {
    char *meminfo = open_read(mem_path);
    char **arr = str2arr(meminfo, ":\n");
    char **mem = NULL;
    
    free(meminfo);
    mem = malloc(sizeof(char *) * 2);
    if (mem == NULL)
        return;
    for (int i = 0; arr[i] != NULL; i++) {
        if (my_strstr(arr[i], "MemTotal"))
            mem[TOTAL] = my_strdup(arr[i+1]);
        if (my_strstr(arr[i], "MemAvailable"))
            mem[AVAIL] = my_strdup(arr[i+1]);
        free(arr[i]);
    }
    printw("Mem : %s/%s", mem[AVAIL], mem[TOTAL]);
    refresh();
    free(mem[0]);
    free(mem[1]);
}

void window(st_cores *core) {
    int ch = 0;

    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    while ((ch = getch()) != 113) {
        clear();
        //mem_usage();
        print_st_cores(core);
        sleep(1);
    }
    refresh();
    endwin();
}
*/
