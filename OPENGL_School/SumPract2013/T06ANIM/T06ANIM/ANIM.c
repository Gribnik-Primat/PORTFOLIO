/* FILE NAME: ANIM.C
* PROGRAMMER: VG4
* DATE: 06.06.2013
* PURPOSE: Main animation implementation module.
*/

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "anim.h"
#include "vec.h"

/* Глобальные переменные анимации */
static BOOL VG4_IsInit;
static vg4ANIM Anim;

/* Массив активных объектов анимации */
#define VG4_MAX_UNITS 1000
static INT VG4_NumOfUnits;
static vg4UNIT *VG4_Units[VG4_MAX_UNITS];

/* Функция инициализации анимации.
* АРГУМЕНТЫ:
*   - рабочее окно анимации:
*       HWND hWnd;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (BOOL) TRUE при успехе, FALSE при невозможности инициализации.
*/
BOOL AnimInit( HWND hWnd )
{
  HDC hDC;
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};
  LARGE_INTEGER li;

  Anim.hWnd = hWnd;
  Anim.hDC = GetDC(hWnd);

  /*** инициализация OpenGL ***/
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 0;
  pfd.cStencilBits = 0;

  i = ChoosePixelFormat(Anim.hDC, &pfd);
  DescribePixelFormat(Anim.hDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(Anim.hDC, i, &pfd);

  Anim.hGLRC = wglCreateContext(Anim.hDC);
  wglMakeCurrent(Anim.hDC, Anim.hGLRC);

  /* установка базовых параметров OpenGL */
  if (glewInit() != GLEW_OK)
  {
    ReleaseDC(Anim.hWnd, Anim.hDC);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(Anim.hGLRC);
    memset(&Anim, 0, sizeof(Anim));
    return FALSE;
  }

  glClearColor(0.3, 0.5, 0.7, 1);

  QueryPerformanceFrequency (&li); 
  TimeFreq  = li.QuadPart;
  QueryPerformanceCounter(&li); 
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = TimeShift = 0;
  FrameCounter = 0;

  Anim.hWnd = hWnd;
  /* определяем контект окна */
  hDC = GetDC(hWnd);
  /* создаем контекст в памяти, совместимый с контекстом окна */
  Anim.hDC  = CreateCompatibleDC(hDC);
  /* убираем контекст окна */
  ReleaseDC(hWnd, hDC);

  VG4_IsInit = TRUE;

  /* Инициализация всех зарегистрированных объектов анимации */
  for (i = 0; i < VG4_NumOfUnits; i++)
    VG4_Units[i]->Init(VG4_Units[i], &Anim);

  return TRUE;
} /* End of 'AnimInit' function */

/* Функция деинициализации анимации.
* АРГУМЕНТЫ: Нет.
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID AnimClose( VOID )
{
  INT i;

  ReleaseDC(Anim.hWnd, Anim.hDC);
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(Anim.hGLRC);

  /* Уничтожение всех зарегистрированных объектов анимации */
  for (i = 0; i < VG4_NumOfUnits; i++)
    VG4_Units[i]->Close(VG4_Units[i]);

  VG4_IsInit = FALSE;
} /* End of 'AnimClose' function */

/* Функция обработки изменение размера кадра.
* АРГУМЕНТЫ:
*   - новые размеры кадра:
*       INT W, H;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID AnimResize( INT W, INT H )
{
 

  if (!VG4_IsInit)
    return;
   
  glViewport(0, 0, W, H);

  /* Первичное построение кадра в новом размере */
  AnimRender();
} /* End of 'AnimResize' function */

/* Функция построения кадра.
* АРГУМЕНТЫ: Нет.
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID AnimRender( VOID )
{
  INT i;
  static CHAR Buf[100];

  /* Опрос таймера */

  /* Опрос устройств ввода */
  LARGE_INTEGER li;

  QueryPerformanceCounter(&li);

  /* глобальное время */
  Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* локальное время */
  if (Anim.IsPause)
    Anim.DeltaTime = 0, TimePause += li.QuadPart - TimeOld;
  else
    Anim.DeltaTime = Anim.GlobalDeltaTime;

  Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause - TimeShift) /
    TimeFreq;

  /* вычисляем производительность */
  if (TimeFPS > TimeFreq)
  {
    Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }
  /* обновляем время "старого" кадра */
  TimeOld = li.QuadPart;
  
  /* Инициализация кадра */
  glClear(GL_COLOR_BUFFER_BIT);

  /* Посылка всем объектам анимации сигнала перерисовки */
  for (i = 0; i < VG4_NumOfUnits; i++)
  {
    VG4_Units[i]->Render(VG4_Units[i], &Anim);
  }
  /* Завершение кадра */
  glFinish();

  /* Посылка всем объектам анимации сигнала обработки
  /* Инициализация кадра */

  if (!VG4_IsInit)
    return;

  /* Посылка всем объектам анимации сигнала перерисовки */

  

} /* End of 'AnimRender' function */

/* Функция копирования рабочего кадра в окно.
* АРГУМЕНТЫ: Нет.
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID AnimCopyFrame( VOID )
{


  if (!VG4_IsInit)
    return;
  /* Выводм кадр в окно анимации */
  SwapBuffers(Anim.hDC);
} /* End of 'AnimCopyFrame' function */

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
VOID AnimAdd( vg4UNIT *Unit )
{
  if (VG4_NumOfUnits >= VG4_MAX_UNITS)
    return;
  VG4_Units[VG4_NumOfUnits++] = Unit;
} /* End of 'AnimAdd' function */

/* END OF 'ANIM.C' FILE */
