/* FILE NAME: TESTUNIT.C
 * PROGRAMMER: bg3
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
  BG3_UNIT_BASE_FUNCS;
  INT X, Y, Z;   /* Позиция объекта  */
  INT No;        /* Порядковый номер объекта */
  bg3GOBJ G,LAND; /* Тестовый примитив */
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
 *       bg3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static bg3PRIM p;

static INT x=0,y=0,z=0;

static VOID TestRender( TEST *Unit, bg3ANIM *Ani )
{
  INT j,i;
  FLT s;
  MATR M, WVP;
  VEC dvec = VecSet(0, 0, 0);

  Ani->camera.Wh = 0.70 * Ani->W / Ani->H;
  Ani->camera.Hh = 0.70;
  Ani->camera.PD = 1.0;


  


  M = MatrMulMatr (MatrMulMatr(MatrScale(Ani->cam, Ani->cam, Ani->cam),MatrTranslate(Ani->AtX,Ani->AtY,0)),MatrRotate(Ani->angle,0,1,0));

  Ani->camera.Wh = 0.70 * Ani->W / Ani->H;
  Ani->camera.Hh = 0.70;
  Ani->camera.PD = 1.0;

  Ani->camera.MVP = M;
  Ani->camera.viewMatr = MatrLookAt(VecSet(Unit->G.X+30,Unit->G.Y+30,Unit->G.Z+30), VecSet(Unit->G.X,Unit->G.Y,Unit->G.Z), VecSet(0, 1, 0));
  Ani->camera.projMatr = MatrFrustum(-Ani->camera.Wh / 2, Ani->camera.Wh / 2,-Ani->camera.Hh / 2, Ani->camera.Hh / 2,Ani->camera.PD, 100000);


  WVP = MatrMulMatr(MatrMulMatr(Ani->camera.MVP, Ani->camera.viewMatr), Ani->camera.projMatr);


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
    glUniformMatrix4fv(j, 1, FALSE, Ani->camera.MVP.A[0]);

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

  glColor3d(1, 1, 1);

  BG3_GeomDraw(&Unit->G);
  BG3_GeomDraw(&Unit->LAND);


  glUseProgram(0);
  glPopAttrib();
} /* End of 'TestRender' function */

/* Функция инициализации объекта.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       TEST *Unit;
 *   - указатель на систему анимации:
 *       bg3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID TestInit( TEST *Unit, bg3ANIM *Ani )

{
  if (Unit->No == 1)
    ShaderProg = ShadProgInit("a.vert", "a.frag");
  Ani->cam = 30;
  Ani->angle = 0;
  Ani->AtX = 0;
  Ani->AtX = 0;
  Ani->s = 8;
  Ani->x = 0;
  Ani->y = 0;
  Ani->z = 0;

} /* End of 'TestInit' function */

/* Функция сроздания примерного объекта.
 * АРГУМЕНТЫ:
 *   - позиция:
 *       DBL X, Y, Z;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (bg3UNIT *) указатель на созданный объект;
 */

VOID TestUnitResponse( TEST *Unit, bg3ANIM *Ani )
{   
  if(Ani->keyState.actual['A'])
    Unit->G.X -= 5;
  if(Ani->keyState.actual['D'])
    Unit->G.X += 5;
  if(Ani->keyState.actual['W'])
    Unit->G.Z -= 5;
  if(Ani->keyState.actual['S'])
    Unit->G.Z += 5;
  if(Ani->keyState.actual['E'])
    Unit->G.Y += 5;
  if(Ani->keyState.actual['Q'])
    Unit->G.Y -= 5;



  if(Ani->keyState.actual['L'])
    Ani->angle += 1;
  if(Ani->keyState.actual['J'])
    Ani->angle -= 1;
  if(Ani->keyState.actual['I'])
    Ani->cam += 2;
  if(Ani->keyState.actual['K'])
    Ani->cam -= 2;


}


bg3UNIT * TestUnitCreate( DBL X, DBL Y, DBL Z)
{

  TEST *Unit;
  bg3MATERIAL DefMat;


  DefMat.Ka = VecSet(0.1, 0.1, 0.1);
  DefMat.Kd = VecSet(0.9, 0.9, 0.9);
  DefMat.Ks = VecSet(0.0, 0.0, 0.0);
  DefMat.Phong = 30;
  DefMat.Trans = 0;
  strcpy(DefMat.MapD, "cDepth1000_pdrv.bmp");
  strcpy(DefMat.Name, "unknown");

  if ((Unit = (TEST *)BG3_UnitCreate(sizeof(TEST))) == NULL)
    return NULL;

  Unit->Init = (bg3UNIT_INIT)TestInit;
  Unit->Render = (bg3UNIT_RENDER)TestRender;
  Unit->Response = (bg3UNIT_RESPONSE)TestUnitResponse;
  Unit->G.X = X;
  Unit->G.Y = Y;
  Unit->G.Z = Z;
  Unit->No = ++CurrentNo;

  
  
  BG3_PrimDefaultColor = VecSet(0, 1, 0);
  ///BG3_PrimCreatePlane(&p, 300, 300, VecSet(-5, -1, 5), VecSet(10, 0, 0), VecSet(0, 0, -10));
  BG3_PrimCreateHeightField( &p, "cDepth1000_pdrv.bmp", VecSet(-5, -1, 5), VecSet(1000, 0, 0), VecSet(0, 0, -1000));
  BG3_GeomAddPrim(&Unit->LAND,&p);
  BG3_GeomAddMat(&Unit->LAND,&DefMat);
  BG3_GeomLoad(&Unit->G, "Mark 42.obj");
  return (bg3UNIT *)Unit;
} /* End of 'TestUnitCreate' function */

/* END OF 'TESTUNIT.C' FILE */
