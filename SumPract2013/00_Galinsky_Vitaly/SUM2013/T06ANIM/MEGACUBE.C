/* FILE NAME: MEGACUBE.C
 * PROGRAMMER: VG4
 * DATE: 07.06.2013
 * PURPOSE: Animation sample module.
 */

#include <stdio.h>
#include <stdlib.h>
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
} CUBE;

static INT CurrentNo = 0; /* очередной порядковый номер */

/* Функция отрисовки куба.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       CUBE *Unit;
 *   - указатель на систему анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID CubeRender( CUBE *Unit, vg4ANIM *Ani )
{
  DBL t = (DBL)clock() / CLOCKS_PER_SEC;
  INT
    a = 30 * sin(t * Unit->RandScale + Unit->RandShift),
    b = 30 * cos(t * Unit->RandScale + Unit->RandShift);
  static CHAR Buf[10000];
  static INT mx, my, mw;

  switch (Unit->Who)
  {
  case 0:
    Rectangle(Ani->hDC, Unit->X + a, Unit->Y + b, Unit->X + a + 30, Unit->Y + b + 30);
    break;
  case 1:
    Ellipse(Ani->hDC, Unit->X + a, Unit->Y + b, Unit->X + a + 30, Unit->Y + b + 30);
    break;
  default:
    RoundRect(Ani->hDC, Unit->X + a, Unit->Y + b, Unit->X + a + 30, Unit->Y + b + 30, 8, 8);
    break;
  }
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
          "FPS: %10.3f Time: %11.4f (%11.4f) %s\n"
          "Mouse:[%i %i] b:%i%i%i [dx % dy %i dw %i]\n"
          "Keyboard:%s%s%s%s%s%s\n"
          "Joy: %10.4f %10.4f %10.4f %10.4f Pov:%d b:",
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

    SetBkMode(Ani->hDC, TRANSPARENT);
    SetTextColor(Ani->hDC, RGB(8, 8, 8));
    SetRect(&rc, 2, 2, Ani->W, Ani->H);
    DrawText(Ani->hDC, Buf, l, &rc, DT_LEFT | DT_TOP);
    SetTextColor(Ani->hDC, RGB(255, 255, 255));
    SetRect(&rc, 0, 0, Ani->W, Ani->H);
    DrawText(Ani->hDC, Buf, l, &rc, DT_LEFT | DT_TOP);
  }
} /* End of 'CubeRender' function */

/* Функция сроздания нового куба.
 * АРГУМЕНТЫ:
 *   - позиция на экране:
 *       INT X, Y;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (vg4UNIT *) указатель на созданный объект;
 */
vg4UNIT * CubeCreate( INT X, INT Y )
{
  CUBE *Unit;

  if ((Unit = (CUBE *)VG4_UnitCreate(sizeof(CUBE))) == NULL)
    return NULL;

  Unit->Render = (vg4UNIT_RENDER)CubeRender;
  Unit->X = X;
  Unit->Y = Y;
  Unit->Who = rand() % 3;
  Unit->RandShift = rand() % 1000;
  Unit->RandScale = 0.75 + 5.5 * rand() / (DBL)RAND_MAX;
  Unit->No = ++CurrentNo;
  return (vg4UNIT *)Unit;
} /* End of 'CubeCreate' function */

/* END OF 'MEGACUBE.C' FILE */
