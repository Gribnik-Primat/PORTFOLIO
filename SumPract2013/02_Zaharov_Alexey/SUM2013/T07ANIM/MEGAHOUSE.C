/* Zakharov Alexey, 10-1, 14.06, Summer Practise 2013 */

#include <stdlib.h>
#include "anim.h"

/* Тип представления объекта */
typedef struct
{
  /* Включаем базовые поля объекта анимации */
  AZ1_UNIT_BASE_FUNCS;
  INT X, Y, Z;   /* Позиция объекта  */
  INT No;        /* Порядковый номер объекта */
  az1GOBJ G; /* Тестовый примитив */
} TEST;

static INT CurrentNo = 0; /* очередной порядковый номер */
static UINT ShaderProg;

/* Функция отрисовки дома.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       TEST *Unit;
 *   - указатель на систему анимации:
 *       az1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID TestHouseRender( TEST *Unit, az1ANIM *Ani )
{
  INT j;
  FLT s;
  MATR M, WVP;

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

  Ani->Mworld = M;
  Ani->Mview = MatrViewLookAt(VecSet(50, 50, -500), VecSet(0, 0, 0), VecSet(0, 1, 0));
  Ani->Mproj = MatrProject(-Ani->Wp / 2, Ani->Wp / 2,
                                 -Ani->Hp / 2, Ani->Hp / 2,
                                 Ani->PD, 15000);

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

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  if (Ani->Keys['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON);

  glColor3d(1, 1, 1);
  AZ1_GeomDraw(&Unit->G);

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
static VOID TestHouseInit( TEST *Unit, az1ANIM *Ani )
{
  if (Unit->No == 1)
    ShaderProg = ShadProgInit("a.vert", "a.frag");
} /* End of 'TestInit' function */
 
az1UNIT * TestHouseCreate( FLT X, FLT Y, FLT Z, INT HouseNumber )
{
  TEST *Unit;
  az1PRIM p;
  CHAR Extension[]=".object", FileNameEnding[2], FileName[]={"Models//Houses//house\0\0\0\0\0\0\0\0"};

  /* Строим путь */
  _itoa( HouseNumber+1, FileNameEnding, 10 );
  strcat( FileName, FileNameEnding );
  strcat( FileName, Extension );
  /* */

  if ((Unit = (TEST *)AZ1_UnitCreate(sizeof(TEST))) == NULL)
    return NULL;

  Unit->Init = (az1UNIT_RENDER)TestHouseInit;
  Unit->Render = (az1UNIT_RENDER)TestHouseRender;
  //Unit->Response = (az1UNIT_RESPONSE)Te
  Unit->X = X;
  Unit->Y = Y;
  Unit->Z = Z;
  Unit->No = ++CurrentNo;
  AZ1_GeomAddPrim(&Unit->G, &p);
  AZ1_GeomLoad(&Unit->G, FileName);
  return (az1UNIT *)Unit;
}