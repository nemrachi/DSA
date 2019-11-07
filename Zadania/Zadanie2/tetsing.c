#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LIMIT 100

int get_next_prime(int size) {
    int is_prime = 1;
    size += 1;

    while (size < 100) {
        for(int i = 2; i <= size/2; ++i) {
            if(size % i == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            return size;
        }

        size += 1;
        is_prime = 1;
    }

    return 0;
}

int main()
{
    printf("%d\n", get_next_prime(20));


    return 0;
}