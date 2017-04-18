/* FILE NAME: ANIM.C
 * PROGRAMMER: VG4
 * DATE: 07.06.2013
 * PURPOSE: Main animation implementation module.
 */

#include <stdlib.h>
#include <string.h>

#include "anim.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm")

/* Глобальные переменные анимации */
static BOOL VG4_IsInit;
static vg4ANIM VG4_Anim;

/* Массив активных объектов анимации */
#define VG4_MAX_UNITS 1000
static INT VG4_NumOfUnits;
static vg4UNIT *VG4_Units[VG4_MAX_UNITS];

/* Переменные обслуживания времени */
static INT64
  VG4_TimeFreq,  /* единица измерения времени в секунду */
  VG4_TimeStart, /* время начала анимации */
  VG4_TimeOld,   /* время предыдущего кадра */
  VG4_TimePause, /* время простоя в паузу */
  VG4_TimeShift, /* время сдвига */
  VG4_TimeFPS;   /* время для замера FPS - frame-per-second */
static
  LONG VG4_FrameCounter; /* Счетчик кадров */

/* Старая позиция мыши */
static POINT VG4_MousePosOld;
/* Значение сдвига колеса мыши */
INT VG4_MouseWheel;

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - рабочее окно анимации:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE при невозможности инициализации.
 */
BOOL VG4_AnimInit( HWND hWnd )
{
  INT i;
  HDC hDC;
  LARGE_INTEGER li;

  VG4_Anim.hWnd = hWnd;
  /* определяем контект окна */
  hDC = GetDC(hWnd);
  /* создаем контекст в памяти, совместимый с контекстом окна */
  VG4_Anim.hDC  = CreateCompatibleDC(hDC);
  /* убираем контекст окна */
  ReleaseDC(hWnd, hDC);

  VG4_IsInit = TRUE;

  /* Инициализация всех зарегистрированных объектов анимации */
  for (i = 0; i < VG4_NumOfUnits; i++)
    VG4_Units[i]->Init(VG4_Units[i], &VG4_Anim);

  /* инициализация таймера */
  QueryPerformanceFrequency(&li); 
  VG4_TimeFreq  = li.QuadPart;
  QueryPerformanceCounter(&li); 
  VG4_TimeStart = VG4_TimeOld = VG4_TimeFPS = li.QuadPart;
  VG4_TimePause = VG4_TimeShift = 0;
  VG4_FrameCounter = 0;
  VG4_Anim.FPS = 30;

  VG4_Anim.MatrWorld = MatrUnit();
  VG4_Anim.MatrView = MatrUnit();
  VG4_Anim.PD = 1;
  VG4_Anim.Wp = 1;
  VG4_Anim.Hp = 1;


  return TRUE;
} /* End of 'VG4_AnimInit' function */

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimClose( VOID )
{
  INT i;

  if (VG4_Anim.hDC != NULL)
    DeleteDC(VG4_Anim.hDC), VG4_Anim.hDC = NULL;
  if (VG4_Anim.hFrame != NULL)
    DeleteObject(VG4_Anim.hFrame), VG4_Anim.hFrame = NULL;

  /* Уничтожение всех зарегистрированных объектов анимации */
  for (i = 0; i < VG4_NumOfUnits; i++)
    VG4_Units[i]->Close(VG4_Units[i]);

  VG4_IsInit = FALSE;
} /* End of 'VG4_AnimClose' function */

/* Функция обработки изменение размера кадра.
 * АРГУМЕНТЫ:
 *   - новые размеры кадра:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimResize( INT W, INT H )
{
  BITMAPINFOHEADER bih;

  if (!VG4_IsInit)
    return;

  if (VG4_Anim.hFrame != NULL)
    DeleteObject(VG4_Anim.hFrame);

  /* создаем изображение заданной структуры */
  bih.biSize = sizeof(BITMAPINFOHEADER);
  bih.biWidth = VG4_Anim.W = W;
  bih.biHeight = -(VG4_Anim.H = H);
  bih.biPlanes = 1;    /* всегда */
  bih.biBitCount = 32; /* xRGB */
  bih.biCompression = BI_RGB;
  bih.biSizeImage = W * H * 4;
  bih.biClrImportant = 0;
  bih.biClrUsed = 0;
  bih.biXPelsPerMeter = bih.biYPelsPerMeter = 0;

  VG4_Anim.hFrame = CreateDIBSection(NULL, (BITMAPINFO *)&bih,
    DIB_RGB_COLORS, (VOID *)&VG4_Anim.Bits, NULL, 0);

  SelectObject(VG4_Anim.hDC, VG4_Anim.hFrame);

  /* Первичное построение кадра в новом размере */
  VG4_AnimRender();
} /* End of 'VG4_AnimResize' function */

/* Функция построения кадра.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;

  if (!VG4_IsInit)
    return;

  /*** Опрос таймера ***/

  /* обновляем информацию: */
  QueryPerformanceCounter(&li);

  /* глобальное время */
  VG4_Anim.GlobalTime = (DBL)(li.QuadPart - VG4_TimeStart) / VG4_TimeFreq;
  VG4_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - VG4_TimeOld) / VG4_TimeFreq;

  /* локальное время */
  if (VG4_Anim.IsPause)
    VG4_Anim.DeltaTime = 0, VG4_TimePause += li.QuadPart - VG4_TimeOld;
  else
    VG4_Anim.DeltaTime = VG4_Anim.GlobalDeltaTime;

  VG4_Anim.Time = (DBL)(li.QuadPart - VG4_TimeStart - VG4_TimePause - VG4_TimeShift) /
    VG4_TimeFreq;

  /* вычисляем производительность */
  if (li.QuadPart - VG4_TimeFPS > VG4_TimeFreq * 3.0)
  {
    VG4_Anim.FPS = VG4_FrameCounter / ((DBL)(li.QuadPart - VG4_TimeFPS) / VG4_TimeFreq);
    VG4_TimeFPS = li.QuadPart;
    VG4_FrameCounter = 0;
  }
  /* обновляем время "старого" кадра */
  VG4_TimeOld = li.QuadPart;

  /*** Опрос устройств ввода ***/

  /* клавиатура */
  /* копируем старое состояние */
  memcpy(VG4_Anim.KeysOld, VG4_Anim.Keys, 256);
  /* Считываем новое */
  GetKeyboardState(VG4_Anim.Keys);
  /* конвертироем в Да,Нет */
  for (i = 0; i < 256; i++)
    VG4_Anim.Keys[i] >>= 7;

  /* мышь */
  GetCursorPos(&pt);
  ScreenToClient(VG4_Anim.hWnd, &pt);
  VG4_Anim.MouseX = pt.x;
  VG4_Anim.MouseY = pt.y;
  VG4_Anim.MouseBut[0] = VG4_Anim.Keys[VK_LBUTTON];
  VG4_Anim.MouseBut[1] = VG4_Anim.Keys[VK_RBUTTON];
  VG4_Anim.MouseBut[2] = VG4_Anim.Keys[VK_MBUTTON];
  VG4_Anim.MouseDeltaX = pt.x - VG4_MousePosOld.x;
  VG4_Anim.MouseDeltaY = pt.y - VG4_MousePosOld.y;
  VG4_Anim.MouseWheel = VG4_MouseWheel;
  VG4_MouseWheel = 0;
  VG4_MousePosOld = pt;

  /* джойстик */
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
        memcpy(VG4_Anim.JButOld, VG4_Anim.JBut, 32);
        for (i = 0; i < 32; i++)
          VG4_Anim.JBut[i] = (ji.dwButtons >> i) & 1;

        /* оси переводим в диапазон -1..1 */
        VG4_Anim.Jx = 2.0 * (ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin) - 1;
        VG4_Anim.Jy = 2.0 * (ji.dwYpos - jc.wYmin) / (jc.wYmax - jc.wYmin) - 1;
        VG4_Anim.Jz = 2.0 * (ji.dwZpos - jc.wZmin) / (jc.wZmax - jc.wZmin) - 1;
        VG4_Anim.Jr = 2.0 * (ji.dwRpos - jc.wRmin) / (jc.wRmax - jc.wRmin) - 1;

        if (ji.dwPOV == 0xFFFF)
          VG4_Anim.JPov = 0;
        else
          VG4_Anim.JPov = ji.dwPOV / 4500 + 1;
      }
    }
  }

  /* Посылка всем объектам анимации сигнала обработки
   * межкадрового взаимодействия */
  for (i = 0; i < VG4_NumOfUnits; i++)
    VG4_Units[i]->Response(VG4_Units[i], &VG4_Anim);

  /* Инициализация кадра */
  SelectObject(VG4_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(VG4_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(VG4_Anim.hDC, RGB(50, 150, 200));
  Rectangle(VG4_Anim.hDC, 0, 0, VG4_Anim.W, VG4_Anim.H);

  /* Посылка всем объектам анимации сигнала перерисовки */
  for (i = 0; i < VG4_NumOfUnits; i++)
  {
    SelectObject(VG4_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(VG4_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(VG4_Anim.hDC, RGB(0, 0, 0));
    SetDCPenColor(VG4_Anim.hDC, RGB(255, 255, 255));
    VG4_Units[i]->Render(VG4_Units[i], &VG4_Anim);
  }
} /* End of 'VG4_AnimRender' function */

/* Функция копирования рабочего кадра в окно.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimCopyFrame( VOID )
{
  HDC hDC;

  if (!VG4_IsInit)
    return;
  VG4_FrameCounter++;

  /* Выводим кадр в окно анимации */
  hDC = GetDC(VG4_Anim.hWnd);
  BitBlt(hDC, 0, 0, VG4_Anim.W, VG4_Anim.H, VG4_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(VG4_Anim.hWnd, hDC);
} /* End of 'VG4_AnimCopyFrame' function */

/***
 * Функции объектов обработки по-умолчанию
 ***/

/* Функция инициализации объекта.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       vg4UNIT *Unit;
 *   - указатель на систему анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID VG4_DefUnitInit( vg4UNIT *Unit, vg4ANIM *Ani )
{
} /* End of 'VG4_DefUnitInit' function */

/* Функция деинициализации объекта.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       vg4UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID VG4_DefUnitClose( vg4UNIT *Unit )
{
} /* End of 'VG4_DefUnitClose' function */

/* Функция реакции на межкадровое взаимодействие объекта.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       vg4UNIT *Unit;
 *   - указатель на систему анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID VG4_DefUnitResponse( vg4UNIT *Unit, vg4ANIM *Ani )
{
} /* End of 'VG4_DefUnitResponse' function */

/* Функция отрисовки объекта.
 * АРГУМЕНТЫ:
 *   - указатель на "себя":
 *       vg4UNIT *Unit;
 *   - указатель на систему анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID VG4_DefUnitRender( vg4UNIT *Unit, vg4ANIM *Ani )
{
} /* End of 'VG4_DefUnitRender' function */

/* Функция сроздания нового объекта-единицы анимации.
 * АРГУМЕНТЫ:
 *   - размер нового объекта в байтах:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (vg4UNIT *) указатель на созданный объект;
 */
vg4UNIT * VG4_UnitCreate( INT Size )
{
  vg4UNIT *Unit;

  if (Size < sizeof(vg4UNIT) || (Unit = malloc(Size)) == NULL)
    return NULL;

  memset(Unit, 0, Size);
  Unit->Init = VG4_DefUnitInit;
  Unit->Close = VG4_DefUnitClose;
  Unit->Response = VG4_DefUnitResponse;
  Unit->Render = VG4_DefUnitRender;
  return Unit;
} /* End of 'VG4_UnitCreate' function */

/* Функция добавления объекта анимации к системе.
 * АРГУМЕНТЫ:
 *   - указатель на объект:
 *       vg4UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimAdd( vg4UNIT *Unit )
{
  if (VG4_NumOfUnits >= VG4_MAX_UNITS)
    return;
  VG4_Units[VG4_NumOfUnits++] = Unit;
} /* End of 'VG4_AnimAdd' function */

/* Функция преобразования из мировой системы коорлинат в кадр.
 * АРГУМЕНТЫ:
 *   - исходная точка:
 *       VEC P;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (POINT) координаты в кадре.
 */
POINT VG4_AnimWorldToScreen( VEC P )
{
  POINT pt;
  VEC P1, P2, P3, P4;

  P1 = VecMulMatr(P, VG4_Anim.MatrWorld);
  P2 = VecMulMatr(P1, VG4_Anim.MatrView);
  P3 = VecMulMatr(P2, VG4_Anim.MatrProj);

  /* центральное проецирование */
  P4.X = P2.X * VG4_Anim.PD / -P2.Z;
  P4.Y = P2.Y * VG4_Anim.PD / -P2.Z;
  P4.Z = VG4_Anim.PD;

  /* отображение в кадр */
  P4.X *= 2 / VG4_Anim.Wp;  /* ужатие в [-1..1] */
  P4.Y *= 2 / VG4_Anim.Hp;

  if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
    P3 = P4;

  pt.x = (P3.X + 1) / 2 * (VG4_Anim.W - 1);
  pt.y = (-P3.Y + 1) / 2 * (VG4_Anim.H - 1);
  return pt;
} /* End of 'VG4_AnimWorldToScreen' function */

/* END OF 'ANIM.C' FILE */
