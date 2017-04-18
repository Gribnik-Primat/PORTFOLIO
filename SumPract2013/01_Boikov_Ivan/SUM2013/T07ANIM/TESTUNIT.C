/* FILE NAME: TESTUNIT.C
 * PROGRAMMER: IB1
 * DATE: 10.06.2013
 * PURPOSE: Animation sample module.
 */

#include <stdio.h>
#include <stdlib.h>

#include "anim.h"
#include "gobj.h"

/***
 * Функции обработки объекта
 ***/

static UINT ShaderProg;

/* Тип представления объекта */
typedef struct
{
  /* Включаем базовые поля объекта анимации */
  IB1_UNIT_BASE_FUNCS;
  INT X, Y, Z;   /* Позиция объекта  */
  INT No;        /* Порядковый номер объекта */
  IB1GOBJ G; /* Тестовый примитив */
  IB1GOBJ Land;
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
 *       IB1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID TestRender( TEST *Unit, IB1ANIM *Ani )
{
  INT j;
  FLT s;                       
  MATR M, WVP;

  Ani->Wp = 0.70 * Ani->W / Ani->H;
  Ani->Hp = 0.70;
  Ani->PD = 1.0;

  s = 2;
  
  if (Unit->No == 1)
  {
    Ani->Mworld = MatrMulMatr(MatrScale(50, 50, 50), MatrMulMatr(MatrTranslate(Unit->X, Unit->Y + 100, Unit->Z), MatrMulMatr(MatrRotateY(3.14 / 2  + sin(Ani->Time) / 20 + Ani->Jx / 8), MatrMulMatr(MatrMulMatr(MatrRotateX(3.14 / 2 + sin(Ani->Time) / 100), MatrRotateZ(sin(Ani->Time) / 100)), MatrScale(s, s, s)))));
    Ani->Mview = MatrViewLookAt(VecSet(200 + Unit->X, 100 + Unit->Y + Ani->gamma * 100, Unit->Z), VecSet(0, 1000, 0), VecSet(0, 1, 0));
  }
  //else if ((Unit->No - 1) % 2 == 0)
  //  Ani->Mworld = MatrMulMatr(MatrScale(0.01, 0.01, 0.01), MatrMulMatr(MatrTranslate(Unit->X + Ani->Time * 30 * cos(Ani->alpha), Unit->Y, Unit->Z + Ani->Time * 30 * sin(Ani->alpha)), MatrRotateY(Ani->alpha)));
//  else if (Unit->No >= 2)
   // Ani->Mworld = MatrMulMatr(MatrTranslate(Unit->X, Unit->Y, Unit->Z), MatrScale(1000, 0.001, 1000));
  else
    Ani->Mworld = MatrMulMatr(MatrScale(4, 4, 4), MatrMulMatr(MatrTranslate(Unit->X + Ani->Time * 800 * cos(Ani->alpha), Unit->Y + Ani->Time * 50 * sin(Ani->betha), Unit->Z + Ani->Time * 800 * sin(Ani->alpha)), MatrRotateY(Ani->alpha)));

  Ani->Wp = 0.70 * Ani->W / Ani->H;
  Ani->Hp = 0.70;
  Ani->PD = 1.0;

  Ani->Mproj = MatrProject(-Ani->Wp / 2, Ani->Wp / 2,
                                 -Ani->Hp / 2, Ani->Hp / 2,
                                 Ani->PD, 10000);


  WVP = MatrMulMatr(MatrMulMatr(Ani->Mworld, Ani->Mview), Ani->Mproj);


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
    glUniformMatrix4fv(j, 1, FALSE, Ani->Mworld.A[0]);
  //IB1_GeomDraw(&Unit->Land);
  /* рисуем оси */
  /*glBegin(GL_LINES);
    glColor3d(1, 0, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(1, 0, 0, 0);

    glColor3d(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 1, 0, 0);

    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 0);
    glVertex4d(0, 0, 1, 0);
  glEnd();   */

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  if (Ani->Keys['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glColor3d(1, 1, 1);
  IB1_GeomDraw(&Unit->G);
  if (Unit->No == 2)
    IB1_GeomDraw(&Unit->Land);
  // RenderHeightMap();
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
 *       IB1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID TestInit( TEST *Unit, IB1ANIM *Ani )
{

  if (Unit->No < 3)
    ShaderProg = ShadProgInit("a.vert", "a.frag");
} /* End of 'TestInit' function */

static VOID TestClose( TEST *Unit )
{
  IB1_GeomClose(&Unit->G);
} /* End of 'TestClose' function */

/* Функция сроздания примерного объекта.
 * АРГУМЕНТЫ:
 *   - позиция:
 *       DBL X, Y, Z;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (IB1UNIT *) указатель на созданный объект;
 */
IB1UNIT * TestUnitCreate( DBL X, DBL Y, DBL Z, CHAR *FN )
{
  TEST *Unit;
  IB1PRIM p;
  IB1MATERIAL Mat;

  if ((Unit = (TEST *)IB1_UnitCreate(sizeof(TEST))) == NULL)
    return NULL;

  Unit->Init = (IB1UNIT_RENDER)TestInit;
  Unit->Close = (IB1UNIT_CLOSE)TestClose;
  Unit->Render = (IB1UNIT_RENDER)TestRender;
  Unit->X = X;
  Unit->Y = Y;
  Unit->Z = Z;
  Unit->No = ++CurrentNo; 
  IB1_GeomLoad(&Unit->G, FN);
  IB1_PrimDefaultColor = VecSet(0.8, 1, 0.1);
  Mat.Ka = VecSet(0.1, 0.1, 0.1);
  Mat.Kd = VecSet(0.8, 0.8, 0.8);
  Mat.Ks = VecSet(0.8, 0.8, 0.8);
  Mat.Phong = 30;        
  strcpy(Mat.MapD, "map2-hf.bmp");    
  
  IB1_PrimCreateHeightField(&p, "hf.bmp", VecSet(5000, -100, 5000), VecSet(0, 0, -10000), VecSet(-10000, 0, 0));
  IB1_GeomAddPrim(&Unit->Land, &p);
  IB1_GeomAddMat(&Unit->Land, &Mat);
  LoadMaterials(&Unit->Land,"map2-hf.bmp");
  //LoadMaterials(&Unit->Land,"map2-shadow.bmp");
  
  //IB1_PrimCreateHeightField(&p, "map2-hf.bmp", VecSet(0,0,0), VecSet(0, -10, 0), VecSet(0, 10, 0));
  //IB1_GeomAddPrim(&Unit->G, &p); 
 // if (Unit->No < 2)
  
  return (IB1UNIT *)Unit;
} /* End of 'TestUnitCreate' function */

/* END OF 'TESTUNIT.C' FILE */
