/* FILE NAME: GOBJLOAD.C
 * PROGRAMMER: bg3
 * DATE: 13.06.2013
 * PURPOSE: Geometry object load from file handle.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "anim.h"

#define SCAN3(str, ref) \
  if (sscanf(str, "%i/%i/%i", &ref[0], &ref[1], &ref[2]) == 3) \
    ref[0]--, ref[1]--, ref[2]--;                                                  \
  else if (sscanf(str, "%i//%i", &ref[0], &ref[2]) == 2)       \
    ref[0]--, ref[2]--, ref[1] = -1;                                               \
  else if (sscanf(str, "%i/%i", &ref[0], &ref[1]) == 2)        \
    ref[0]--, ref[1]--, ref[2] = -1;                                               \
  else if (sscanf(str, "%i", &ref[0], &ref[1]) == 1)           \
    ref[0]--, ref[1] = ref[2] = -1

#define MAX_SIZE 10000

/* Глобальный буфер для чтения */
static CHAR Buf[MAX_SIZE];

/* Части строки после разбиения */
static INT NumOfParts;
static CHAR *Parts[MAX_SIZE / 2];

/* Функция разбиения входной строки на части */
static INT Split( VOID )
{
  INT i = 0;

  NumOfParts = 0;
  Parts[0] = Buf;
  while (Buf[i] != 0)
  {
    /* пропускаем пробелы до начала цикла */
    while (isspace(Buf[i]))
      i++;
    /* уходим, если строка закончилась */
    if (Buf[i] == 0)
      return NumOfParts;

    /* запоминаем начало слова */
    Parts[NumOfParts++] = Buf + i;
    /* ищем конец слова */
    while (!isspace(Buf[i]) && Buf[i] != 0)
      i++;

    /* уходим, если строка закончилась */
    if (Buf[i] == 0)
      return NumOfParts;
    /*завершаем текущее слово */
    Buf[i++] = 0;
  }
  return NumOfParts;
} /* End of 'Split' function */

/* Массив индексов вершинных координат, норматей и текстурных координат */
struct
{
  INT Nv, Nn, Nt, Next;
} *VertexRefs;
INT NumOfVertexRefs;

/* массив начальных индеков в 'VertexRefs' */
INT *VertexStartIndex;

/* Функция получения реальной точки */
INT GetVertexNumber( INT Nv, INT Nn, INT Nt )
{
  INT *n = &VertexStartIndex[Nv];

  while (*n != -1 &&
         !(VertexRefs[*n].Nv == Nv && VertexRefs[*n].Nn == Nn && VertexRefs[*n].Nt == Nt))
    n = &VertexRefs[*n].Next;
  if (*n == -1)
  {
    *n = NumOfVertexRefs++;
    VertexRefs[*n].Nv = Nv;
    VertexRefs[*n].Nn = Nn;
    VertexRefs[*n].Nt = Nt;
    VertexRefs[*n].Next = -1;
  }
  return *n;
} /* End of 'GetVertexNumber' function */

/* Функция загрузки материалов.
 * АРГУМЕНТЫ:
 *   - указатель на создаваемый объект:
 *       bg3GOBJ *Go;
 *   - имя файла материалов:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID LoadMaterials( bg3GOBJ *Go, CHAR *FileName )
{
  FILE *F;
  bg3MATERIAL DefMat, Mat;
  BOOL IsFirst = TRUE;

  DefMat.Ka = VecSet(0.1, 0.1, 0.1);
  DefMat.Kd = VecSet(0.9, 0.9, 0.9);
  DefMat.Ks = VecSet(0.0, 0.0, 0.0);
  DefMat.Phong = 30;
  DefMat.Trans = 0;
  strcpy(DefMat.MapD, "");
  strcpy(DefMat.Name, "unknown");

  Mat = DefMat;

  if ((F = fopen(FileName, "rt")) == NULL)
    return;

  /* считываем все команды работы с материалами */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    Split();
    if (NumOfParts > 1)
      if (strcmp(Parts[0], "Ka") == 0)
      {
        sscanf(Parts[1], "%f", &Mat.Ka.X);
        sscanf(Parts[2], "%f", &Mat.Ka.Y);
        sscanf(Parts[3], "%f", &Mat.Ka.Z);
      }
      else if (strcmp(Parts[0], "Kd") == 0)
      {
        sscanf(Parts[1], "%f", &Mat.Kd.X);
        sscanf(Parts[2], "%f", &Mat.Kd.Y);
        sscanf(Parts[3], "%f", &Mat.Kd.Z);
      }
      else if (strcmp(Parts[0], "Ks") == 0)
      {
        sscanf(Parts[1], "%f", &Mat.Ks.X);
        sscanf(Parts[2], "%f", &Mat.Ks.Y);
        sscanf(Parts[3], "%f", &Mat.Ks.Z);
      }
      else if (strcmp(Parts[0], "Ns") == 0)
        sscanf(Parts[1], "%f", &Mat.Phong);
      else if (strcmp(Parts[0], "Tr") == 0 ||
               strcmp(Parts[0], "d") == 0 ||
               strcmp(Parts[0], "D") == 0)
        sscanf(Parts[1], "%f", &Mat.Trans);
      else if (strcmp(Parts[0], "map_Kd") == 0)
        strncpy(Mat.MapD, Parts[1], sizeof(Mat.MapD) - 1);
      else if (strcmp(Parts[0], "newmtl") == 0)
      {
        if (IsFirst)
          IsFirst = FALSE;
        else 
          BG3_GeomAddMat(Go, &Mat);
        Mat = DefMat;
        strncpy(Mat.Name, Parts[1], sizeof(Mat.Name) - 1);
      }
    }
  if (!IsFirst)
    BG3_GeomAddMat(Go, &Mat);
  fclose(F);
} /* End of 'LoadMaterials' function */

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на создаваемый объект:
 *       bg3GOBJ *Go;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE при нехватке памати.
 */
BOOL BG3_GeomLoad( bg3GOBJ *Go, CHAR *FileName )
{
  INT vn = 0, vtn = 0, vnn = 0, fn = 0, pn = 0, size, i, j, p;
  FILE *F;
  bg3PRIM prim;
  VEC *ReadV, *ReadN;
  UV *ReadUV;
  INT (*ReadF)[3];
  struct
  {
    INT First, Last, MatNo;
  } *PrimsInfo;

  memset(Go, 0, sizeof(bg3GOBJ));
  if ((F = fopen(FileName, "rt")) == NULL)
    return FALSE;

  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      vn++;
    else if (Buf[0] == 'v' && Buf[1] == 't')
      vtn++;
    else if (Buf[0] == 'v' && Buf[1] == 'n')
      vnn++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      fn += Split() - 3;
    else if (strncmp(Buf, "usemtl", 6) == 0)
      pn++;

  if (pn == 0)
    pn = 1; /* материалы не использовались */
  /* память под дополнительные данные выделяем скопом */
  size = sizeof(VEC) * vn +                             /* вершины */
         sizeof(VEC) * vnn +                            /* нормали */
         sizeof(UV) * vtn +                             /* текстуры */
         sizeof(INT) * vn +                             /* начальные индексы */
         sizeof(PrimsInfo[0]) * pn +                    /* примитивы */
         sizeof(INT [3]) * fn +                         /* треугольки */
         sizeof(VertexRefs[0]) * (vn + vtn + vnn) * 30; /* индексы */

  if ((ReadV = malloc(size)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  memset(ReadV, 0, size);
  ReadN = (VEC *)(ReadV + vn);
  ReadUV = (UV *)(ReadN + vnn);
  ReadF = (INT (*)[3])(ReadUV + vtn);
  PrimsInfo = (VOID *)(ReadF + fn);
  VertexStartIndex = (INT *)(PrimsInfo + pn);
  VertexRefs = (VOID *)(VertexStartIndex + vn);
  /* все стартовые индексы - '-1' */
  memset(VertexStartIndex, 0xFF, sizeof(INT) * vn);
  /* все ссылки от вершин - '-1' */
  memset(VertexRefs, 0xFF, size - ((INT)VertexRefs - (INT)ReadV));

  /* второй проход - читаем геометрию */
  rewind(F);
  NumOfVertexRefs = 0;

  vn = 0;
  vtn = 0;
  vnn = 0;
  fn = 0;
  pn = 0;
  PrimsInfo[0].First = 0; 
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (strncmp(Buf, "mtllib ", 7) == 0)
    {
      Split();
      LoadMaterials(Go, Parts[1]);
    }
    else if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      FLT x, y, z;

      sscanf(Buf + 2, "%f%f%f", &x, &y, &z);
      ReadV[vn++] = VecSet(x, y, z);
    }
    else if (Buf[0] == 'v' && Buf[1] == 'n')
    {
      FLT x, y, z;

      sscanf(Buf + 2, "%f%f%f", &x, &y, &z);
      ReadN[vnn++] = VecNormalize(VecSet(x, y, z));
    }
    else if (Buf[0] == 'v' && Buf[1] == 't')
    {
      FLT u, v;

      sscanf(Buf + 2, "%f%f", &u, &v);
      ReadUV[vtn++] = UVSet(u, v);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT i, n0[3], n_prev[3], n[3], r0, r_prev, r;

      Split();
      
      SCAN3(Parts[1], n0);
      r0 = GetVertexNumber(n0[0], n0[2], n0[1]);
      SCAN3(Parts[2], n_prev);
      r_prev = GetVertexNumber(n_prev[0], n_prev[2], n_prev[1]);
      for (i = 3; i < NumOfParts; i++)
      {
        SCAN3(Parts[i], n);
        r = GetVertexNumber(n[0], n[2], n[1]);

        ReadF[fn][0] = r0;
        ReadF[fn][1] = r_prev;
        ReadF[fn][2] = r;
        r_prev = r;
        fn++;
      }
    }
    else if (strncmp(Buf, "usemtl", 6) == 0)
    {
      INT i;

      Split();

      if (pn != 0)
        PrimsInfo[pn - 1].Last = fn - 1; 
      pn++;
      PrimsInfo[pn - 1].First = fn;
      /* ищем материал в таблице у объекта */
      for (i = 0; i < Go->NumOfMats; i++)
        if (strcmp(Parts[1], Go->Mats[i].Name) == 0)
          break;
      if (i == Go->NumOfMats)
        PrimsInfo[pn - 1].MatNo = -1;
      else
        PrimsInfo[pn - 1].MatNo = i;

    }
  if (pn != 0)
    PrimsInfo[pn - 1].Last = fn - 1; 
  else
  {
    PrimsInfo[0].First = 0; 
    PrimsInfo[0].Last = fn - 1; 
    PrimsInfo[0].MatNo = -1;
  }
  fclose(F);

  /* Создание примитивов и копирование данных */
  BG3_PrimDefaultColor = VecSet(1, 1, 1);
  for (p = 0; p < pn; p++)
  {
    INT
      fn = PrimsInfo[p].Last - PrimsInfo[p].First + 1,
      minv, maxv, vn;

    minv = maxv = ReadF[PrimsInfo[p].First][0];
    for (j = PrimsInfo[p].First + 1; j <= PrimsInfo[p].Last; j++)
    {
      INT k;

      for (k = 0; k < 3; k++)
      {
        if (minv > ReadF[j][k])
          minv = ReadF[j][k];
        if (maxv < ReadF[j][k])
          maxv = ReadF[j][k];
      }
    }
    vn = maxv - minv + 1; 

    BG3_PrimCreate(&prim, BG3_PRIM_TRIMESH, vn, fn * 3, NULL, NULL);
    /* копируем вершины */
    for (i = 0; i < prim.NumOfV; i++)
    {
      INT n;

      n = VertexRefs[i + minv].Nv;
      prim.V[i].P = ReadV[n];

      n = VertexRefs[i + minv].Nn;
      if (n != -1)
        prim.V[i].N = ReadN[n];
      else
        prim.V[i].N = VecSet(0, 1, 0);

      n = VertexRefs[i + minv].Nt;
      if (n != -1)
        prim.V[i].T = ReadUV[n];
      else
        prim.V[i].T = UVSet(0, 0);
    }
    /* копируем треугольники (индексы вершин) */
    for (i = 0; i < prim.NumOfI; i++)
    {
      prim.I[i] = ReadF[i / 3 + PrimsInfo[p].First][i % 3] - minv;
    }
    prim.Mat = PrimsInfo[p].MatNo;
    BG3_GeomAddPrim(Go, &prim);
  }

  free(ReadV);
  return TRUE;
} /* End of 'BG3_GeomLoad' function */

/* END OF 'GOBJLOAD.C' FILE */

