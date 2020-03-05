// uloha2-2.c -- Ema Richnáková, 17.11.2019 15:34
#include <stdio.h>
#include <stdlib.h>

int get_max(int * a, int n) {
  int max = a[0]; 

  for (int i = 1; i < n; i++) { 
      if (a[i] > max) {
          max = a[i]; 
      }
  }

  return max; 
}

void count_sort(int * a, int n, int exp) { 
    int output[n], i, count[10] = {0}; 
  
    for (i = 0; i < n; i++) {
      count[(a[i] / exp) % 10]++; 
    }
   
    for (i = 1; i < 10; i++) {
      count[i] += count[i - 1];
    } 

    for (i = n - 1; i >= 0; i--) { 
        output[count[(a[i] / exp) % 10] - 1] = a[i]; 
        count[(a[i] / exp) % 10 ]--; 
    } 
  
    for (i = 0; i < n; i++) {
      a[i] = output[i]; 
    }
        
} 

// Utriedi n cisel v poli a
void utried(int *a, int n) {
  int max = get_max(a, n); 
  
  for (int i = 1; max / i > 0; i *= 10) {
    count_sort(a, n, i); 
  }
    
}

int main(void) {
  int i, * x, n;

  scanf("%d", &n);
  x = (int *)malloc(n * sizeof(int));

  for (i = 0; i < n; i++) {
    scanf("%d", &x[i]);
  }

  utried(x, n);
  
  printf("%d", x[0]);

  for (i = 1; i < n; i++) {
    printf(" %d", x[i]);
    if (x[i-1] > x[i]) {
      printf(" -- CHYBA\n");
      return 0;
    }
  }

  printf("\n");
  return 0;
}