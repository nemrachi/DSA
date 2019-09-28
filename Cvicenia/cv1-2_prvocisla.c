#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int maxPrime = 1299709,
    *nums, *primeNums, *inputNums,
    temp, maxNumIn = 0, index = 0;
    char ch;

    nums = malloc((maxPrime + 1) * sizeof(int));
    inputNums = malloc(sizeof(int));

    printf("Zadaj cisla:\n");

    while(scanf("%d", &temp) >= 1) {
        inputNums = realloc(inputNums, sizeof(int)*(index+1));
        inputNums[index] = temp;
        index++;
        if (temp > maxNumIn) {
            maxNumIn = temp;
        }
    }

    primeNums = malloc(sizeof(inputNums));

    int max = maxNumIn*maxNumIn;

    for (int i = 0; i <= max; ++i) {
        nums[i] = i;
    }


    for (int j = 2; j <= max ; ++j) {
        if (j == 2) {
            for (int i = 4; i <= max; i+=2) {
                nums[i] = 0;
            }
        } else if (j == 3) {
            for (int i = 9; i <= max; i+=6) {
                nums[i] = 0;
            }
        } else if (j == 5) {
            for (int i = 25; i <= max; i+=20) {
                nums[i] = 0;
            }
        } else if (j == 7) {
            for (int i = 49; i <= max; i+=42) {
                nums[i] = 0; //not true
            }           }
        }
    }


    free(nums);
    free(primeNums);
    free(inputNums);
    return 0;
}