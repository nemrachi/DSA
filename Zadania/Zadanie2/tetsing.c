#include <stdio.h>
#include <stdlib.h>

void fill_arr(int *arr) {
    for (int j = 0; j < 10; j++) {
        arr[j] = j;
    }
}

int main() {
    int *rand_arr = malloc(10 * sizeof(int));
    fill_arr(rand_arr);

    for (int i = 0; i < 10; ++i) {
        printf("%d\n", rand_arr[i]);
    }

    return 0;
}