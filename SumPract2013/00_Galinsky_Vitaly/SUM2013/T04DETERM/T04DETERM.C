#include <stdio.h>

#define N 4
int P[N];
int Parity, n;

void Swap( int *A, int *B )
{
  int c = *A;

  *A = *B;
  *B = c;
} /* End of 'Swap' function */

void Go( int Pos )
{
  int i;

  /* проверяем выход из рекурсии */
  if (Pos == N)
  {
    printf("%5d: ", ++n);
    for (i = 0; i < N; i++)
      printf("%d ", P[i]);
    printf(" - %d\n", Parity);
    return;
  }
  for (i = Pos; i < N; i++)
  {
    Swap(&P[Pos], &P[i]);
    if (Pos != i)
      Parity = !Parity;
    Go(Pos + 1);
    Swap(&P[Pos], &P[i]);
    if (Pos != i)
      Parity = !Parity;
  }
}


void main( void )
{
  int i;

  for (i =0 ; i < N; i++)
    P[i] = i + 1;
  Go(0);
} /* End of ' main' function */
