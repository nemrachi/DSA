#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *adding(char *str, char element) {
    int count = 0;

    for (int i = 0; i < sizeof(str) ; ++i) {
        if (str[i] == '*') {
            count++;
        }
    }

    char temp[100];
    strcpy(temp, str);

    str = realloc(str, sizeof(str)+count+1);
    strcpy(str, "");
    strcpy(str, temp);
    strcpy(temp, "");

    int size = sizeof(str);

    int j;
    for (j = 0; j < size ; ++j) {
        if (str[j] == '*') {
            strncpy(temp, str+j, size - j);
            str[j] = element;
            strcat(&str[j++], temp);
        }
    }

    str[j] = '\0';

    return str;
}

char *recursion(int n, char *str) {
    char *temp = malloc(100);
    if (n == 1) {
        strcat(temp, "|*\0");
        return temp;
    } else if (n == 2) {
        strcat(temp, "||*=*\0");
        return temp;
    } else {
        str = strcat(adding(recursion(n-1, temp), '|'),
                     adding(recursion(n-2, temp), '='));
        return (str);
    }

}

int main() {
    int input;
    char *str = malloc(100);

    printf("Zadaj druhy rozmer 2x");
    scanf("%d\n", &input);

    printf("%s", recursion(input, str));

    free(str);
    return 0;
}