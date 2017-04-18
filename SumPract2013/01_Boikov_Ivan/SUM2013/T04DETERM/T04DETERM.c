/*Boikov Ivan 10-1*/
#define MAX 100
#include <stdio.h>
#include <stdlib.h>

static int A[MAX][MAX], Determ = 0, P[MAX], N;                                                    

void Load (char *FileName)
{                                                                     
  FILE *F;
  int i, j;

  if ((F = fopen(FileName, "rt")) == NULL)
    return;
  fscanf(F, "%i", &N);
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%i", &A[i][j]);
  for (i = 0; i < N; i++)
  {
    printf("\n\n");
    for (j = 0; j < N; j++)
      printf("%i  ", A[i][j]);
  }
  printf("\n\n");
  fclose(F);
}

void Swap (int *src, int *dest)
{
  int temp = *src;

  *src = *dest;
  *dest = temp;
}

void Go (int Pos)
{
  int i, Parity = 0;
  double prod = 1;
                        
  if (Pos == N)
  {
    for (i = 0; i < N; i++)
    {
      prod *= A[i][P[i]];
      printf("%i ", P[i]);
    }
    printf("- %i \n", Parity);
    if (Parity)
      Determ -= prod;
    else
      Determ += prod;  
    return;
  }
  else
    for (i = Pos; i < N; i++)
    {
       Swap(&P[Pos], &P[i]);
       if (Pos != i)
         if (Parity == 0)
           Parity = 1;
         else
           Parity = 0;
       Go(Pos + 1);
       Swap(&P[Pos], &P[i]);
       if (Pos != i)
         if (Parity == 0)
           Parity = 1;
         else
           Parity = 0;
    }
}

int main (void)
{
  int a, i, j, line, sign = 1;

  Load("matrix.txt");
  for (i = 0; i < MAX; i++)
    P[i] = i + 1;
  Go(0);
  printf("Determ: %i\n\n", Determ);

  printf("GAUSS\n\n");
  Determ = 1;
  for (line = 1; line < N; line++)
  {
    a = 0;
  while (A[a][0] == 0)
    a++;
  if (a > N)
  {
    printf("after...\n");
    getch();
    return;
  }
  for (i = 0; i < N; i++)
    Swap(&A[i][a], &A[i][0]), sign = -sign;;
  for (j = 1; j < N; j++)
    for (i = 1; i < N; i++)
      A[i][j] -= A[i][0] * (A[i][0] / A[0][0]);
  }
  for (i = 0; i < N; i++)
    Determ *= A[i][i];
  printf("Determ: %i", Determ * sign);
  getch();
  return 0;
}