#include <stdio.h>
#include <stdlib.h>
#define INT_SIZE sizeof(int)
#define MAX_PRIME 1299709
//unsigned na double
//prime prepisat na 1, potom brat index!!!
// idu cisla aj dole

char *erathosten(char *primeNums)
{
    primeNums[0] = '0';
    primeNums[1] = '0';

    for (int i = 2; i < MAX_PRIME; i++)
    {
        if (primeNums[i] == '1')
        {
            for (int j = i+i; j < MAX_PRIME; j+= i)
            {
                primeNums[j] = '0';
            }
            
        }
    }
    
    return primeNums;
}

void find(int num, char *primeNum) 
{
    int count = 2;

    if (num == 1) {
        printf("2\n");
        return;
    }

    for (int i = 3; i < MAX_PRIME; i+=2)
    {
        if (primeNum[i] == '1') {
            if (count == num) {
                printf("%d\n", i);
                return;
            }
            count++;
        }
    }
}


int main() 
{
    int temp; 
    char *primeNums = malloc(MAX_PRIME * sizeof(char)),  ch; //primeNums only 0 and 1
    for (int i = 0; i < MAX_PRIME; i++)
    {
        primeNums[i] = '1';
    }

    primeNums = erathosten(primeNums);   
    
    while(scanf("%d%c", &temp, &ch) >= 1) 
    {
        if ((1 <= temp) && (temp <= 100000)) 
        {
            find(temp, primeNums);

        } else {
            printf("Number is out of range\n");
        }
    }
    
    return 0;
}