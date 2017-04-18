/* FILE NAME: TESTUNIT.C
 * PROGRAMMER: VG4
 * DATE: 10.06.2013
 * PURPOSE: Animation sample module.
 */

#include <stdio.h>
#include <stdlib.h>

#include "anim.h"

/***
 * Функции обработки объекта
 ***/

static UINT ShaderProg;

/* Тип представления объекта */
typedef struct
{
  /* Включаем базовые поля объекта анимации */
  VG4_UNIT_BASE_FUNCS;
  INT X, Y, Z;   /* Позиция объекта  */
  INT No;        /* Порядковый номер объекта */
  vg4GOBJ G; /* Тестовый примитив */
} TEST;

static INT CurrentNo = 0; /* очередной порядковый номер */

/* Функция получения случайного счисла от 0 до 1 */
FLT r0( VOID )
{
  return (FLT)rand() / RAND_MAX;
} /* End of 'r0' function */

/* Функция получения случайного счисла от -1 до 1 */
FLT r1( VOID )
{
  return 2.0 * rand() / RAND_MAX - 1;
} /* End of 'r1' function */


/* Функция отрисовки объекта.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       TEST *Unit;
 *   - указатель на систему анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID TestRender( TEST *Unit, vg4ANIM *Ani )
{
  INT j;
  FLT s;
  MATR M, WVP;

  Ani->Wp = 0.70 * Ani->W / Ani->H;
  Ani->Hp = 0.70;
  Ani->PD = 1.0;

  s = 8;
  M = MatrMulMatr(MatrMulMatr(MatrMulMatr(MatrRotateX(-90), MatrRotateZ(-88 * Ani->Time)), MatrScale(s, s, s)), MatrRotateY(Ani->Time * 30));
  //M = MatrUnit();

  Ani->Wp = 0.70 * Ani->W / Ani->H;
  Ani->Hp = 0.70;
  Ani->PD = 1.0;

  Ani->MatrWorld = M;
  Ani->MatrView = MatrViewLookAt(VecSet(30, 30, 30), VecSet(0, 0, 0), VecSet(0, 1, 0));
  Ani->MatrProj = MatrProjection(-Ani->Wp / 2, Ani->Wp / 2,
                                 -Ani->Hp / 2, Ani->Hp / 2,
                                 Ani->PD, 1000);


  WVP = MatrMulMatr(MatrMulMatr(Ani->MatrWorld, Ani->MatrView), Ani->MatrProj);


  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
  glUseProgram(ShaderProg);
  glColor3d(0, 0, 0);

  j = glGetUniformLocation(ShaderProg, "UseColor");
  if (j != -1)
    glUniform4d(j, 1.0, 1.0, 1.0, 1.0);
  j = glGetUniformLocation(ShaderProg, "Matr");
  if (j != -1)
    glUniformMatrix4fv(j, 1, FALSE, WVP.A[0]);
  j = glGetUniformLocation(ShaderProg, "MatrWorld");
  if (j != -1)
    glUniformMatrix4fv(j, 1, FALSE, Ani->MatrWorld.A[0]);

  /* рисуем оси */
  glBegin(GL_LINES);
    glColor3d(1, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(1, 0, 0, 0);

    glColor3d(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 1, 0, 0);

    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 0, 1, 0);
  glEnd();

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  if (Ani->Keys['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glColor3d(1, 1, 1);
  VG4_GeomDraw(&Unit->G);

  /* рисуем треугольники */
  /*
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
  glPopAttrib();
} /* End of 'TestRender' function */

/* Функция инициализации объекта.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       TEST *Unit;
 *   - указатель на систему анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID TestInit( TEST *Unit, vg4ANIM *Ani )
{
  if (Unit->No == 1)
    ShaderProg = ShadProgInit("a.vert", "a.frag");
} /* End of 'TestInit' function */

/* Функция сроздания примерного объекта.
 * АРГУМЕНТЫ:
 *   - позиция:
 *       DBL X, Y, Z;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (vg4UNIT *) указатель на созданный объект;
 */
vg4UNIT * TestUnitCreate( DBL X, DBL Y, DBL Z )
{
  TEST *Unit;
  vg4PRIM p;

  if ((Unit = (TEST *)VG4_UnitCreate(sizeof(TEST))) == NULL)
    return NULL;

  Unit->Init = (vg4UNIT_RENDER)TestInit;
  Unit->Render = (vg4UNIT_RENDER)TestRender;
  Unit->X = X;
  Unit->Y = Y;
  Unit->Z = Z;
  Unit->No = ++CurrentNo;
  VG4_PrimDefaultColor = VecSet(1, 0, 0);
  VG4_PrimCreateSphere(&p, 300, 300, VecSet(0, 0, 0), 1);
  VG4_GeomAddPrim(&Unit->G, &p);
  VG4_PrimDefaultColor = VecSet(0, 1, 0);
  VG4_PrimCreatePlane(&p, 300, 300, VecSet(-5, -1, 5), VecSet(10, 0, 0), VecSet(0, 0, -10));
  VG4_GeomAddPrim(&Unit->G, &p);
  VG4_GeomLoad(&Unit->G, "cessna172.object");
  return (vg4UNIT *)Unit;
} /* End of 'TestUnitCreate' function */

/* END OF 'TESTUNIT.C' FILE */
