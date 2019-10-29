#include <stdio.h>
#include <stdlib.h>
#define DOUBLE_SIZE sizeof(double)
//unsigned na double
//prime prepisat na 1, potom brat index!!!
// idu cisla aj dole
int main() {
    double maxPrime = 1299709,
    *resultPrimes, *primeNums, *inputNums,
    temp, maxNumIn = 0;
    char ch;
    int index = 0;

    inputNums = malloc(DOUBLE_SIZE+1);
    //printf("Zadaj cisla:\n");
    //zadanie cisel poradia prvocisel
    while(scanf("%lf%c", &temp, &ch) >= 1) {
        if ((1 <= temp) && (temp <= 100000)) {
            inputNums[index] = temp;
            inputNums = realloc(inputNums, DOUBLE_SIZE*(index+1));
            index++;
            //uklada si najvacsie poradove cislo
            if (temp > maxNumIn) {
                maxNumIn = temp;
            }
        } else {
            printf("Number is out of range\n");
        }
    }

    double sizeNums = maxPrime; //cca celkovy pocet cisel

    primeNums = malloc(((int)sizeNums) * sizeof(int));

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
    double index2 = inputNums[0];
    double index3 = 1;
    double index4 = 0;

    for (int l = 2; l < sizeNums; ++l) {
        if(primeNums[l]) {
            if (index3 == inputNums[(int)index4]) {
                resultPrimes[(int)index4] = primeNums[l];
                index4++;
            }
            index3++;
        }
    }

    //printf("prvocisla:\n");
    for (int k = 0; k < index; ++k) {
        temp = inputNums[k];
        printf("\t %lf\n", resultPrimes[k]);
    }

    free(primeNums);
    free(inputNums);
    free(resultPrimes);
    return 0;
}