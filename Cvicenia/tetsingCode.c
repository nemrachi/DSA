#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int size = 0;
    char *ch = malloc(100);
    fgets(ch, 100, stdin);
    printf("%d ", strlen(ch));
    for (int i=0; ch[i]; ch[i]==' ' ? size++: i++);
    printf("%d ", size);
    return 0;
}