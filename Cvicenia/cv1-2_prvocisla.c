#include <stdio.h>
#include <stdlib.h>
//unsigned na double
//prime prepisat na 1, potom brat index!!!
int main() {
    int maxPrime = 1299709,
    *resultPrimes, *primeNums, *inputNums,
    temp, maxNumIn = 0, index = 0;
    char ch;

    inputNums = malloc(sizeof(int));
    printf("Zadaj cisla:\n");
    //zadanie cisel poradia prvocisel
    while(scanf("%d%c", &temp, &ch) >= 1) {
        if (1 <= temp <= 100000) {
            inputNums = realloc(inputNums, sizeof(int)*(index+1));
            inputNums[index] = temp;
            index++;
            //uklada si najvacsie poradove cislo
            if (temp > maxNumIn) {
                maxNumIn = temp;
            }
        } else {
            printf("Number is out of range\n");
        }
    }

    int sizeNums = maxPrime; //cca celkovy pocet cisel

    primeNums = malloc((sizeNums) * sizeof(int));

    //vypis postupnosti cisel
    for (int i = 2; i < sizeNums ; ++i) {
        primeNums[i] = i;
    }

    //oznaci neprvocisla 0

    for (unsigned int j = 2; j < sizeNums; j++) {
        if (primeNums[j]) {
            for (unsigned int i = j; i*j < sizeNums ; i++) {
                primeNums[i*j] = 0;
            }
        }
    }

    resultPrimes = malloc(sizeof(inputNums));
    int index2 = inputNums[0];
    int index3 = 1;
    int index4 = 0;

    for (int l = 2; l < sizeNums; ++l) {
        if(primeNums[l]) {
            if (index3 == inputNums[index4]) {
                resultPrimes[index4] = primeNums[l];
                index4++;
            }
            index3++;
        }
    }

    printf("prvocisla:\n");
    for (int k = 0; k < index; ++k) {
        temp = inputNums[k];
        printf("\t %d\n", resultPrimes[k]);
    }

    free(primeNums);
    free(inputNums);
    free(resultPrimes);
    return 0;
}