#include <stdio.h>
#include <stdlib.h>

int get_max(int *arr, int size)
{
   int max = arr[0];
        for (int i = 1; i < size; i++)
        {
            if (arr[i] > max)
            {
                max = arr[i];
            }
        }
    return max;
}

void count_sort(int *arr, int size, int digit)
{
     int output[size], count[10] = { 0 }, i;

     for (i = 0; i < size; i++)
     {
         count[(arr[i]/digit) % 10]++;
     }
     for (i = 1; i < 10; i++)
     {
        count[i] += count[i - 1];
     }

     for (i = size - 1; i >= 0; i--)
     {
         output[count[(arr[i] / digit) % 10] - 1] = arr[i];
         count[(arr[i] / digit) % 10]--;
     }

     for (int i = 0; i < size; i++)
     {
        arr[i] = output[i];
     }
}

void radix_sort(int *array, int size) 
{
    int max_num = get_max(array, size);

    for (int digit = 1; max_num / digit > 0; digit *= 10) 
    {
        count_sort(array, size, digit);
    }
}

long sucet_k_najvacsich(int *arr, int n, int k) {
    int i, sum = 0;

    if (n <= 0 || k <= 0) {
        return 1;
    } else {
        arr = malloc(n * sizeof(int));

    for (i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    radix_sort(arr, n);

    for (i = n - 1; i > (n - k - 1); i--)
    {
        sum += arr[i];
    }

    free(arr);

    return sum;
    }
}

int main()
{
    int n, k, *arr = NULL;

    scanf("%d", &n);
    scanf("%d", &k);

    long your = sucet_k_najvacsich(arr, n, k);

    printf("%ld\n", your);

    return 0;
}