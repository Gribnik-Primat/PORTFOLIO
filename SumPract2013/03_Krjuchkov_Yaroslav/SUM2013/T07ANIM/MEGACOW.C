/* FILE NAME: PLANEUNIT.C
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
} PLANE;

static INT CurrentNo = 0; /* очередной порядковый номер */

/* Функция отрисовки объекта.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       PLANE *Unit;
 *   - указатель на систему анимации:
 *       ik1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PlaneRender( PLANE *Unit, ik1ANIM *Ani )
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

  MJ = MatrUnit();//MatrTranslate(Ani->Jx + cos(Ani->Time) * 10, Ani->Jy, Ani->Jz);
  //Rudder = MatrRotateY( Ani->Time * Ani->Jr * 5);
  Elevators = MatrRotateX(-0.6 * Ani->Jy);
  Rudder = MatrRotateY(-0.5 * Ani->Jx);

  

  Flying = MatrTranslate(Unit->X, Unit->Y, Unit->Z);

  FlightControlSurfaces = MatrMulMatr(MatrMulMatr(Elevators, Rudder),Flying);//MatrMulMatr(MatrMulMatr(Aileron, Rudder), Elevators);

  s = Ani->Jz * 1.5 + 1 ;
//  M = MatrMulMatr(MatrMulMatr(MatrMulMatr(MatrRotateY(26.7), MatrRotateX(0)), MatrRotateZ(-90/*0 * Ani->Time * 3*/)),MatrScale(s, s, s));
 // s = MatrUnit();

  Ani->Wp = 0.70 * Ani->W / Ani->H;
  Ani->Hp = 0.70;
  Ani->PD = 1.0;

  Ani->MatrWorld = MatrUnit
    (); //MatrMulMatr(M, FlightControlSurfaces);
  Ani->MatrView = MatrViewLookAt(VecSet(600, 600, 1000), VecSet(0, 0, 0), VecSet(0, 1, 0));
  Ani->MatrProj = MatrProjection(-Ani->Wp / 2, Ani->Wp / 2,
                                 -Ani->Hp / 2, Ani->Hp / 2,
                                 Ani->PD, 10000);


  WVP = MatrMulMatr(MatrMulMatr(Ani->MatrWorld, Ani->MatrView), Ani->MatrProj);


  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
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
  IK1_GeomDraw(&Unit->Land);

  glUseProgram(0);
  glPopAttrib();
} /* End of 'PlaneRender' function */

/* Функция инициализации объекта.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       PLANE *Unit;
 *   - указатель на систему анимации:
 *       ik1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID PlaneInit( PLANE *Unit, ik1ANIM *Ani )
{
  if (Unit->No == 1)
    ShaderProg = ShadProgInit("geom.vert", "geom.frag");
} /* End of 'PlaneInit' function */

/* Функция сроздания примерного объекта.
 * АРГУМЕНТЫ:
 *   - позиция:
 *       DBL X, Y, Z;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ik1UNIT *) указатель на созданный объект;
 */
ik1UNIT * PlaneUnitCreate( FLT X, FLT Y, FLT Z )
{
  PLANE *Unit;
  ik1PRIM p;
  ik1MATERIAL Mat;

  if ((Unit = (PLANE *)IK1_UnitCreate(sizeof(PLANE))) == NULL)
    return NULL;
 
  Unit->No = ++CurrentNo;
  Unit->Init = (ik1UNIT_RENDER)PlaneInit;
  Unit->Render = (ik1UNIT_RENDER)PlaneRender;
  Unit->X = X;
  Unit->Y = Y;
  Unit->Z = Z;
  /*IK1_PrimDefaultColor = VecSet(1, 0, 0);
  IK1_PrimCreateSphere(&p, 300, 300, VecSet(0, 0, 0), 1);
  IK1_GeomAddPrim(&Unit->G, &p);              */
  //IK1_PrimDefaultColor = VecSet(0, 1, 0);
  //Mat.Ka = VecSet(0.1, 0.1, 0.1);
  //Mat.Kd = VecSet(0.8, 0.8, 0.8);
  //Mat.Ks = VecSet(0.8, 0.8, 0.8);
  //Mat.Phong = 30;        
  //strcpy(Mat.MapD, "map-2hf.bmp");    
  
  //IK1_PrimCreateHeightField(&p, "hf1.bmp", VecSet(-500, -50, 500), VecSet(1000, 0, 0), VecSet(0, 0, -1000));
  //IK1_GeomAddPrim(&Unit->Land, &p);
  //IK1_GeomAddMat(&Unit->Land, &Mat);
  //LoadMaterials(&Unit->Land,"map-2hf.bmp");
  IK1_GeomLoad(&Unit->G, "cessna172.obj");
  return (ik1UNIT *)Unit;
} /* End of 'PlaneUnitCreate' function */

/* END OF 'PLANEUNIT.C' FILE */
