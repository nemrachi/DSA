#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define UPPER 100000
#define LOWER 0

int get_random() {
    return (rand() % (UPPER + 1 - LOWER)) + LOWER;
}

int main(){
    int max = 0, min = 100001, num;
    for (int i = 0; i < 200000; i++)
    {
        num = get_random();
        if (max < num){
            max = num;
        }
        if (min > num){
            min = num;
        }
    }

    
    printf("%d %d\n", min, max);

    return 0;
    
}