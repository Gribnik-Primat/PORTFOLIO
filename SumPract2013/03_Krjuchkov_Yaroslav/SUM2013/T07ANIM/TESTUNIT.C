/* FILE NAME: TESTUNIT.C
 * PROGRAMMER: IK1
 * DATE: 10.06.2013
 * PURPOSE: Animation sample module.
 */

#include <stdio.h>
#include <stdlib.h>

#include "anim.h"
#include "heightmap.h"

/***
 * Функции обработки объекта
 ***/

static UINT ShaderProg;

/* Тип представления объекта */
typedef struct
{
  /* Включаем базовые поля объекта анимации */
  IK1_UNIT_BASE_FUNCS;
  INT X, Y, Z;   /* Позиция объекта  */
  INT No;        /* Порядковый номер объекта */
  ik1GOBJ G;     /* Тестовый примитив */
  ik1GOBJ Land;
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
 *       ik1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID TestRender( TEST *Unit, ik1ANIM *Ani )
{
  INT j;
  FLT s, x = 0, y = 0, z = 0;
  MATR M, MJ, MRZ, WVP, Aileron, Rudder, Elevators, Flying, FlightControlSurfaces;
   switch (Ani->Jpov)
  {
  case 1:
    Unit->Y -= 0.1;
    break;
  case 2:
    ///Unit->X += sqrt(2) * 0.1;
   /// Unit->Y -= sqrt(2) * 0.1;
    break;
  case 3:
    Unit->X -= Ani->Time;  
    break;
  case 4:
    ///Unit->X += sqrt(2) * 0.1;
   /// Unit->Y += sqrt(2) * 0.1;
    break;
  case 5:
     Unit->Y += 0.1;
    break;
  case 6:
   ///Unit->X -= sqrt(2) * 0.1;
   ///Unit->Y += sqrt(2) * 0.1;   
    break;
  case 7:
    Unit->X += 0.1;
    break;
  case 8:
    Unit->Y -= sqrt(2) * 0.1;
    Unit->X -= sqrt(2) * 0.1;  
    break;
  default:
    break;
  };                           

  Ani->Wp = 0.70 * Ani->W / Ani->H;
  Ani->Hp = 0.70;
  Ani->PD = 1.0;
 /*
  MJ = MatrUnit();//MatrTranslate(Ani->Jx + cos(Ani->Time) * 10, Ani->Jy, Ani->Jz);
  //Rudder = MatrRotateY( Ani->Time * Ani->Jr * 5);
  Elevators = MatrRotateX(-0.6 * Ani->Jy);
  Rudder = MatrRotateY(-0.5 * Ani->Jx);

  

  Flying = MatrTranslate(Unit->X, Unit->Y, Unit->Z);

  FlightControlSurfaces = MatrMulMatr(MatrMulMatr(Elevators, Rudder),Flying);//MatrMulMatr(MatrMulMatr(Aileron, Rudder), Elevators);

  s = Ani->Jz * 3.5 + 1 ;
//  M = MatrMulMatr(MatrMulMatr(MatrMulMatr(MatrRotateY(26.7), MatrRotateX(0)), MatrRotateZ(-90/*0 * Ani->Time * 3)),MatrScale(s, s, s));
 // s = MatrUnit();

  Ani->Wp = 0.70 * Ani->W / Ani->H;
  Ani->Hp = 0.70;
  Ani->PD = 1.0;
  M = MatrMulMatr(MatrTranslate(0, 10,0), MatrMulMatr(MatrMulMatr(MatrMulMatr(MatrRotateY(26.7), MatrRotateX(0)), MatrRotateZ(-60/*0 * Ani->Time * 3)),MatrScale(2 * s, 2 * s, 2 * s))); 

  Ani->MatrWorld = M;
     //MatrMulMatr(M, FlightControlSurfaces);
  Ani->MatrView = MatrViewLookAt(VecSet(300, 10, 0), VecSet(0, 0, 0), VecSet(0, 1, 0));
  Ani->MatrProj = MatrProjection(-Ani->Wp / 2, Ani->Wp / 2,
                                 -Ani->Hp / 2, Ani->Hp / 2,
                                 Ani->PD, 10000);


  WVP = MatrMulMatr(MatrMulMatr(Ani->MatrWorld, Ani->MatrView), Ani->MatrProj);




  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */

   s = 2;
  
  if (Unit->No == 1)
  {
    Ani->MatrWorld = MatrMulMatr(MatrRotateX(0), MatrMulMatr(MatrScale(50, 50, 50), MatrMulMatr(MatrTranslate(Unit->X, Unit->Y + 100, Unit->Z), MatrMulMatr(MatrRotateY(3.14 / 2  + sin(Ani->Time) / 20 + Ani->Jx / 2), MatrMulMatr(MatrMulMatr(MatrRotateX(3.14 / 2 + sin(Ani->Time) / 100 +Ani->Jy / 10), MatrRotateZ(sin(Ani->Time) / 100)), MatrScale(s, s, s))))));
    Ani->MatrView = MatrViewLookAt(VecSet(200 + Unit->X, 100 + Unit->Y + Ani->gamma * 100, Unit->Z), VecSet(0, 1000, 0), VecSet(0, 1, 0));
  }
  //else if ((Unit->No - 1) % 2 == 0)
  //  Ani->Mworld = MatrMulMatr(MatrScale(0.01, 0.01, 0.01), MatrMulMatr(MatrTranslate(Unit->X + Ani->Time * 30 * cos(Ani->alpha), Unit->Y, Unit->Z + Ani->Time * 30 * sin(Ani->alpha)), MatrRotateY(Ani->alpha)));
//  else if (Unit->No >= 2)
   // Ani->Mworld = MatrMulMatr(MatrTranslate(Unit->X, Unit->Y, Unit->Z), MatrScale(1000, 0.001, 1000));
  else
    Ani->MatrWorld = MatrMulMatr(MatrScale(4, 4, 4), MatrMulMatr(MatrTranslate(Unit->X + Ani->Time * 300 * cos(Ani->alpha), Unit->Y + Ani->Time * 100 * sin(Ani->betha), Unit->Z + Ani->Time * 300 * sin(Ani->alpha)), MatrRotateY(Ani->alpha)));
  //M = MatrUnit();

  Ani->Wp = 0.70 * Ani->W / Ani->H;
  Ani->Hp = 0.70;
  Ani->PD = 1.0;

  Ani->MatrProj = MatrProjection(-Ani->Wp / 2, Ani->Wp / 2,
                                 -Ani->Hp / 2, Ani->Hp / 2,
                                 Ani->PD, 10000);


  WVP = MatrMulMatr(MatrMulMatr(Ani->MatrWorld, Ani->MatrView), Ani->MatrProj);


  glUseProgram(ShaderProg);
  glColor3d(0, 0, 0);

//  RenderHeightMap();

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
  glEnd();
   */
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  //if (Ani->Keys['W'])
  //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glColor3d(1, 1, 1);
  IK1_GeomDraw(&Unit->G);
  if (Unit->No == 2)
    IK1_GeomDraw(&Unit->Land);

  /* рисуем треугольники */
 
  glBegin(GL_TRIANGLES);
    glColor3d(0, 0, 1);
    glVertex3d(0, 0, 0);
    glColor3d(0, 1, 0);
    glVertex3d(0, 1, 0);
    glColor3d(1, 0, 0);
    glVertex3d(1, 0, 0);
  glEnd();
  /**/
 // IK1_GeomDraw(&Unit->Land);
  //IK1_GeomDraw(&Unit->G);

  glUseProgram(0);
  glPopAttrib();
} /* End of 'TestRender' function */

/* Функция инициализации объекта.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       TEST *Unit;
 *   - указатель на систему анимации:
 *       ik1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID TestInit( TEST *Unit, ik1ANIM *Ani )
{
  if (Unit->No == 1)
    ShaderProg = ShadProgInit("a.vert", "a.frag");
} /* End of 'TestInit' function */

/* Функция сроздания примерного объекта.
 * АРГУМЕНТЫ:
 *   - позиция:
 *       DBL X, Y, Z;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ik1UNIT *) указатель на созданный объект;
 */
ik1UNIT * TestUnitCreate( FLT X, FLT Y, FLT Z, CHAR *FN )
{
  TEST *Unit;
  ik1PRIM p;
  ik1MATERIAL Mat, MatSh;

  if ((Unit = (TEST *)IK1_UnitCreate(sizeof(TEST))) == NULL)
    return NULL;
 
  Unit->No = ++CurrentNo;
  Unit->Init = (ik1UNIT_RENDER)TestInit;
  Unit->Render = (ik1UNIT_RENDER)TestRender;
  Unit->X = X;
  Unit->Y = Y;
  Unit->Z = Z;
  IK1_GeomLoad(&Unit->G, FN);
  /*IK1_PrimDefaultColor = VecSet(1, 0, 0);
  IK1_PrimCreateSphere(&p, 300, 300, VecSet(0, 0, 0), 1);
  IK1_GeomAddPrim(&Unit->G, &p);              */
  IK1_PrimDefaultColor = VecSet(0, 1, 0);
  Mat.Ka = VecSet(0.1, 0.1, 0.1);
  Mat.Kd = VecSet(0.8, 0.8, 0.8);
  Mat.Ks = VecSet(0.8, 0.8, 0.8);
  Mat.Phong = 30;        
  strcpy(Mat.MapD, "map-2hf.bmp");

  MatSh.Ka = VecSet(0.1, 0.1, 0.1);
  MatSh.Kd = VecSet(0.8, 0.8, 0.8);
  MatSh.Ks = VecSet(0.8, 0.8, 0.8);
  MatSh.Phong = 30;        
  strcpy(MatSh.MapD, "map-2shadow.bmp");
  
  IK1_PrimCreateHeightField(&p, "hf1.bmp", VecSet(-5000, -50, 5000), VecSet(10000, 0, 0), VecSet(0, 0, -10000));
  IK1_GeomAddPrim(&Unit->Land, &p);
  IK1_GeomAddMat(&Unit->Land, &Mat);
  IK1_GeomAddMat(&Unit->Land, &MatSh);
  LoadMaterials(&Unit->Land,"map-2hf.bmp");
  LoadMaterials(&Unit->Land,"map-2shadow.bmp");

  
  return (ik1UNIT *)Unit;
} /* End of 'TestUnitCreate' function */

/* END OF 'TESTUNIT.C' FILE */
