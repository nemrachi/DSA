#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_LEN 51

int main() {
    char line[MAX_LEN];
    char *token;

    int scenarios, profit, temp, next, best;
    printf("Zadaj pocet scenarov: ");
    scanf("%d\n", &scenarios);

    for (int i = 0; i < scenarios; ++i) {
        fgets(line, MAX_LEN, stdin);
        token = strtok(line, "\n");
        printf("%0.2f\n", ceil(strlen(token)/2));
    }
    return 0;
}