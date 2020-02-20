#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXINT 2147483646
int max = 0;

void sucet_k_najvacsich(int cena[], int n, int k) {
    int sortedCena[max];
    memset(sortedCena, 0, max* sizeof(int));
    int index = 0;

    for (int i = 0; i < n; ++i) {
        sortedCena[cena[i]-1] += 1;
    }

    for (int j = max-1; j >= 0; ++j) {
        // for (int i = 0; i < ; ++i) {
            
        // }
    }
}

int main() {
    int n, k, index = 0, *cena;
    char ch;

    scanf("%d", &n);
    printf("\n");

    cena = malloc(n * sizeof(int));

    scanf("%d", &k);
    printf("\n");

    while (scanf("%d%c", &cena[index], &ch)) {
        if (max < cena[index]) {
            max = cena[index];
        }

        index++;
        
        if (ch == '\n' && index == n) {
            break;
        }
    }

    sucet_k_najvacsich(cena, n, k);

    free(cena);

    return 0;
}
