/* FILE NAME: TESTUNIT.C
* PROGRAMMER: az1
* DATE: 10.06.2013
* PURPOSE: Animation sample module.
*/

#include <stdio.h>
#include <stdlib.h>

#include "anim.h"
#include "heightmap.h"
#include <time.h>

/***
* Функции обработки объекта
***/

static UINT ShaderProg;

/* Тип представления объекта */
typedef struct
{
  /* Включаем базовые поля объекта анимации */
  AZ1_UNIT_BASE_FUNCS;
  INT X, Y, Z;   /* Позиция объекта  */
  INT No;        /* Порядковый номер объекта */
  az1GOBJ G, Land; /* Тестовый примитив */
} TEST;

static INT CurrentNo = 0; /* очередной порядковый номер */

/* Функция получения случайного счисла от 0 до 1 */
FLT r0( VOID )
{
  return (FLT)rand() / RAND_MAX;
} /* End of 'r0' function */

/* Функция получения случайного  счисла от -1 до 1 */
FLT r1( VOID )
{
  return 2.0 * rand() / RAND_MAX - 1;
} /* End of 'r1' function */


static VOID TestResponce( TEST *Unit, az1ANIM *Ani )
{
}

INT dx=0, dy=0, dz=0;     // координаты самолета
BOOL IsSlew = FALSE;      // включен ли режим свободного, не привязанного к самолету перемещения камеры
INT slx=0, sly=0, slz=0;  // координаты камеры в режиме перемещения

/* Функция отрисовки объекта.
* АРГУМЕНТЫ:
*   - указатель на "себя":
*       TEST *Unit;
*   - указатель на систему анимации:
*       az1ANIM *Ani;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID TestRender( TEST *Unit, az1ANIM *Ani )
{
  INT j;
  FLT s;
  MATR M, WVP;
  //static dx, dy, dz;

  Ani->Wp = 0.70 * Ani->W / Ani->H;
  Ani->Hp = 0.70;
  Ani->PD = 1.0;

  Ani->Time = clock();

  s = 10;
  M = MatrScale(s, s, s);
  //M = MatrUnit();

  Ani->Wp = 0.70 * Ani->W / Ani->H;
  Ani->Hp = 0.70;
  Ani->PD = 1.0;

  switch (Ani->Jpov)
  {
  case 1:
    dz -= 1;
    break;
  case 2:
    dx += sqrt(2);
    dz -= sqrt(2);
    break;
  case 3:
    dx -= 1;  
    break;
  case 4:
    dx += sqrt(2);
    dz += sqrt(2);
    break;
  case 5:
    dz += 1;
    break;
  case 6:
    dx -= sqrt(2);
    dz += sqrt(2);   
    break;
  case 7:
    dx += 1;
    break;
  case 8:
    dz -= sqrt(2);
    dx -= sqrt(2);  
    break;
  default:
    break;
  };

  if (!IsSlew)
  {
    SetWindowText(Ani->hWnd, "T07ANIM");
    Ani->Mworld = MatrUnit();
    Ani->Mview = MatrViewLookAt(VecSet(0, 0, -500), VecSet(0, 0, 0), VecSet(0, 1, 0));

    Ani->Wp = 0.70 * Ani->W / Ani->H;
    Ani->Hp = 0.70;
    Ani->PD = 1.0;

    Ani->Mproj = MatrProject(-Ani->Wp / 2, Ani->Wp / 2,
      -Ani->Hp / 2, Ani->Hp / 2,
      Ani->PD, 10000);
  }
  else
  {
    if (Ani->JBut[0])
      slx -= 2;
    if (Ani->JBut[1])
      slz += 2;
    if (Ani->JBut[2])
      slz -= 2;
    if (Ani->JBut[3])
      slx += 2;

    SetWindowText(Ani->hWnd, "Slew");
    Ani->Mworld = MatrMulMatr(MatrRotateX(90), MatrTranslate(slx, sly, slz));
    Ani->Mview = MatrViewLookAt(VecSet(Ani->Jx * 5, Ani->Jy * 5, -Ani->Jz * 5 - 30), VecSet(dx, dy, dz), VecSet(0, 1, 0));
    Ani->Mproj = MatrProject(-Ani->Wp / 2, Ani->Wp / 2,
        -Ani->Hp / 2, Ani->Hp / 2,
        Ani->PD, 10000);
  }

  /* if (Unit->No == 1)
  {
  Ani->Mworld = MatrMulMatr(MatrRotateY(90), MatrMulMatr(MatrMulMatr(MatrRotateX(90), MatrRotateY(0)), MatrScale(s, s, s)));
  Ani->Mview = MatrViewLookAt(VecSet(500 + Unit->X, 300 + Unit->Y + Ani->gamma * 100, Unit->Z), VecSet(0, 0, 0), VecSet(0, 1, 0));
  }
  else
  Ani->Mworld = MatrMulMatr(MatrTranslate(Unit->X + Ani->Time * 10 * cos(Ani->alpha), Unit->Y, Unit->Z + Ani->Time * 10 * sin(Ani->alpha)),
  MatrRotateY(Ani->alpha));

  Ani->Mproj = MatrProject(-Ani->Wp / 2, Ani->Wp / 2,
  -Ani->Hp / 2, Ani->Hp / 2,
    Ani->PD, 15000);
 */
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

  /* передаем параметры материалов */
  /*
  j = glGetUniformLocation(ShaderProg, "Ka");
  if (j != -1)
    glUniform3fv(j, 1, &Go->Mats[n].Ka.X);
  j = glGetUniformLocation(ShaderProg, "Kd");
  if (j != -1)
    glUniform3fv(j, 1, &Go->Mats[n].Kd.X);
  j = glGetUniformLocation(ShaderProg, "Ks");
  if (j != -1)
    glUniform3fv(j, 1, &Go->Mats[n].Ks.X);
  j = glGetUniformLocation(ShaderProg, "Phong");
  if (j != -1)
    glUniform1f(j, Go->Mats[n].Phong);
  j = glGetUniformLocation(ShaderProg, "Alpha");
  if (j != -1)
    glUniform1f(j, Go->Mats[n].Trans);

  j = glGetUniformLocation(AZ1_GeomShadProgId, "UseColor");
  if (j != -1)
    glUniform4f(j, 1.0f, 1.0f, 1.0f, 1.0f);
  */
  // RenderHeightMap();

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  if (Ani->Keys['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  if (Ani->Keys['Y'] && !Ani->OldKeys['Y'])
    IsSlew = !IsSlew;

  //if (Ani->JButOld[6] && !Ani->JBut[6])
  //  IsSlew = !IsSlew;
  
  glColor3d(1, 1, 1);
 // AZ1_GeomDraw(&Unit->G);  
  AZ1_GeomDraw(&Unit->Land);

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
*       az1ANIM *Ani;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID TestInit( TEST *Unit, az1ANIM *Ani )
{
  if (Unit->No == 1)
    ShaderProg = ShadProgInit("geom.vert", "geom.frag");
} /* End of 'TestInit' function */

/* Функция создания примерного объекта.
* АРГУМЕНТЫ:
*   - позиция:
*       FLT X, Y, Z;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (az1UNIT *) указатель на созданный объект;
*/
az1UNIT * TestUnitCreate( FLT X, FLT Y, FLT Z, CHAR *FileName )
{
  TEST *Unit;
  az1PRIM p;
  az1MATERIAL Mat;

  if ((Unit = (TEST *)AZ1_UnitCreate(sizeof(TEST))) == NULL)
    return NULL;

  Unit->Init = (az1UNIT_RENDER)TestInit;
  Unit->Render = (az1UNIT_RENDER)TestRender;
  Unit->Response = (az1UNIT_RESPONSE)TestResponce;
  Unit->X = X;
  Unit->Y = Y;
  Unit->Z = Z;
  Unit->No = ++CurrentNo;
  //AZ1_PrimDefaultColor = VecSet(1, 0, 0);
  //AZ1_PrimCreateSphere(&p, 300, 300, VecSet(0, 0, 0), 1);
 // AZ1_GeomAddPrim(&Unit->G, &p);
  //AZ1_PrimDefaultColor = VecSet(0, 1, 0);
 // AZ1_PrimCreatePlane(&p, 300, 300, VecSet(-5, -1, 5), VecSet(10, 0, 0), VecSet(0, 0, -10));
 // AZ1_GeomAddPrim(&Unit->G, &p);
  AZ1_GeomLoad(&Unit->G, FileName);

  Mat.Ka = VecSet(0.1, 0.1, 0.1);
  Mat.Kd = VecSet(0.8, 0.8, 0.8);
  Mat.Ks = VecSet(0.8, 0.8, 0.8);
  Mat.Phong = 30;        
  strcpy(Mat.MapD, "HF\\map-2hf.bmp");    
  
  AZ1_PrimCreateHeightField(&p, "HF\\hf.bmp", VecSet(0, 0, 0), VecSet(900, 0, 0), VecSet(0, 0, -900));
  AZ1_GeomAddMat(&Unit->Land, &Mat);
  LoadMaterials(&Unit->Land,"HF\\map-2hf.bmp");

  return (az1UNIT *)Unit;
} /* End of 'TestUnitCreate' function */

/* END OF 'TESTUNIT.C' FILE */    