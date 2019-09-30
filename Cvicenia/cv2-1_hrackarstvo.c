#include <stdio.h>
#include <stdlib.h>
#define MAXINT 2147483646

void sucet_k_najvacsich(int cena[], int n, int k) {
    int *najdrahsiaCena = malloc(k* sizeof(int));
    int min = MAXINT;
    int minPos = 0;

    for (int i = 0; i < k; ++i) {
        najdrahsiaCena[i] = cena[i];
        if (cena[i] < min) {
            min = cena[i];
            minPos = i;
        }
    }

    for (int j = k; j < n; ++j) {
        if (cena[j] > min) {
            najdrahsiaCena[minPos] = cena[j];
            min = MAXINT;

            for (int i = 0; i < k; ++i) {
                if (najdrahsiaCena[i] < min) {
                    min = najdrahsiaCena[i];
                    minPos = i;
                }
            }
        }
    }

    int sum = 0;

    for (int l = 0; l < k; ++l) {
        sum += najdrahsiaCena[l];
    }

    printf("%d\n", sum);

    free(najdrahsiaCena);

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
        index++;

        if (ch == '\n' && index == n) {
            break;
        }
    }

    sucet_k_najvacsich(cena, n, k);

    free(cena);

    return 0;
}
