// uloha2-2.c -- Ema Richnáková, 17.11.2019 15:34

#include <stdio.h>

// Utriedi n cisel v poli a
void utried(int *a, int n)
{
  // ...
}

int main(void)
{
  int i, *x, n;

  scanf("%d", &n);
  x = (int*)malloc(n * sizeof(int));
  for (i = 0; i < n; i++)
    scanf("%d", &x[i]);

  utried(x, n);
  
  printf("%d", x[0]);
  for (i = 1; i < n; i++)
  {
    printf(" %d", x[i]);
    if (x[i-1] > x[i])
    {
      printf(" -- CHYBA\n");
      return 0;
    }
  }
  printf("\n");
  return 0;
}





/*
#include <stdio.h>

// Utriedi n cisel v poli a
void utried(int *a, int n)
{
  // ...
}

int main(void)
{
  int i, *x, n;

  scanf("%d", &n);
  x = (int*)malloc(n * sizeof(int));
  for (i = 0; i < n; i++)
    scanf("%d", &x[i]);

  utried(x, n);
  
  printf("%d", x[0]);
  for (i = 1; i < n; i++)
  {
    printf(" %d", x[i]);
    if (x[i-1] > x[i])
    {
      printf(" -- CHYBA\n");
      return 0;
    }
  }
  printf("\n");
  return 0;
}
*/