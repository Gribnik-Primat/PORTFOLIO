#include <iostream>
#include <windows.h>

const int n(4);
int colors[n];
int bestcolors[n];
int bestcolors1[n];
int matr[n][n] = { 0 };
int col = 1;
int flag = 0;
int minx = n;
int mincol = 0;

bool proverka(int m, int i)
{
  for (int j = 0; j < m; j++)
  if (matr[j][m] == 1 && colors[j] == i)
	return false;
  return true;
}

void backtrack1(int m, int n, int k,int a) // n - кол-во стран k-кол-во цветов
{
  int i,curmincol = 0;
  if (m > n)
  {
	for (i = 0; i < n; i++)
	{
	  if (colors[i] == n)
		return;
	  if (curmincol < colors[i])
		curmincol = colors[i];
	}
	if (mincol < curmincol )
	{
	  mincol = curmincol;
	  for (i = 0; i < n; i++)
		  bestcolors1[i] = colors[i];
	 // std::cout << "граф - цвет\n";
	  //for (int i = 0; i < n; i++)
	  	//std::cout << " " << i << "      " << bestcolors1[i] << " \n";
	  return;
	}
  }
  else
  {
	for (i = 1; i < k;i++)
	{
	  colors[m] = i;
	  if(proverka(m, a))
		backtrack1(m + 1, n, k,i);
	  if (col < k)
	  {	 
		col++;	
		colors[m] = col;
	    backtrack1(m + 1,n,k,col);
		col--;
	  }
	}
  }
}



void backtrack(int m, int n, int k) // n - кол-во стран k-кол-во цветов
{
  int i,curmincol=0;
  if (m > n)
  {
	for (i = 0; i < n; i++)
	{
	  if (curmincol < colors[i])
		curmincol = colors[i];
	}
	if (mincol <= curmincol)
	{
	  mincol = curmincol;
	  for (i = 0; i < n; i++)
		bestcolors[i] = colors[i];
	  //std::cout << "граф - цвет\n";
	 // for (int i = 0; i < n; i++)
		//std::cout << " " << i << "      " << bestcolors[i] << " \n";
	}
  }
  else
  {
	for (i = 1; i < k; i++)
	{
	  colors[m] = i;
	  if (proverka(m,i))
	 	backtrack(m + 1, n, k);
	}
  }
}


int main()
{
  LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds1, ElapsedMicroseconds2;
   
  setlocale(LC_ALL, "Russian");
  int color[n] = {0};
  int i, j,x,k,y,maxn=0;
  char alphabet[26] = { 0 };
  FILE *F;
  F = fopen("graph.gv", "wt");
  fprintf(F, "graph G\n{\n");

  /*for (;;) ручной
  {
	std::cout << "введите начальную вершину";
	std::cin >> x;
	std::cout << "введите конечную вершину";
	std::cin >> y;

	if (x == -1 || y == -1)
	  break;
	else
	  matr[x][y] = matr[y][x] = 1;
  } заполнение графа*/

  for (i = 0; i < n; i++) // рандом
  for (j = 0; j < n; j++)
	matr[i][j] = matr[j][i] = rand() % 2;
  for (i = 0; i < n; i++)
	matr[i][i] = 0;

  for (i = 0; i < n; i++)
  {
	std::cout << std::endl;
	for (j = 0; j < n; ++j)
	  std::cout << matr[i][j] << ' '; // вывод матрицы
  }

  for (i = 0; i < n; i++)
	color[i] = 1;


  for (i = 0; i < n; i++)
  {
	for (j = 0; j < n; j++)
	if (matr[i][j] == 1 && maxn < i)
	{
	  if (maxn < i)
		maxn = i;
	}
  }
  for (i = 0; i < n; i++)
  {
	for (j = 0; j < n; j++)
	if (matr[i][j] == 1 && minx > i)
	{
	  if (minx > i)
		minx = i;
	}
  }

  QueryPerformanceCounter(&StartingTime);

  backtrack(0, n, n+1);

  QueryPerformanceCounter(&EndingTime);
  ElapsedMicroseconds2.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

  for (int i = 0; i < n; i++)
	colors[i] = 0;
  flag = 0;
  mincol=0;

  QueryPerformanceCounter(&StartingTime);

  backtrack1(0,n, n+1,1);

  QueryPerformanceCounter(&EndingTime);
  ElapsedMicroseconds1.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

  std::cout << "\n" << mincol << "\n";
  std::cout << "граф - цвет\n";
  for (int i = 0; i < n; i++)
  {
	  
		std::cout << " " << i << "      " << bestcolors[i] << " \n";
		if (bestcolors[i] == 1)
		  fprintf(F, "%c[color=green];\n", 'a' + i);
		if (bestcolors[i] == 2)
		  fprintf(F, "%c[color=red];\n", 'a' + i);
		if (bestcolors[i] == 3)
		  fprintf(F, "%c[color=blue];\n", 'a' + i);
		if (bestcolors[i] == 4)
		  fprintf(F, "%c[color=yellow];\n", 'a' + i);
		if (bestcolors[i] == 5)
		  fprintf(F, "%c[color=cyan];\n", 'a' + i);
	}
  std::cout << "\n";
  for (int i = 0; i < n; i++)
  {
	std::cout << " " << i << "      " << bestcolors1[i] << " \n";
  }
  for (int i = 0; i < n; i++)
	for (int j = 0; j < n; j++)
	if (matr[i][j] == 1)
	  fprintf(F, "%c -- %c;\n", 'a' + i, 'a' + j);
  fprintf(F, "} \n");
  fclose(F);
  system("F:\\Graphviz2.38\\bin\\dot.exe -Tjpg -O F:\\Projects(politex)(3)\\task4\\task4\\graph.gv");
	  fclose(F);
  printf("%lf\n", (double)ElapsedMicroseconds1.QuadPart);
  printf("%lf\n", (double)ElapsedMicroseconds2.QuadPart);
 
  system("pause");
  return 0;
}