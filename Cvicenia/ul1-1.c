#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define INT_SIZE sizeof(int)

int main() {
    int *input = NULL;
    char ch;
    int scenarios, profit = 0, temp, days;

    scanf("%d\n", &scenarios);

    if (scenarios <= 0) {
        return 0;
    }

    while (scenarios) {
        scanf("%d%c", &temp, &ch);
        days = temp;

        input = malloc(days * INT_SIZE);

        for (int i = 0; i < days; i++) {
            scanf("%d%c", &temp, &ch);
            input[i] = temp;
        }

        for (int j = 0; j < days - 1; j++) {
            if (input[j] < input[j+1]) {
                profit += input[j+1] - input[j];
            }
        }    
            
        printf("%d\n", profit);
        profit = 0;
        free(input);

        scenarios--;
    }

    return 0;
}