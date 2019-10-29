// uloha3-2.c -- Ema Richnáková, 27.10.2019 20:25

#include <stdio.h>

unsigned long int count_binary_trees(unsigned int numNodes) {
    unsigned int m = 2 * numNodes;
    unsigned long int temp = 1;

    for (unsigned int i = 0; i < numNodes; i++) {
        temp = ((temp * (m - i)) / (i + 1));
    }

    return temp/(numNodes + 1);
}

int main() {
    int inputNum;
    char c;

    while (scanf("%d%c",&inputNum, &c) >= 1) {
        printf("%lu\n", count_binary_trees((unsigned int)inputNum));
    }

    return 0;
}