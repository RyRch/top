#include <stdbool.h>

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
