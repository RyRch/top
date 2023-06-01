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
    printw("Mem : %s/%s\n", mem[AVAIL], mem[TOTAL]);
    refresh();
    free(mem[0]);
    free(mem[1]);
}

char *clean_swap(char *s) {
    char *cswap = NULL;
    char *str = NULL;
    int u = 0;

    cswap = malloc(sizeof(char) * my_strlen(s) + 1);
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == ' ' || s[i] == '\t')
            cswap[u++] = '?';
        else
            cswap[u++] = s[i];
    }
    cswap[u] = '\0';
    u = 0;
    str = malloc(sizeof(char) * my_strlen(cswap));
    for (int i = 0; cswap[i] != '\0'; i++) {
        if (cswap[i] == '?' && (cswap[i-1] != '?' && cswap[i-1] != '\n'))
            str[u++] = ' ';
        else if (cswap[i] != '?')
            str[u++] = cswap[i];
    }
    str[u] = '\0';
    return str;
}

void swap_usage(void) {
    char *swapinfo = open_read(swap_path);
    char **arr = str2arr(clean_swap(swapinfo), " \n");

    printw("Swap : %s kB /%s kB\n", arr[8], arr[7]);
    refresh();
}

void get_uptime(void) {
    char *uptime = open_read("/proc/uptime");

    printw("Uptime : %s\n", uptime);
    refresh();
    free(uptime);
}

void window(void) {
    int ch = 0;

    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    while ((ch = getch()) != 113) {
        clear();
        mem_usage();
        swap_usage();
        get_uptime();
        //print_st_cores(core);
        sleep(1);
    }
    refresh();
    endwin();
}
int main(int ac, char **av) {
    (void)ac;
    (void)av;
    window();
    return 0;
}

/*
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
        core[u].idle[STATE] = core[u].stats[IDLE] + core[u].stats[IOWAIT];
        u++;
        free(arr[y]);
    }
}

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
*/
