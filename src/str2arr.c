#include <stdbool.h>
#include <stdlib.h>

bool is_charset(char c, const char *charset) {
    for (int i = 0; charset[i] != '\0'; i++) {
        if (c == charset[i])
            return true;
    }  
    return false;
}

int count_rows(char *str, const char *charset) {
    int rows = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (is_charset(str[i], charset))
            rows++;
    }
    return rows;
}

int *count_cols(char *str, int rows, const char *charset) {
    int *cols = malloc(sizeof(int) * rows);
    int x = 0;

    for (int y = 0; y < rows; y++) {
        for (;!is_charset(str[x], charset); x++)
            cols[y]++;
        x++;
    }
    return cols;
}

char **str2arr(char *str, const char *charset) {
    char **arr = NULL;
    int *cols = NULL;
    int rows = 0;
    int w = 0;
    
    rows = count_rows(str, charset);
    if (rows == 0)
        return NULL;
    cols = count_cols(str, rows, charset);
    if (cols == NULL)
        return NULL;
    arr = malloc(sizeof(char *) * (rows + 1));
    if (arr == NULL)
        return NULL;
    for (int y = 0; y < rows; y++) {
        arr[y] = malloc(sizeof(char) * cols[y]);
        if (arr[y] == NULL)
            return NULL;
        for (int x = 0; x < cols[y]; x++, w++)
            arr[y][x] = str[w];
        w++;
    } 
    arr[rows] = NULL;
    return arr;
}
