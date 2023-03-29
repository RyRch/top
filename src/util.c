#include "../icl/top.h"

int my_strlen(char *str) {
    int size = 0;

    while (str[size] != '\0')
        size++;
    return size; 
}

bool my_strcmp(char *s1, char *s2) {
    for (int i = 0; s2[i] != '\0'; i++) {
        if (s1[i] != s2[i])        
            return false;
    }
    return true;
}

bool my_strstr(char *src, char *str) {
    for (int i = 0; src[i] != '\0'; i++) {
        if (my_strcmp(&src[i], str))
            return true;
    }
    return false;
}

char *my_strconc(char *s1, char *s2) {
    char *str = NULL;
    int size = 0;
    int y = 0;
    int w = 0;

    size = my_strlen(s1) + my_strlen(s2);
    str = malloc(sizeof(char) * size + 1);
    if (str == NULL)
        return NULL;
    for (int i = 0; i < size; i++) {
        if (i < my_strlen(s1))
            str[i] = s1[y++];
        else
            str[i] = s2[w++];
    }
    str[y+w] = '\0';
    return str;
}
