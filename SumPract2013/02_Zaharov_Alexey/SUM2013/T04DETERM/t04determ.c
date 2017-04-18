/* Zakharov Alexey, Summer Practise 2013, 04.06.2013 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define MAX 100
double A[MAX][MAX];
int P[MAX], N;
double Determ=0;

void Load( char *FileName );
//void Go( int Pos );
void Swap( int *a, int *b );

void Swap( int *a, int *b )
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void Load( char *FileName )
{
  FILE *F;
  int i, j;
  if ((F = fopen(FileName, "rt"))==NULL)
    return;
  fscanf(F, "%d", &N);
  for (i=0; i<N; i++)
    for (j=0; j<N; j++)
    {
      fscanf(F, "%lf", &A[i][j]);
    }
  fclose(F);
} 

static int Parity=0;
void Go( int Pos )
{
  int i;
  double prod=1;
  double sum=0;
  if (Pos==N)
  {
    for (i=0; i<N; i++)
    {
      prod *= A[i][P[i]];
      printf ("%i", P[i]);
    }
    if (Parity)
        Determ+=prod;
      else
        Determ-=prod;
    
    printf (" - %d\n", Parity);
    return;
  }  
  else
    for (i=Pos; i<N; i++)
    {
      Swap(&P[Pos], &P[i]);
      if (Pos!=i)
        Parity = !Parity;
      
      Go(Pos+1);
      Swap(&P[Pos], &P[i]);
      if (Pos!=i)
        Parity = !Parity;
    }
}

int main (void)
{
  int i, j, k;
  Load("matrix.txt");
/*  for (i=0; i<N; i++)
    P[i]=i+1;
  Go(0);
  */
  Determ = 1;
  for (j = 0; j < N; j++)
  {
    k = 0;
    while (A[k][0] == 0)
      k++;
    for (i = 0; i < N; i++)
      Swap(&A[i][k], &A[i][0]);
    for (j = 1; j < N; j++)
      for (i = 1; i < N; i++)
        A[i][j] -= A[i][0] * (A[i][0] / A[0][0]);
  }
  for (i = 0; i < N; i++)
    Determ *= A[i][i];

  printf("Determinant: %lf\n", Determ);
  return 0;
} 