#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_LEN 51
//profit moze ist do minusu!!

int main() {
    char ch;
    int scenarios, profit = 0, temp, prev = 0,
            buyPrice = 0, sellPrice, business = 1; // 1 - buy      0 - sell

    scanf("%d\n", &scenarios);

    if (scenarios <= 0) {
        return 0;
    }

    while (scenarios--) {
        while (1) {
            scanf("%d%c", &temp, &ch);

            if (business) {
                if (prev) {
                    if (prev < temp) {
                        buyPrice = prev;
                        prev = temp;
                        business = 0;
                    } else {
                        prev = temp;
                    }
                } else {
                    prev = temp;
                }
            } else {
                if (prev < temp) {
                    prev = temp;
                } else {
                    sellPrice = prev;
                    prev = temp;
                    business = 1;
                    profit += sellPrice - buyPrice;
                }
            }

            if (ch == '\r' || ch == '\n' || ch == '\t') {
                if (!business) {
                    if (temp > buyPrice) {
                        profit += temp - buyPrice;
                        printf("%d\n",profit);
                        profit = 0;
                        business = 1;
                        prev = 0;
                    }
                } else {
                    printf("%d\n",profit);
                    profit = 0;
                    business = 1;
                    prev = 0;
                }
                break;
            }
        }
    }

    return 0;
}