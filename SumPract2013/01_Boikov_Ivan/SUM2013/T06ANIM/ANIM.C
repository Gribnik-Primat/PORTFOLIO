 /* FILE NAME: ANIM.C
 * PROGRAMMER: IB1
 * DATE: 06.06.2013
 * PURPOSE: Main animation implementation module.
 */

#include <stdlib.h>
#include <string.h>


#include "anim.h"
#include "vec.h"

#pragma comment (lib, "winmm")
#include <mmsystem.h>
#include <winbase.h>

/* Глобальные переменные анимации */
static BOOL IB1_IsInit;
static IB1ANIM IB1_Anim;
static FrameCount = 0;

/* Массив активных объектов анимации */
#define IB1_MAX_UNITS 1000
static INT IB1_NumOfUnits;
static IB1UNIT *IB1_Units[IB1_MAX_UNITS];
INT64
  TimeFreq,  /* единиц измерения времени в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время предыдущего кадра */
  TimePause, /* время простоя в паузу */
  TimeShift, /* время сдвига */
  TimeFPS;   /* время для замера FPS - frame-per-second */
LONG FrameCounter;

MATR MatrWorld, MatrView, MatrProj;;
DBL PD, Wp, Hp, Dx = 0, Dy = 0, Dz = 0;

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - рабочее окно анимации:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE при невозможности инициализации.
 */
BOOL IB1_AnimInit( HWND hWnd )
{
  INT i;
  HDC hDC;
  LARGE_INTEGER li;

  IB1_Anim.hWnd = hWnd;
  /* определяем контект окна */
  hDC = GetDC(hWnd);
  /* создаем контекст в памяти, совместимый с контекстом окна */
  IB1_Anim.hDC  = CreateCompatibleDC(hDC);
  /* убираем контекст окна */
  ReleaseDC(hWnd, hDC);

  IB1_IsInit = TRUE;

  /* инициализация таймера */
                                     
  
  QueryPerformanceFrequency(&li);
  TimeFreq  = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = TimeShift = 0;
  FrameCount = 0;

  /* Инициализация всех зарегистрированных объектов анимации */
  for (i = 0; i < IB1_NumOfUnits; i++)
    IB1_Units[i]->Init(IB1_Units[i], &IB1_Anim);

  return TRUE;
} /* End of 'IB1_AnimInit' function */

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID IB1_AnimClose( VOID )
{
  INT i;

  if (IB1_Anim.hDC != NULL)
    DeleteDC(IB1_Anim.hDC), IB1_Anim.hDC = NULL;
  if (IB1_Anim.hFrame != NULL)
    DeleteObject(IB1_Anim.hFrame), IB1_Anim.hFrame = NULL;

  /* Уничтожение всех зарегистрированных объектов анимации */
  for (i = 0; i < IB1_NumOfUnits; i++)
    IB1_Units[i]->Close(IB1_Units[i]);

  IB1_IsInit = FALSE;
} /* End of 'IB1_AnimClose' function */

/* Функция обработки изменение размера кадра.
 * АРГУМЕНТЫ:
 *   - новые размеры кадра:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID IB1_AnimResize( INT W, INT H )
{
  BITMAPINFOHEADER bih;

  if (!IB1_IsInit)
    return;

  if (IB1_Anim.hFrame != NULL)
    DeleteObject(IB1_Anim.hFrame);

  /* создаем изображение заданной структуры */
  bih.biSize = sizeof(BITMAPINFOHEADER);
  bih.biWidth = IB1_Anim.W = W;
  bih.biHeight = -(IB1_Anim.H = H);
  bih.biPlanes = 1;    /* всегда */
  bih.biBitCount = 32; /* xRGB */
  bih.biCompression = BI_RGB;
  bih.biSizeImage = W * H * 4;
  bih.biClrImportant = 0;
  bih.biClrUsed = 0;
  bih.biXPelsPerMeter = bih.biYPelsPerMeter = 0;

  IB1_Anim.hFrame = CreateDIBSection(NULL, (BITMAPINFO *)&bih,
    DIB_RGB_COLORS, (VOID *)&IB1_Anim.Bits, NULL, 0);

  SelectObject(IB1_Anim.hDC, IB1_Anim.hFrame);

  /* Первичное построение кадра в новом размере */
  IB1_AnimRender();
} /* End of 'IB1_AnimResize' function */

/* Функция построения кадра.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID IB1_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  CHAR Buf[100];

  if (!IB1_IsInit)
    return;

  /* Опрос таймера */
  /* обновляем информацию: */
  QueryPerformanceCounter(&li);

  /* глобальное время */
  IB1_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  IB1_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* локальное время */
  if (IB1_Anim.IsPause)
    IB1_Anim.DeltaTime = 0, TimePause += li.QuadPart - TimeOld;
  else
    IB1_Anim.DeltaTime = IB1_Anim.GlobalDeltaTime;

  IB1_Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause - TimeShift) /
    TimeFreq;

  /* вычисляем производительность */
  if (TimeFPS > TimeFreq)
  {
    IB1_Anim.FPS = FrameCount / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
  }
  /* обновляем время "старого" кадра */
  TimeOld = li.QuadPart;

  //TextOut(IB1_Anim.hDC, 0, 0, Buf, sprintf("%lf", IB1_Anim.FPS));

  /* Опрос устройств ввода */
  if ((i = joyGetNumDevs()) > 1)
    {
      JOYCAPS jc;

      if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
      {
        JOYINFOEX ji;

        ji.dwSize = sizeof(ji);
        ji.dwFlags = JOY_RETURNALL;

        if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
        {
          /* кнопки */
          memcpy(IB1_Anim.JButOld, IB1_Anim.JBut, 32);
          for (i = 0; i < 32; i++)
            IB1_Anim.JBut[i] = (ji.dwButtons >> i) & 1;

          /* оси переводим в диапазон -1..1 */
          IB1_Anim.Jx = 2.0 * (ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin) - 1;
          IB1_Anim.Jy = 2.0 * (ji.dwYpos - jc.wYmin) / (jc.wYmax - jc.wYmin) - 1;
          IB1_Anim.Jz = 2.0 * (ji.dwZpos - jc.wZmin) / (jc.wZmax - jc.wZmin) - 1;
          IB1_Anim.Jr = 2.0 * (ji.dwRpos - jc.wRmin) / (jc.wRmax - jc.wRmin) - 1;

          if (ji.dwPOV == 0xFFFF)
            IB1_Anim.JPov = 0;
          else
            IB1_Anim.JPov = ji.dwPOV / 4500 + 1;
        }
      }
    }

  /* Посылка всем объектам анимации сигнала обработки
   * межкадрового взаимодействия */
  for (i = 0; i < IB1_NumOfUnits; i++)
    IB1_Units[i]->Response(IB1_Units[i], &IB1_Anim);

  /* Инициализация кадра */
  SelectObject(IB1_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(IB1_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(IB1_Anim.hDC, RGB(0, 0, 0));
  Rectangle(IB1_Anim.hDC, 0, 0, IB1_Anim.W, IB1_Anim.H);

  /* Посылка всем объектам анимации сигнала перерисовки */
  for (i = 0; i < IB1_NumOfUnits; i++)
  {
    SelectObject(IB1_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(IB1_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(IB1_Anim.hDC, RGB(0, 0, 0));
    SetDCPenColor(IB1_Anim.hDC, RGB(100, 0, 255));
    IB1_Units[i]->Render(IB1_Units[i], &IB1_Anim);
  }
  Dx += IB1_Anim.Jx;
  Dy += IB1_Anim.Jy;
  Dz += IB1_Anim.Jz;
} /* End of 'IB1_AnimRender' function */

/* Функция копирования рабочего кадра в окно.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID IB1_AnimCopyFrame( VOID )
{
  HDC hDC;

  if (!IB1_IsInit)
    return;
  /* Выводм кадр в окно анимации */
  hDC = GetDC(IB1_Anim.hWnd);
  BitBlt(hDC, 0, 0, IB1_Anim.W, IB1_Anim.H, IB1_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(IB1_Anim.hWnd, hDC);
} /* End of 'IB1_AnimCopyFrame' function */

/***
 * Функции объектов обработки по-умолчанию
 ***/

/* Функция инициализации объекта.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       IB1UNIT *Unit;
 *   - указатель на систему анимации:
 *       IB1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID IB1_DefUnitInit( IB1UNIT *Unit, IB1ANIM *Ani )
{
} /* End of 'IB1_DefUnitInit' function */

/* Функция деинициализации объекта.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       IB1UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID IB1_DefUnitClose( IB1UNIT *Unit )
{
} /* End of 'IB1_DefUnitClose' function */

/* Функция реакции на межкадровое взаимодействие объекта.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       IB1UNIT *Unit;
 *   - указатель на систему анимации:
 *       IB1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID IB1_DefUnitResponse( IB1UNIT *Unit, IB1ANIM *Ani )
{
} /* End of 'IB1_DefUnitResponse' function */

/* Функция отрисовки объекта.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       IB1UNIT *Unit;
 *   - указатель на систему анимации:
 *       IB1ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID IB1_DefUnitRender( IB1UNIT *Unit, IB1ANIM *Ani )
{
} /* End of 'IB1_DefUnitRender' function */

/* Функция сроздания нового объекта-единицы анимации.
 * АРГУМЕНТЫ:
 *   - размер нового объекта в байтах:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (IB1UNIT *) указатель на созданный объект;
 */
IB1UNIT * IB1_UnitCreate( INT Size )
{
  IB1UNIT *Unit;

  if (Size < sizeof(IB1UNIT) || (Unit = malloc(Size)) == NULL)
    return NULL;

  memset(Unit, 0, Size);
  Unit->Init = IB1_DefUnitInit;
  Unit->Close = IB1_DefUnitClose;
  Unit->Response = IB1_DefUnitResponse;
  Unit->Render = IB1_DefUnitRender;
  return Unit;
} /* End of 'IB1_UnitCreate' function */

/* Функция добавления объекта анимации к системе.
 * АРГУМЕНТЫ:
 *   - указатель на объект:
 *       IB1UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID IB1_AnimAdd( IB1UNIT *Unit )
{
  if (IB1_NumOfUnits >= IB1_MAX_UNITS)
    return;
  IB1_Units[IB1_NumOfUnits++] = Unit;
} /* End of 'IB1_AnimAdd' function */




POINT WorldToScreen (VEC P)
{
  VEC P1, P2, P3, P4;
  POINT pt;
                 
  //MatrWorld = MatrMulMatr(MatrMulMatr(MatrRotateX(IB1_Anim.Jx), MatrRotateY(IB1_Anim.Jy)), MatrRotateZ(IB1_Anim.Jz));
  //MatrView = MatrUnit();
  MatrWorld = MatrUnit();
  MatrView = MatrTranslate(Dx * 10, Dy * 10, Dz * 100); 
  //MatrWorld = MatrTranslate(IB1_Anim.Time, Dy, Dz);
  MatrProj = MatrProject(-100, 100, -100, 100, 1000, 10000);

  Wp = 1.0 * IB1_Anim.W / IB1_Anim.H;
  Hp = 1.0 * IB1_Anim.H / IB1_Anim.W;
  PD = 1.0;

  P1 = VecMulMatr(P, MatrWorld); 
  P2 = VecMulMatr(P1, MatrView);
  P3 = VecMulMatr(P2, MatrProj);

  P4.X = P3.X * PD / P3.Z;
  P4.Y = P3.Y * PD / P3.Z;


  P4.X *= 2 / Wp;
  P4.Y *= 2 / Hp;

  pt.x = ((P4.X + 1) / 2) * (IB1_Anim.W - 1);
  pt.y = ((-P4.Y + 1) / 2) * (IB1_Anim.H - 1);

  return pt;
}

/* END OF 'ANIM.C' FILE */