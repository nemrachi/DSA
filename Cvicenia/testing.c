#include <stdio.h>
#include <stdlib.h>

void fun(int *arr) {
    for (int i = 0; i < 5; i++)
    {
        arr[i] += 1;
    }
    
}

int main () 
{
    int arr[10];

    printf("%d\n", sizeof(arr));

    for (int i = 0; i < (sizeof(arr)/sizeof(int)); i++)
    {
        printf("%d\n", arr[i]);
    }
    
    

    return 0;
}