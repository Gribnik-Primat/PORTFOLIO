/* FILE NAME: ANIM.C
 * PROGRAMMER: IB1
 * DATE: 06.06.2013
 * PURPOSE: Main animation implementation module.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#include "anim.h"
#include "relief.h"

static BOOL IB1_IsInit;

INT64
  TimeFreq,  
  TimeStart, 
  TimeOld,   
  TimePause, 
  TimeShift, 
  TimeFPS;   
LONG FrameCounter;

static IB1ANIM IB1_Anim;
static LONG IB1_FrameCounter;

#define IB1_MAX_UNITS 10000
INT IB1_NumOfUnits;
static IB1UNIT *IB1_Units[IB1_MAX_UNITS];

BOOL IB1_AnimInit( HWND hWnd )
{
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};
  LARGE_INTEGER li;
  
  //LoadRawFile("Region0000.raw", MAP_SIZE * MAP_SIZE);
  //RenderHeightMap();

  IB1_Anim.alpha = 3.14 / 2;
  IB1_Anim.betha = 0;

  QueryPerformanceFrequency (&li); 
  TimeFreq  = li.QuadPart;
  QueryPerformanceCounter(&li); 
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = TimeShift = 0;
  FrameCount = 0;

  IB1_Anim.hWnd = hWnd;
  IB1_Anim.hDC = GetDC(hWnd);

  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 0;
  pfd.cStencilBits = 0;

  i = ChoosePixelFormat(IB1_Anim.hDC, &pfd);
  DescribePixelFormat(IB1_Anim.hDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(IB1_Anim.hDC, i, &pfd);

  IB1_Anim.hGLRC = wglCreateContext(IB1_Anim.hDC);
  wglMakeCurrent(IB1_Anim.hDC, IB1_Anim.hGLRC);

  if (glewInit() != GLEW_OK)
  {
    ReleaseDC(IB1_Anim.hWnd, IB1_Anim.hDC);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(IB1_Anim.hGLRC);
    memset(&IB1_Anim, 0, sizeof(IB1_Anim));
    return FALSE;
  }

  glClearColor(0.3, 0.5, 0.7, 1);

  IB1_IsInit = TRUE;
  IB1_Anim.dx = IB1_Anim.dy = IB1_Anim.dz = 0; 
  for (i = 0; i < IB1_NumOfUnits; i++)
    IB1_Units[i]->Init(IB1_Units[i], &IB1_Anim);

  return TRUE;
} /* End of 'IB1_AnimInit' function */

VOID IB1_AnimClose( VOID )
{
  INT i;

  ReleaseDC(IB1_Anim.hWnd, IB1_Anim.hDC);
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(IB1_Anim.hGLRC);

  for (i = 0; i < IB1_NumOfUnits; i++)
    IB1_Units[i]->Close(IB1_Units[i]);

  IB1_IsInit = FALSE;
} /* End of 'IB1_AnimClose' function */

VOID IB1_AnimResize( INT W, INT H )
{
  if (!IB1_IsInit)
    return;
  IB1_Anim.W = W;
  IB1_Anim.H = H;
  glViewport(0, 0, W, H);
  IB1_AnimRender();
} /* End of 'IB1_AnimResize' function */

VOID IB1_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  char Buf[100];

  if (!IB1_IsInit)
    return;

  /* Таймер */  
  QueryPerformanceCounter(&li);
  IB1_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  IB1_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;
  if (IB1_Anim.IsPause)
    IB1_Anim.DeltaTime = 0, TimePause += li.QuadPart - TimeOld;
  else
    IB1_Anim.DeltaTime = IB1_Anim.GlobalDeltaTime;

  IB1_Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause - TimeShift) /
    TimeFreq;

  if (TimeFPS > TimeFreq)
  {
    IB1_Anim.FPS = IB1_FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    IB1_FrameCounter = 0;
  }
  TimeOld = li.QuadPart;
    

  /* Опрос устройств */
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
        memcpy(IB1_Anim.JButOld, IB1_Anim.JBut, 32);
        for (i = 0; i < 32; i++)
          IB1_Anim.JBut[i] = (ji.dwButtons >> i) & 1;

        IB1_Anim.Jx = 2.0 * (ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin) - 1;
        IB1_Anim.Jy = 2.0 * (ji.dwYpos - jc.wYmin) / (jc.wYmax - jc.wYmin) - 1;
        IB1_Anim.Jz = 2.0 * (ji.dwZpos - jc.wZmin) / (jc.wZmax - jc.wZmin) - 1;
        IB1_Anim.Jr = 2.0 * (ji.dwRpos - jc.wRmin) / (jc.wRmax - jc.wRmin) - 1;

        if (ji.dwPOV == 0xFFFF)
          IB1_Anim.Jpov = 0;
        else
          IB1_Anim.Jpov = ji.dwPOV / 4500 + 1;
      }
    }
  }
  IB1_Anim.alpha += IB1_Anim.Jx / 500;
  if (IB1_Anim.betha + IB1_Anim.Jy / 100 <= 3.14 / 2 && IB1_Anim.betha + IB1_Anim.Jy / 100 >= -3.14 / 2)
    IB1_Anim.betha += IB1_Anim.Jy / 300;
  IB1_Anim.gamma += IB1_Anim.Jz / 50;

  sprintf(Buf, "-> Cessna 172 <-  FPS: %0.1lf Alpha: %0.3lf Betha: %0.3lf Gamma: %0.3lf", IB1_Anim.FPS, IB1_Anim.alpha, IB1_Anim.betha, IB1_Anim.gamma);
    SetWindowText(IB1_Anim.hWnd, Buf);
  for (i = 0; i < IB1_NumOfUnits; i++)
    IB1_Units[i]->Response(IB1_Units[i], &IB1_Anim);

  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);

  for (i = 0; i < IB1_NumOfUnits; i++)
    IB1_Units[i]->Render(IB1_Units[i], &IB1_Anim);
  glFinish();


} /* End of 'IB1_AnimRender' function */

VOID IB1_AnimCopyFrame( VOID )
{
  if (!IB1_IsInit)
    return;  
  SwapBuffers(IB1_Anim.hDC);
  IB1_FrameCounter++;
} /* End of 'IB1_AnimCopyFrame' function */

static VOID IB1_DefUnitInit( IB1UNIT *Unit, IB1ANIM *Ani )
{
} /* End of 'IB1_DefUnitInit' function */

static VOID IB1_DefUnitClose( IB1UNIT *Unit )
{
} /* End of 'IB1_DefUnitClose' function */

static VOID IB1_DefUnitResponse( IB1UNIT *Unit, IB1ANIM *Ani )
{
} /* End of 'IB1_DefUnitResponse' function */

static VOID IB1_DefUnitRender( IB1UNIT *Unit, IB1ANIM *Ani )
{
} /* End of 'IB1_DefUnitRender' function */

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

VOID IB1_AnimAdd( IB1UNIT *Unit )
{
  if (IB1_NumOfUnits >= IB1_MAX_UNITS)
    return;
  IB1_Units[IB1_NumOfUnits++] = Unit;
} /* End of 'IB1_AnimAdd' function */


/*IB1_POINT WorldToScreen( VEC P )
{
  VEC P1, P2, P3, P4;
  IB1_POINT pt;
  DBL Xs, Ys;

  IB1_Anim.Mworld = MatrMulMatr(MatrScale(30, 30, 30), MatrTranslate(0, 0, 500)); 
  IB1_Anim.Mview = MatrViewLookAt(VecSet(0, 0, -3), VecSet(IB1_Anim.Jx, IB1_Anim.Jy, IB1_Anim.Jz), VecSet(0, 1, 0)); 
  IB1_Anim.Mproj = MatrProject(-1, 1, 1, -1, 1, 10);


  IB1_Anim.Wp = 2;
  IB1_Anim.Hp = 1;
  IB1_Anim.PD = 1;

  P1 = VecMulMatr(P, IB1_Anim.Mworld);
  P2 = VecMulMatr(P1, IB1_Anim.Mview);
  P3 = VecMulMatr(P2, IB1_Anim.Mproj);

  P4.X = P2.X * IB1_Anim.PD/P2.Z;
  P4.Y = P2.Y * IB1_Anim.PD/P2.Z;                                                            



  P4.X *= 2/IB1_Anim.Wp;
  P4.Y *= 2/IB1_Anim.Hp;

  Xs = (P4.X + 1) * (IB1_Anim.Ws - 1);
  Ys = (-P4.Y + 1) * (IB1_Anim.Hs - 1);
  pt.x = Xs;
  pt.y = Ys;
  return pt;
}                       */

/* END OF 'ANIM.C' FILE */