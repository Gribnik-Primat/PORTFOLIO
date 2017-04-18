/* FILE NAME: ANIM.C
 * PROGRAMMER: IK1
 * DATE: 06.06.2013
 * PURPOSE: Main animation implementation module.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "anim.h"
#include "heightmap.h"

static BOOL IK1_IsInit;

INT64
  TimeFreq,  
  TimeStart, 
  TimeOld,   
  TimePause, 
  TimeShift, 
  TimeFPS;   
LONG FrameCounter;


#define IK1_MAX_UNITS 1000
INT IK1_NumOfUnits;
static ik1UNIT *IK1_Units[IK1_MAX_UNITS];

BOOL IK1_AnimInit( HWND hWnd )
{
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};
  LARGE_INTEGER li;
  
  QueryPerformanceFrequency (&li); 
  TimeFreq  = li.QuadPart;
  QueryPerformanceCounter(&li); 
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = TimeShift = 0;
  FrameCount = 0;

  IK1_Anim.hWnd = hWnd;
  IK1_Anim.hDC = GetDC(hWnd);

  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 24;
  pfd.cStencilBits = 8;

  i = ChoosePixelFormat(IK1_Anim.hDC, &pfd);
  DescribePixelFormat(IK1_Anim.hDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(IK1_Anim.hDC, i, &pfd);

  IK1_Anim.hGLRC = wglCreateContext(IK1_Anim.hDC);
  wglMakeCurrent(IK1_Anim.hDC, IK1_Anim.hGLRC);

  LoadRawFile("Region0000.raw", MAP_SIZE * MAP_SIZE);  

  if (glewInit() != GLEW_OK)
  {
    ReleaseDC(IK1_Anim.hWnd, IK1_Anim.hDC);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(IK1_Anim.hGLRC);
    memset(&IK1_Anim, 0, sizeof(IK1_Anim));
    return FALSE;
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);

  for (i = 0; i < IK1_NumOfUnits; i++)
    IK1_Units[i]->Init(IK1_Units[i], &IK1_Anim);
  glClearColor(0.3, 0.5, 0.7, 1);

  IK1_IsInit = TRUE;
  return TRUE;
} /* End of 'IK1_AnimInit' function */

VOID IK1_AnimClose( VOID )
{
  INT i;

  ReleaseDC(IK1_Anim.hWnd, IK1_Anim.hDC);
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(IK1_Anim.hGLRC);

  for (i = 0; i < IK1_NumOfUnits; i++)
    IK1_Units[i]->Close(IK1_Units[i]);

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);

  IK1_IsInit = FALSE;
} /* End of 'IK1_AnimClose' function */

VOID IK1_AnimResize( INT W, INT H )
{
  if (!IK1_IsInit)
    return;
  IK1_Anim.W = W;
  IK1_Anim.H = H;

  glViewport(0, 0, W, H);
  IK1_AnimRender();
} /* End of 'IK1_AnimResize' function */

VOID IK1_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  CHAR Buf[100];

  if (!IK1_IsInit)
    return;

  /* Таймер */  
  QueryPerformanceCounter(&li);
  IK1_Anim.GlobalTime = (FLT)(li.QuadPart - TimeStart) / TimeFreq;
  IK1_Anim.GlobalDeltaTime = (FLT)(li.QuadPart - TimeOld) / TimeFreq;
  
  if (IK1_Anim.IsPause)
    IK1_Anim.DeltaTime = 0, TimePause += li.QuadPart - TimeOld;
  else
    IK1_Anim.DeltaTime = IK1_Anim.GlobalDeltaTime;

  IK1_Anim.Time = (FLT)(li.QuadPart - TimeStart - TimePause - TimeShift) /
    TimeFreq;

  if (TimeFPS > TimeFreq)
  {
    IK1_Anim.FPS = FrameCounter / ((FLT)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
  }
  TimeOld = li.QuadPart;

  //sprintf(Buf," FPS : %lf", IK1_Anim.FPS);
  //SetWindowText(IK1_Anim.hWnd, Buf);

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
        memcpy(IK1_Anim.JButOld, IK1_Anim.JBut, 32);
        for (i = 0; i < 32; i++)
          IK1_Anim.JBut[i] = (ji.dwButtons >> i) & 1;

        IK1_Anim.Jx = 2.0 * (ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin) - 1;
        IK1_Anim.Jy = 2.0 * (ji.dwYpos - jc.wYmin) / (jc.wYmax - jc.wYmin) - 1;
        IK1_Anim.Jz = 2.0 * (ji.dwZpos - jc.wZmin) / (jc.wZmax - jc.wZmin) - 1;
        IK1_Anim.Jr = 2.0 * (ji.dwRpos - jc.wRmin) / (jc.wRmax - jc.wRmin) - 1;

        if (ji.dwPOV == 0xFFFF)
          IK1_Anim.Jpov = 0;
        else
          IK1_Anim.Jpov = ji.dwPOV / 4500 + 1;
      }
    }
  }

  IK1_Anim.alpha += IK1_Anim.Jx / 1000;
  if (IK1_Anim.betha + IK1_Anim.Jy / 100 <= 3.14 / 2 && IK1_Anim.betha + IK1_Anim.Jy / 100 >= -3.14 / 2)
    IK1_Anim.betha += IK1_Anim.Jy / 300;
  IK1_Anim.gamma += IK1_Anim.Jz / 50;

  for (i = 0; i < IK1_NumOfUnits; i++)
    IK1_Units[i]->Response(IK1_Units[i], &IK1_Anim);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  

  for (i = 0; i < IK1_NumOfUnits; i++)
    IK1_Units[i]->Render(IK1_Units[i], &IK1_Anim);

  glFinish();


} /* End of 'IK1_AnimRender' function */

VOID IK1_AnimCopyFrame( VOID )
{


  if (!IK1_IsInit)
    return;  
  SwapBuffers(IK1_Anim.hDC);
} /* End of 'IK1_AnimCopyFrame' function */

static VOID IK1_DefUnitInit( ik1UNIT *Unit, ik1ANIM *Ani )
{
} /* End of 'IK1_DefUnitInit' function */

static VOID IK1_DefUnitClose( ik1UNIT *Unit )
{
} /* End of 'IK1_DefUnitClose' function */

static VOID IK1_DefUnitResponse( ik1UNIT *Unit, ik1ANIM *Ani )
{
} /* End of 'IK1_DefUnitResponse' function */

static VOID IK1_DefUnitRender( ik1UNIT *Unit, ik1ANIM *Ani )
{
} /* End of 'IK1_DefUnitRender' function */

ik1UNIT * IK1_UnitCreate( INT Size )
{
  ik1UNIT *Unit;

  if (Size < sizeof(ik1UNIT) || (Unit = malloc(Size)) == NULL)
    return NULL;

  memset(Unit, 0, Size);
  Unit->Init = IK1_DefUnitInit;
  Unit->Close = IK1_DefUnitClose;
  Unit->Response = IK1_DefUnitResponse;
  Unit->Render = IK1_DefUnitRender;
  return Unit;
} /* End of 'IK1_UnitCreate' function */

VOID IK1_AnimAdd( ik1UNIT *Unit )
{
  if (IK1_NumOfUnits >= IK1_MAX_UNITS)
    return;
  IK1_Units[IK1_NumOfUnits++] = Unit;
} /* End of 'IK1_AnimAdd' function */


IK1_POINT WorldToScreen( VEC P )
{
  VEC P1, P2, P3, P4;
  IK1_POINT pt;
  FLT Xs, Ys;

  IK1_Anim.MatrWorld = MatrMulMatr(MatrScale(30, 30, 30), MatrTranslate(IK1_Anim.Jpov, IK1_Anim.Jpov, 500)); 
  IK1_Anim.MatrView = MatrViewLookAt(VecSet(0, 0, -3), VecSet(-IK1_Anim.Jx, IK1_Anim.Jy, IK1_Anim.Jr), VecSet(0, 1, 0)); 
  IK1_Anim.MatrProj = MatrProjection(-1, 1, 1, -1, 1, 10);


  IK1_Anim.Wp = 1.0;
  IK1_Anim.Hp = 1.0;
  IK1_Anim.PD = 1;

  P1 = VecMulMatr(P, IK1_Anim.MatrWorld);
  P2 = VecMulMatr(P1, IK1_Anim.MatrView);
  P3 = VecMulMatr(P2, IK1_Anim.MatrProj);

  P4.X = P2.X * IK1_Anim.PD/P2.Z;
  P4.Y = P2.Y * IK1_Anim.PD/P2.Z;                                                            



  P4.X *= 2/IK1_Anim.Wp;
  P4.Y *= 2/IK1_Anim.Hp;

  Xs = (P4.X + 1) * (IK1_Anim.Ws - 1);
  Ys = (-P4.Y + 1) * (IK1_Anim.Hs - 1);
  pt.x = Xs;
  pt.y = Ys;
  return pt;
}

/* END OF 'ANIM.C' FILE */