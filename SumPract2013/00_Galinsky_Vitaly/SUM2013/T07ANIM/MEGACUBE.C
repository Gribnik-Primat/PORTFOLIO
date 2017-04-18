/* FILE NAME: MEGACUBE.C
 * PROGRAMMER: VG4
 * DATE: 10.06.2013
 * PURPOSE: Animation sample module.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "anim.h"

/***
 * Функции обработки мегакуба
 ***/

/* Тип представления мегакуба */
typedef struct
{
  /* Включаем базовые поля объекта анимации */
  VG4_UNIT_BASE_FUNCS;
  INT X, Y; /* Позиция куба  */
  INT RandShift; /* Случайный сдвиг */
  DBL RandScale; /* Случайный масштаб */
  INT Who;       /* Тип объекта */
  INT No;        /* Порядковый номер объекта */
  vg4GOBJ Land;  /* Примитив - земля */
} COW;

static INT CurrentNo = 0; /* очередной порядковый номер */

/* Функция отрисовки куба.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       COW *Unit;
 *   - указатель на систему анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeRender( COW *Unit, vg4ANIM *Ani )
{
  DBL t = (DBL)clock() / CLOCKS_PER_SEC;
  INT
    a = 30 * sin(t * Unit->RandScale + Unit->RandShift),
    b = 30 * cos(t * Unit->RandScale + Unit->RandShift);
  static CHAR Buf[10000];
  static INT mx, my, mw;

  /* Вывод у первого объекта анимационной информации на экран */
  if (Unit->No == CurrentNo)
  {
    INT i, l;
    RECT rc;

    /* для проверки относительных перемещений */
    mx += Ani->MouseDeltaX;
    my += Ani->MouseDeltaY;
    mw += Ani->MouseWheel;

    l = sprintf(Buf,
          "FPS:%8.3f T:%.4f (%11.4f) %s|"
          "Ms:[%i %i]:%i%i%i [d %i %i w %i]|"
          "Kb:%s%s%s%s%s%s|"
          "J:%7.3f %7.3f %7.3f %7.3f Pov:%d b:",
          Ani->FPS, Ani->Time, Ani->GlobalTime,
          Ani->IsPause ? "Paused" : "",
          Ani->MouseX, Ani->MouseY,
          Ani->MouseBut[0], Ani->MouseBut[1], Ani->MouseBut[2],
          mx, my, mw,
          Ani->Keys[VK_ESCAPE] ? " Escape" : "",
          Ani->Keys[VK_SPACE] ? " Space" : "",
          Ani->Keys[VK_UP] ? " Up" : "",
          Ani->Keys[VK_DOWN] ? " Down" : "",
          Ani->Keys[VK_LEFT] ? " Left" : "",
          Ani->Keys[VK_RIGHT] ? " Right" : "",
          Ani->Jx, Ani->Jy, Ani->Jz, Ani->Jr, Ani->JPov);
    for (i = 0; i < 32; i++)
      l += sprintf(Buf + l, "%d", Ani->JBut[i]);

    SetWindowText(Ani->hWnd, Buf);
    /*
    SetBkMode(Ani->hDC, TRANSPARENT);
    SetTextColor(Ani->hDC, RGB(8, 8, 8));
    SetRect(&rc, 2, 2, Ani->W, Ani->H);
    DrawText(Ani->hDC, Buf, l, &rc, DT_LEFT | DT_TOP);
    SetTextColor(Ani->hDC, RGB(255, 255, 255));
    SetRect(&rc, 0, 0, Ani->W, Ani->H);
    DrawText(Ani->hDC, Buf, l, &rc, DT_LEFT | DT_TOP);
    */
  }
  if (Ani->Keys['P'] && !Ani->KeysOld['P'])
    Ani->IsPause = !Ani->IsPause;

  glPushAttrib(GL_ALL_ATTRIB_BITS);
  if (Ani->Keys['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  if (Ani->Keys['F'])
    VG4_SetFullScreen(TRUE);
  if (Ani->Keys['G'])
    VG4_SetFullScreen(FALSE);

  VG4_GeomDraw(&Unit->Land);
  glPopAttrib();
} /* End of 'CubeRender' function */

/* Функция деинициализации куба.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       COW *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeClose( COW *Unit )
{
  VG4_GeomClose(&Unit->Land);
} /* End of 'CubeClose' function */

/* Функция сроздания нового куба.
 * АРГУМЕНТЫ:
 *   - позиция на экране:
 *       INT X, Y;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (vg4UNIT *) указатель на созданный объект;
 */
vg4UNIT * CubeCreate( INT X, INT Y )
{
  COW *Unit;
  vg4PRIM p;
  vg4MATERIAL mat;

  if ((Unit = (COW *)VG4_UnitCreate(sizeof(COW))) == NULL)
    return NULL;

  Unit->Render = (vg4UNIT_RENDER)CubeRender;
  Unit->Close = (vg4UNIT_CLOSE)CubeClose;
  Unit->X = X;
  Unit->Y = Y;
  Unit->Who = rand() % 3;
  Unit->RandShift = rand() % 1000;
  Unit->RandScale = 0.75 + 5.5 * rand() / (DBL)RAND_MAX;
  Unit->No = ++CurrentNo;

  /*
  VG4_PrimDefaultColor = VecSet(0.5, 1, 0.5);
  VG4_PrimCreatePlane(&p, 100, 100, VecSet(-5, -1, 5), VecSet(10, 0, 0), VecSet(0, 0, -10));
  VG4_GeomAddPrim(&Unit->Land, &p);
  */
  //VG4_PrimCreateHeightField(&p, "hf.bmp", VecSet(-100, -30, 100), VecSet(200, 0, 0), VecSet(0, 0, -200));
  VG4_PrimCreatePlane(&p, 10, 10, VecSet(-100, -30, 100), VecSet(200, 0, 0), VecSet(0, 0, -200));
  memset(&mat, 0, sizeof(mat));
  mat.Ka = VecSet(0.5, 0.5, 0.5);
  mat.Kd = VecSet(1.8, 1.8, 1.8);
  mat.Ks = VecSet(0.8, 0.8, 0.8);
  mat.Phong = 10;
  mat.Trans = 0;
  strcpy(mat.MapD, "");
  strcpy(mat.Tex2, "norm_map.bmp");
  VG4_GeomAddMat(&Unit->Land, &mat);
  p.Mat = 0;

  VG4_GeomAddPrim(&Unit->Land, &p);

  return (vg4UNIT *)Unit;
} /* End of 'CubeCreate' function */

/* END OF 'MEGACUBE.C' FILE */
