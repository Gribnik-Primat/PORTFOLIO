/* FILE NAME: TESTUNIT.C
 * PROGRAMMER: VG4
 * DATE: 14.06.2013
 * PURPOSE: Animation sample module.
 */

#include <stdio.h>
#include <stdlib.h>

#include "anim.h"

/***
 * Функции обработки объекта
 ***/

/* Тип представления объекта */
typedef struct
{
  /* Включаем базовые поля объекта анимации */
  VG4_UNIT_BASE_FUNCS;
  INT X, Y, Z;    /* Позиция объекта  */
  INT No;         /* Порядковый номер объекта */
  vg4GOBJ G[2];   /* Тестовые примитивы */
  UINT SndBuf[2]; /* Звуковой буфер */
  UINT SndSrc;    /* Источник звука */
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

/* Функция реакции на межкадровоек взаимодействие объекта.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       TEST *Unit;
 *   - указатель на систему анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID TestResponse( TEST *Unit, vg4ANIM *Ani )
{
  if (Ani->Keys['A'] && !Ani->KeysOld['A'])
    alSourcePlay(Unit->SndSrc);
  if (Ani->Keys['Q'] && !Ani->KeysOld['Q'])
    alSourcePause(Unit->SndSrc);
  if (Ani->Keys['Z'] && !Ani->KeysOld['Z'])
    alSourceRewind(Unit->SndSrc);
  if (Ani->Keys['S'])
    alSourceStop(Unit->SndSrc);

  if (Ani->Keys['1'] && !Ani->KeysOld['1'])
  {
    alSourcei(Unit->SndSrc, AL_BUFFER, Unit->SndBuf[0]);
    alSourcei(Unit->SndSrc, AL_LOOPING, 1);
  }
  if (Ani->Keys['2'] && !Ani->KeysOld['2'])
  {
    alSourcei(Unit->SndSrc, AL_BUFFER, Unit->SndBuf[1]);
    alSourcei(Unit->SndSrc, AL_LOOPING, 0);
  }
  if (Ani->Keys['3'] && !Ani->KeysOld['3'])
    alSourcei(Unit->SndSrc, AL_PITCH, 2.0);
  if (Ani->Keys['4'] && !Ani->KeysOld['4'])
    alSourcef(Unit->SndSrc, AL_PITCH, 0.5);

} /* End of 'TestResponse' function */

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
  INT i, j;
  FLT s;
  MATR M;

  if (Ani->W > Ani->H)
  {
    Ani->Wp = 0.70 * Ani->W / Ani->H;
    Ani->Hp = 0.70;
  }
  else
  {
    Ani->Wp = 0.70;
    Ani->Hp = 0.70 * Ani->H / Ani->W;
  }
  Ani->PD = 1.0;

  Ani->MatrWorld = MatrUnit();
  M = MatrRotateY(Ani->Time * 30);
  Ani->MatrView = MatrViewLookAt(VecMulMatr(VecMulNum(VecSet(30, 30, 30), Ani->Jz), M), VecSet(0, 0, 0), VecSet(0, 1, 0));
  Ani->MatrProj = MatrProjection(-Ani->Wp / 2, Ani->Wp / 2,
                                 -Ani->Hp / 2, Ani->Hp / 2,
                                 Ani->PD, 1000);

  glPushAttrib(GL_ALL_ATTRIB_BITS);

  if (Ani->Keys['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  if (Ani->Keys['F'])
    VG4_SetFullScreen(TRUE);
  if (Ani->Keys['G'])
    VG4_SetFullScreen(FALSE);


  for (i = -1; i <= 1; i++)
    for (j = -1; j <= 1; j++)
    {
      s = 1;
      M = MatrRotateX(-90);
      M = MatrMulMatr(M, MatrRotateY(90 * Ani->Jx));
      M = MatrMulMatr(M, MatrRotateZ(90 * Ani->Jy));
      M = MatrMulMatr(M, MatrRotateY(-Ani->Time * 8));
      M = MatrMulMatr(M, MatrTranslate(i * 8, 0, j * 8));
      Ani->MatrWorld = M;

      glEnable(GL_CULL_FACE);
      glCullFace(GL_FRONT);
      VG4_GeomDraw(&Unit->G[1]);
      glCullFace(GL_BACK);
      VG4_GeomDraw(&Unit->G[1]);
      glDisable(GL_CULL_FACE);
    }

  M = MatrRotateX(-90);
  M = MatrMulMatr(M, MatrRotateZ(sin(Ani->Time) * 30));
  M = MatrMulMatr(M, MatrRotateX(sin(Ani->Time * 1.11) * 27));
  M = MatrMulMatr(M, MatrTranslate(0, -5, 0));
  Ani->MatrWorld = M;
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  VG4_GeomDraw(&Unit->G[0]);
  glCullFace(GL_BACK);
  VG4_GeomDraw(&Unit->G[0]);
  glDisable(GL_CULL_FACE);

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
  INT format, size, freq;
  CHAR loop;
  VOID *data;

  alGenBuffers(2, Unit->SndBuf);
  alutLoadWAVFile("a.wav", &format, &data, &size, &freq, &loop); 
  alBufferData(Unit->SndBuf[0], format, data, size, freq);
  alutUnloadWAV(format, data, size, freq); 
  alutLoadWAVFile("b.wav", &format, &data, &size, &freq, &loop); 
  alBufferData(Unit->SndBuf[1], format, data, size, freq);
  alutUnloadWAV(format, data, size, freq); 

  alGenSources(1, &Unit->SndSrc);
  alSourcei(Unit->SndSrc, AL_BUFFER, Unit->SndBuf[0]);
  alSourcei(Unit->SndSrc, AL_LOOPING, 1);
} /* End of 'TestInit' function */

/* Функция деинициализации объекта.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       TEST *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID TestClose( TEST *Unit )
{
  VG4_GeomClose(&Unit->G[0]);
  VG4_GeomClose(&Unit->G[1]);
} /* End of 'TestClose' function */

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

  Unit->Init = (vg4UNIT_INIT)TestInit;
  Unit->Close = (vg4UNIT_CLOSE)TestClose;
  Unit->Render = (vg4UNIT_RENDER)TestRender;
  Unit->Response = (vg4UNIT_RESPONSE)TestResponse;
  Unit->X = X;
  Unit->Y = Y;
  Unit->Z = Z;
  Unit->No = ++CurrentNo;
  /*
  VG4_PrimDefaultColor = VecSet(1, 0, 0);
  VG4_PrimCreateSphere(&p, 300, 300, VecSet(0, 0, 0), 1);
  VG4_GeomAddPrim(&Unit->G, &p);
  VG4_PrimDefaultColor = VecSet(0, 1, 0);
  VG4_PrimCreatePlane(&p, 300, 300, VecSet(-5, -1, 5), VecSet(10, 0, 0), VecSet(0, 0, -10));
  VG4_GeomAddPrim(&Unit->G, &p);
  */
  VG4_GeomLoad(&Unit->G[0], "Models/Cessna172/cessna172.object");
  /* VG4_GeomLoad(&Unit->G[1], "Models/Cessna172/cessna172.object"); */
  VG4_GeomLoad(&Unit->G[1], "Models/X6/x6.object");
  return (vg4UNIT *)Unit;
} /* End of 'TestUnitCreate' function */

/* END OF 'TESTUNIT.C' FILE */
