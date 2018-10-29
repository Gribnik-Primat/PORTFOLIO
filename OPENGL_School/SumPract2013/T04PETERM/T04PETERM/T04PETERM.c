#include <stdlib.h>
#include <stdio.h>

#define MAX 100

double A [MAX][MAX];
int P[MAX];
int N;
int Parity = 0;
double Determ = 0;




void Load (char *FileName)
{
  int i;
  int j;
  FILE *F;
  if((F=fopen(FileName,"rt")) == NULL)
    return;
  fscanf(F,"%d",&N);
  for(i = 0; i < N;i++)
    for(j = 0;j < N;j++)
      fscanf(F,"%lf", &A[i][j]);
  fclose(F);

}

void swap (int *a, int *b)
{
  int tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
}

void Go(int pos)
{
  int i;
  double sum = 1; 
  if(pos == N)
  {  
    for(i = 0;i<N;i++)
      sum *= A[i][P[i-1]];
    if(!Parity)
      Determ += sum;
    else
      Determ -= sum;
    return;
  }  
  else
    for(i = pos;i < N; i++)
    {
      swap(&P[pos],&P[i]);
      if(pos != i)  
        Parity = !Parity;
      Go(pos + 1);
      swap(&P[pos],&P[i]);
      if(pos != i)
        Parity = !Parity;
    }
}




void gauss()
{
  int i, j, k;
  for (j = 0; j < N; j++)
  {
    k = 0;
    while (A[k][0] == 0)
      k++;
    for (i = 0; i < N; i++)
      swap(&A[i][k], &A[i][0]);
    for (j = 1; j < N; j++)
      for (i = 1; i < N; i++)
        A[i][j] -= A[i][0] * (A[i][0] / A[0][0]);
  }
  for (i = 0; i < N; i++)
    Determ *= A[i][i];
}  

void main (void)
{
  int i;
  Load ("matrix.txt");
  for(i = 0;i < N;i++)
    P[i]=i+1;
  gauss();
  //Go(0);
  printf("determ is: %lf",Determ);
}