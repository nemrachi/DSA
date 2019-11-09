#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LIMIT 100

int get_random_range(int lower, int upper) {
    return (rand() % (upper + 1 - lower)) + lower;
}


void get_random_char(char **arr, int num_of_nodes) {
    int num, rand_size;
    for (int i = 0; i < num_of_nodes; ++i) {
        rand_size = get_random_range(1, 10);
        for (int j = 0; j < rand_size; ++j) {
            num = rand() % 3;
            if (num == 0) {
                arr[i][j] = (char)('0' + (rand() % 10));
            } else if (num == 1) {
                arr[i][j] = (char)('a' + (rand() % 26));
            } else {
                arr[i][j] = (char)('A' + (rand() % 26));
            }
        }
        arr[i][rand_size] = '\0';
    }
}

int main()
{
    char str[6];
    sprintf(str, "%d", 100000);

    printf("%d", strlen(str));
    return 0;
}