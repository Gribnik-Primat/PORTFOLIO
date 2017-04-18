                                                                                                                                      /* FILE NAME: COW.C
 * PROGRAMMER: IB1
 * DATE: 06.06.2013
 * PURPOSE: Animation sample module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#include "anim.h"
#include "vec.h"



/***
 * Функции обработки мегакуба
 ***/

/* Тип представления мегакуба */
typedef struct
{
  /* Включаем базовые поля объекта анимации */
  IB1_UNIT_BASE_FUNCS;
  INT X, Y; /* Позиция куба  */
  INT RandShift; /* Случайный сдвиг */
  DBL RandScale; /* Случайный масштаб */
  INT Who;       /* Тип объекта */
} CUBE;

/* Функция отрисовки куба.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       CUBE *Unit;
 *   - указатель на систему анимации:
 *       IB1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

/* Vertex array */
VEC *Vertexes; 
static POINT *VertexesProj;
INT NumOfVertexes;

/* Facet array */
INT (*Facets)[3];
INT NumOfFacets;

VOID LoadCow( VOID )
{                                              
  FILE *F;
  INT fn = 0, vn = 0;
  static CHAR Buf[1000];

  if ((F = fopen("cube.obj", "rt")) == NULL)
    return;

  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      vn++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      fn++;

  if ((Vertexes = malloc(2 * sizeof(VEC) * vn)) == NULL)
  {
    fclose(F);
    return;
  }
  if ((Facets = malloc(sizeof(INT [3]) * fn)) == NULL)
  {
    free(Vertexes);
    fclose(F);
    return;
  }
  NumOfVertexes = vn;
  NumOfFacets = fn;
  VertexesProj = Vertexes + NumOfVertexes;

  vn = 0;
  fn = 0;
  rewind(F);
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      Vertexes[vn].X = x;
      Vertexes[vn].Y = y;
      Vertexes[vn].Z = z;
      vn++;
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n1, n2, n3;

      sscanf(Buf + 2, "%d%d%d", &n1, &n2, &n3);
      Facets[fn][0] = n1 - 1;
      Facets[fn][1] = n2 - 1;
      Facets[fn][2] = n3 - 1;
      fn++;
    }

  fclose(F);
}
static VOID CubeRender( CUBE *Unit, IB1ANIM *Ani)
{
  DBL t = (DBL)clock() / CLOCKS_PER_SEC;
  INT
    i, j, y;
  MATR m = 
  {
    {
      {1, 0, 0 ,0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };


  SelectObject(Ani->hDC, GetStockObject(NULL_BRUSH));
  SelectObject(Ani->hDC, GetStockObject(DC_PEN));
  SetDCPenColor(Ani->hDC, RGB(255, 255, 255));
  SetDCBrushColor(Ani->hDC, RGB(0, 111, 0));

  //m = MatrMulMatr(MatrScale(15, 15, 15), 
  //                MatrMulMatr(MatrMulMatr(MatrRotateX(Ani->Jx * PI), 
  //                MatrRotateY(Ani->Jy * PI)), 
  //                MatrRotateZ(Ani->Jz * PI)));
  /* calculate projection */ 
  //VertexesProj = malloc(sizeof(POINT) * NumOfVertexes); 
  for (i = 0; i < NumOfVertexes; i++)
  {
    VEC v;

    v = Vertexes[i];
    v.X += Unit->X;
    v.Y += Unit->Y;
    v.Z += 10;
    VertexesProj[i] = WorldToScreen(v);
  }
  for (i = 0; i < NumOfFacets; i++)
  {
    POINT p[3];

    for (j = 0; j < 3; j++)
    {
      p[j].x = VertexesProj[Facets[i][j]].x;         
      p[j].y = VertexesProj[Facets[i][j]].y;
    }

    Polygon(Ani->hDC, p, 3);
  }  

  /*for (i = 0; i < NumOfVertexes; i++)
  {
    INT
      x = VertexesProj[i].X;
      y = VertexesProj[i].Y;
      SetPixelV(Ani->hDC, x, y, RGB(255, 255, 255));
  } */
}
/*static VOID CubeRender( CUBE *Unit, IB1ANIM *Ani )
{
  DBL t = (DBL)clock() / CLOCKS_PER_SEC;
  INT
    a = 30 * sin(t * Unit->RandScale + Unit->RandShift),
    b = 30 * cos(t * Unit->RandScale + Unit->RandShift);

  switch (Unit->Who)
  {
  case 0:
    Rectangle(Ani->hDC, Unit->X + a, Unit->Y + b, Unit->X + a + 30, Unit->Y + b + 30);
    return;
  case 1:
    Ellipse(Ani->hDC, Unit->X + a, Unit->Y + b, Unit->X + a + 30, Unit->Y + b + 30);
    return;
  default:
    RoundRect(Ani->hDC, Unit->X + a, Unit->Y + b, Unit->X + a + 30, Unit->Y + b + 30, 8, 8);
    return;
  }
} /* End of 'CubeRender' function */

/* Функция сроздания нового куба.
 * АРГУМЕНТЫ:
 *   - позиция на экране:
 *       INT X, Y;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (IB1UNIT *) указатель на созданный объект;
 */
IB1UNIT * CowCreate( INT X, INT Y )
{
  CUBE *Unit;

  if ((Unit = (CUBE *)IB1_UnitCreate(sizeof(CUBE))) == NULL)
    return NULL;

  Unit->Render = (IB1UNIT_RENDER)CubeRender;
  Unit->X = X;
  Unit->Y = Y;
  //Unit->Who = rand() % 3;
  //Unit->RandShift = rand() % 1000;
  //Unit->RandScale = 0.75 + 5.5 * rand() / (DBL)RAND_MAX;
  return (IB1UNIT *)Unit;
} /* End of 'CubeCreate' function */

/* END OF 'COW.C' FILE */

/*POINT IK1_AnimWorldToScreen(VEC P)
{
 
  
  VEC P1, P2, P3;  
  INT XS, YS;
  POINT pt;
  P1 = VecMulMatr(P, IK1_Anim.MatrWorld);
  P2 = VecMulMatr(P1,IK1_Anim.MatrView);
  P3.X = P2.X *  IK1_Anim.PD / P2.Z;
  P3.Y = P2.Y *  IK1_Anim.PD / P2.Z;

  P3.X *= 2/(IK1_Anim.Wp);
  P3.Y *= 2/(IK1_Anim.Hp);
                             
  XS = ((P3.X + 1) / 2) * (IK1_Anim.W - 1);
  YS = ((-P3.Y + 1) / 2) * (IK1_Anim.H - 1);

  pt.x = XS;
  pt.y = YS;

  return pt;

} */