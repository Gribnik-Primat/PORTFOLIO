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

/* ���������� ���������� �������� */
static BOOL VG4_IsInit;
static vg4ANIM Anim;

/* ������ �������� �������� �������� */
#define VG4_MAX_UNITS 1000
static INT VG4_NumOfUnits;
static vg4UNIT *VG4_Units[VG4_MAX_UNITS];

/* ������� ������������� ��������.
* ���������:
*   - ������� ���� ��������:
*       HWND hWnd;
* ������������ ��������:
*   (BOOL) TRUE ��� ������, FALSE ��� ������������� �������������.
*/
BOOL AnimInit( HWND hWnd )
{
  HDC hDC;
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};
  LARGE_INTEGER li;

  Anim.hWnd = hWnd;
  Anim.hDC = GetDC(hWnd);

  /*** ������������� OpenGL ***/
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

  /* ��������� ������� ���������� OpenGL */
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
  /* ���������� ������� ���� */
  hDC = GetDC(hWnd);
  /* ������� �������� � ������, ����������� � ���������� ���� */
  Anim.hDC  = CreateCompatibleDC(hDC);
  /* ������� �������� ���� */
  ReleaseDC(hWnd, hDC);

  VG4_IsInit = TRUE;

  /* ������������� ���� ������������������ �������� �������� */
  for (i = 0; i < VG4_NumOfUnits; i++)
    VG4_Units[i]->Init(VG4_Units[i], &Anim);

  return TRUE;
} /* End of 'AnimInit' function */

/* ������� ��������������� ��������.
* ���������: ���.
* ������������ ��������: ���.
*/
VOID AnimClose( VOID )
{
  INT i;

  ReleaseDC(Anim.hWnd, Anim.hDC);
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(Anim.hGLRC);

  /* ����������� ���� ������������������ �������� �������� */
  for (i = 0; i < VG4_NumOfUnits; i++)
    VG4_Units[i]->Close(VG4_Units[i]);

  VG4_IsInit = FALSE;
} /* End of 'AnimClose' function */

/* ������� ��������� ��������� ������� �����.
* ���������:
*   - ����� ������� �����:
*       INT W, H;
* ������������ ��������: ���.
*/
VOID AnimResize( INT W, INT H )
{
 

  if (!VG4_IsInit)
    return;
   
  glViewport(0, 0, W, H);

  /* ��������� ���������� ����� � ����� ������� */
  AnimRender();
} /* End of 'AnimResize' function */

/* ������� ���������� �����.
* ���������: ���.
* ������������ ��������: ���.
*/
VOID AnimRender( VOID )
{
  INT i;
  static CHAR Buf[100];

  /* ����� ������� */

  /* ����� ��������� ����� */
  LARGE_INTEGER li;

  QueryPerformanceCounter(&li);

  /* ���������� ����� */
  Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* ��������� ����� */
  if (Anim.IsPause)
    Anim.DeltaTime = 0, TimePause += li.QuadPart - TimeOld;
  else
    Anim.DeltaTime = Anim.GlobalDeltaTime;

  Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause - TimeShift) /
    TimeFreq;

  /* ��������� ������������������ */
  if (TimeFPS > TimeFreq)
  {
    Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }
  /* ��������� ����� "�������" ����� */
  TimeOld = li.QuadPart;
  
  /* ������������� ����� */
  glClear(GL_COLOR_BUFFER_BIT);

  /* ������� ���� �������� �������� ������� ����������� */
  for (i = 0; i < VG4_NumOfUnits; i++)
  {
    VG4_Units[i]->Render(VG4_Units[i], &Anim);
  }
  /* ���������� ����� */
  glFinish();

  /* ������� ���� �������� �������� ������� ���������
  /* ������������� ����� */

  if (!VG4_IsInit)
    return;

  /* ������� ���� �������� �������� ������� ����������� */

  

} /* End of 'AnimRender' function */

/* ������� ����������� �������� ����� � ����.
* ���������: ���.
* ������������ ��������: ���.
*/
VOID AnimCopyFrame( VOID )
{


  if (!VG4_IsInit)
    return;
  /* ������ ���� � ���� �������� */
  SwapBuffers(Anim.hDC);
} /* End of 'AnimCopyFrame' function */

/***
* ������� �������� ��������� ��-���������
***/

/* ������� ������������� �������.
* ���������:
*   - ��������� �� "����":
*       vg4UNIT *Unit;
*   - ��������� �� ������� ��������:
*       vg4ANIM *Ani;
* ������������ ��������: ���.
*/
static VOID VG4_DefUnitInit( vg4UNIT *Unit, vg4ANIM *Ani )
{
} /* End of 'VG4_DefUnitInit' function */

/* ������� ��������������� �������.
* ���������:
*   - ��������� �� "����":
*       vg4UNIT *Unit;
* ������������ ��������: ���.
*/
static VOID VG4_DefUnitClose( vg4UNIT *Unit )
{
} /* End of 'VG4_DefUnitClose' function */

/* ������� ������� �� ����������� �������������� �������.
* ���������:
*   - ��������� �� "����":
*       vg4UNIT *Unit;
*   - ��������� �� ������� ��������:
*       vg4ANIM *Ani;
* ������������ ��������: ���.
*/
static VOID VG4_DefUnitResponse( vg4UNIT *Unit, vg4ANIM *Ani )
{
} /* End of 'VG4_DefUnitResponse' function */

/* ������� ��������� �������.
* ���������:
*   - ��������� �� "����":
*       vg4UNIT *Unit;
*   - ��������� �� ������� ��������:
*       vg4ANIM *Ani;
* ������������ ��������: ���.
*/
static VOID VG4_DefUnitRender( vg4UNIT *Unit, vg4ANIM *Ani )
{
} /* End of 'VG4_DefUnitRender' function */

/* ������� ��������� ������ �������-������� ��������.
* ���������:
*   - ������ ������ ������� � ������:
*       INT Size;
* ������������ ��������:
*   (vg4UNIT *) ��������� �� ��������� ������;
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

/* ������� ���������� ������� �������� � �������.
* ���������:
*   - ��������� �� ������:
*       vg4UNIT *Unit;
* ������������ ��������: ���.
*/
VOID AnimAdd( vg4UNIT *Unit )
{
  if (VG4_NumOfUnits >= VG4_MAX_UNITS)
    return;
  VG4_Units[VG4_NumOfUnits++] = Unit;
} /* End of 'AnimAdd' function */

/* END OF 'ANIM.C' FILE */
