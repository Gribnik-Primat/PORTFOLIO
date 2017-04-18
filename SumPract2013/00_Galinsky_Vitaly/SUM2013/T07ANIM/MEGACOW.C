/* FILE NAME: MEGACOW.C
 * PROGRAMMER: VG4
 * DATE: 14.06.2013
 * PURPOSE: Animation sample module.
 */

#include <stdio.h>
#include <stdlib.h>

#include "anim.h"

/***
 * Функции обработки мегакоровы
 ***/

static UINT CowProg;

/* Тип представления мегакоровы */
typedef struct
{
  /* Включаем базовые поля объекта анимации */
  VG4_UNIT_BASE_FUNCS;
  INT X, Y, Z;   /* Позиция коровы  */
  INT No;        /* Порядковый номер объекта */
} COW;

static INT CurrentNo = 0; /* очередной порядковый номер */

/* Vertex array */
static VEC *Vertexes;
static POINT *VertexesProj;
static INT NumOfVertexes;

/* Facet array */
static INT (*Facets)[3];
static INT NumOfFacets;

/* Load cow function */
static VOID LoadCow( VOID )
{
  FILE *F;
  INT fn = 0, vn = 0;
  static CHAR Buf[1000];

  if ((F = fopen("cow_new1.object", "rt")) == NULL)
    return;

  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      vn++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      fn++;

  /* выделяем память подо все */
  if ((Vertexes = malloc(sizeof(VEC) * vn + sizeof(INT [3]) * fn + sizeof(POINT) * vn)) == NULL)
  {
    fclose(F);
    return;
  }
  Facets = (INT (*)[3])(Vertexes + vn);
  VertexesProj = (POINT *)(Facets + fn);

  NumOfVertexes = vn;
  NumOfFacets = fn;

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
} /* End of 'LoadCow' function */

/* Функция отрисовки мегакоровы.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       COW *Unit;
 *   - указатель на систему анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CowRender( COW *Unit, vg4ANIM *Ani )
{
  INT i, j;
  DBL s;
  MATR Ma, M0, M1, M2, M3, WVP;

  s = 1 + 0.30 * sin(Ani->Time * 8 + 3.0 * Unit->No);
  s = 1;
  Ma = MatrScale(s, s, s);
  M0 = MatrRotateZ(30 * sin(Ani->Time * 5 + Unit->No));
  M1 = MatrRotateY(-30);
  M2 = MatrTranslate(Unit->X, Unit->Y, Unit->Z);
  M3 = MatrRotateY(Ani->Time * 30);

  Ani->Wp = 0.70 * Ani->W / Ani->H;
  Ani->Hp = 0.70;
  Ani->PD = 1.0;

  Ani->MatrWorld = MatrMulMatr(MatrMulMatr(MatrMulMatr(MatrMulMatr(Ma, M0), M1), M2), M3);
  Ani->MatrView = MatrViewLookAt(VecSet(8, 10, 8), VecSet(0, 0, 0), VecSet(0, 1, 0));
  Ani->MatrProj = MatrProjection(-Ani->Wp / 2, Ani->Wp / 2,
                                 -Ani->Hp / 2, Ani->Hp / 2,
                                 Ani->PD, 1000);

  WVP = MatrMulMatr(MatrMulMatr(Ani->MatrWorld, Ani->MatrView), Ani->MatrProj);


  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
  glUseProgram(CowProg);
  glColor3d(1, 1, 1);

  j = glGetUniformLocation(CowProg, "Time");
  glUniform1f(j, Ani->Time);

  j = glGetUniformLocation(CowProg, "Matr");
  glUniformMatrix4fv(j, 1, FALSE, WVP.A[0]);

  /* рисуем треугольники */
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(3);
  glBegin(GL_TRIANGLES);
  for (i = 0; i < NumOfFacets; i++)
  {
    for (j = 0; j < 3; j++)
    {
      //VEC p1 = VecMulMatr(Vertexes[Facets[i][j]], WVP);
    
      glColor3d(j % 2, (i + j) % 2, i % 2);
      //glVertex3d(p1.X, p1.Y, p1.Z / 1);
      glVertex3fv(&Vertexes[Facets[i][j]].X);
    }
  }
  glEnd();


  glBegin(GL_TRIANGLES);
    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 0);
    glColor3d(0, 1, 0);
    glVertex3d(0, 1, 0);
    glColor3d(1, 0, 0);
    glVertex3d(1, 0, 0);
  glEnd();
  /**/
  glUseProgram(0);
} /* End of 'CowRender' function */

/* Функция инициализации коровы.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       COW *Unit;
 *   - указатель на систему анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CowInit( COW *Unit, vg4ANIM *Ani )
{
  if (Unit->No == 1)
    CowProg = ShadProgInit("a.vert", "a.frag");
} /* End of 'CowInit' function */

/* Функция деинициализации объекта.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       COW *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID TestClose( COW *Unit )
{
  free(Vertexes);
} /* End of 'TestClose' function */

/* Функция сроздания новой мегакоровы.
 * АРГУМЕНТЫ:
 *   - позиция:
 *       DBL X, Y, Z;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (vg4UNIT *) указатель на созданный объект;
 */
vg4UNIT * CowCreate( DBL X, DBL Y, DBL Z )
{
  COW *Unit;

  if ((Unit = (COW *)VG4_UnitCreate(sizeof(COW))) == NULL)
    return NULL;

  Unit->Init = (vg4UNIT_RENDER)CowInit;
  Unit->Render = (vg4UNIT_RENDER)CowRender;
  Unit->Close = (vg4UNIT_CLOSE)TestClose;
  Unit->X = X;
  Unit->Y = Y;
  Unit->Z = Z;
  Unit->No = ++CurrentNo;
  if (Unit->No == 1)
    LoadCow();
  return (vg4UNIT *)Unit;
} /* End of 'CubeCreate' function */

/* END OF 'MEGACUBE.C' FILE */
