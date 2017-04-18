/* Zakharov Alexey, 10-1, Summer Practise 2013 */

#include <stdlib.h>
#include <string.h>
#include <string.h>

#include "anim.h"
#include "heightmap.h"

static BOOL AZ1_IsInit;

INT64
  TimeFreq,  
  TimeStart, 
  TimeOld,   
  TimePause, 
  TimeShift, 
  TimeFPS;   
LONG FrameCounter;


#define AZ1_MAX_UNITS 1000
INT AZ1_NumOfUnits;
static az1UNIT *AZ1_Units[AZ1_MAX_UNITS];

BOOL AZ1_AnimInit( HWND hWnd )
{
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  AZ1_Anim.hWnd = hWnd;
  AZ1_Anim.hDC = GetDC(hWnd);

  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 0;
  pfd.cStencilBits = 0;

  i = ChoosePixelFormat(AZ1_Anim.hDC, &pfd);
  DescribePixelFormat(AZ1_Anim.hDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(AZ1_Anim.hDC, i, &pfd);

  AZ1_Anim.hGLRC = wglCreateContext(AZ1_Anim.hDC);
  wglMakeCurrent(AZ1_Anim.hDC, AZ1_Anim.hGLRC);

  if (glewInit() != GLEW_OK)
  {
    ReleaseDC(AZ1_Anim.hWnd, AZ1_Anim.hDC);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(AZ1_Anim.hGLRC);
    memset(&AZ1_Anim, 0, sizeof(AZ1_Anim));
    return FALSE;
  }

  AZ1_IsInit = TRUE;
  //AZ1_Anim.dx = AZ1_Anim.dy = AZ1_Anim.dz = 0; 
  for (i = 0; i < AZ1_NumOfUnits; i++)
    AZ1_Units[i]->Init(AZ1_Units[i], &AZ1_Anim);

  glClearColor(0.3, 0.5, 0.7, 1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

//  LoadRawFile("Region0000.raw", MAP_SIZE * MAP_SIZE);
//  FillHeightMap();

  return TRUE;
} /* End of 'AZ1_AnimInit' function */

VOID AZ1_AnimClose( VOID )
{
  INT i;

  ReleaseDC(AZ1_Anim.hWnd, AZ1_Anim.hDC);
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(AZ1_Anim.hGLRC);

  for (i = 0; i < AZ1_NumOfUnits; i++)
    AZ1_Units[i]->Close(AZ1_Units[i]);

  AZ1_IsInit = FALSE;
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
} /* End of 'AZ1_AnimClose' function */

VOID AZ1_AnimResize( INT W, INT H )
{
  if (!AZ1_IsInit)
    return;

  AZ1_Anim.W = W;
  AZ1_Anim.H = H;
  glViewport(0, 0, W, H);
  AZ1_AnimRender();
} /* End of 'AZ1_AnimResize' function */

VOID AZ1_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;

  if (!AZ1_IsInit)
    return;

  /* Таймер */  
  QueryPerformanceCounter(&li);
  AZ1_Anim.GlobalTime = (FLT)(li.QuadPart - TimeStart) / TimeFreq;
  AZ1_Anim.GlobalDeltaTime = (FLT)(li.QuadPart - TimeOld) / TimeFreq;
  
  if (AZ1_Anim.IsPause)
    AZ1_Anim.DeltaTime = 0, TimePause += li.QuadPart - TimeOld;
  else
    AZ1_Anim.DeltaTime = AZ1_Anim.GlobalDeltaTime;

  AZ1_Anim.Time = (FLT)(li.QuadPart - TimeStart - TimePause - TimeShift) /
    TimeFreq;

  if (TimeFPS > TimeFreq)
  {
    AZ1_Anim.FPS = FrameCounter / ((FLT)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
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
        memcpy(AZ1_Anim.JButOld, AZ1_Anim.JBut, 32);
        for (i = 0; i < 32; i++)
          AZ1_Anim.JBut[i] = (ji.dwButtons >> i) & 1;

        AZ1_Anim.Jx = 2.0 * (ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin) - 1;
        AZ1_Anim.Jy = 2.0 * (ji.dwYpos - jc.wYmin) / (jc.wYmax - jc.wYmin) - 1;
        AZ1_Anim.Jz = 2.0 * (ji.dwZpos - jc.wZmin) / (jc.wZmax - jc.wZmin) - 1;
        AZ1_Anim.Jr = 2.0 * (ji.dwRpos - jc.wRmin) / (jc.wRmax - jc.wRmin) - 1;

        AZ1_Anim.alpha += AZ1_Anim.Jx / 1000;
        AZ1_Anim.beta += AZ1_Anim.Jy / 1000;
        AZ1_Anim.gamma += AZ1_Anim.Jz / 1000;

        if (ji.dwPOV == 0xFFFF)
          AZ1_Anim.Jpov = 0;
        else
          AZ1_Anim.Jpov = ji.dwPOV / 4500 + 1;
      }
    }
  }

  for (i = 0; i < AZ1_NumOfUnits; i++)
    AZ1_Units[i]->Response(AZ1_Units[i], &AZ1_Anim);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (i = 0; i < AZ1_NumOfUnits; i++)
    AZ1_Units[i]->Render(AZ1_Units[i], &AZ1_Anim);
  glFinish();
} /* End of 'AZ1_AnimRender' function */

VOID AZ1_AnimCopyFrame( VOID )
{
  if (!AZ1_IsInit)
    return;  
  SwapBuffers(AZ1_Anim.hDC);
} /* End of 'AZ1_AnimCopyFrame' function */

static VOID AZ1_DefUnitInit( az1UNIT *Unit, az1ANIM *Ani )
{
} /* End of 'AZ1_DefUnitInit' function */

static VOID AZ1_DefUnitClose( az1UNIT *Unit )
{
} /* End of 'AZ1_DefUnitClose' function */

static VOID AZ1_DefUnitResponse( az1UNIT *Unit, az1ANIM *Ani )
{
} /* End of 'AZ1_DefUnitResponse' function */

static VOID AZ1_DefUnitRender( az1UNIT *Unit, az1ANIM *Ani )
{
} /* End of 'AZ1_DefUnitRender' function */

az1UNIT * AZ1_UnitCreate( INT Size )
{
  az1UNIT *Unit;

  if (Size < sizeof(az1UNIT) || (Unit = malloc(Size)) == NULL)
    return NULL;

  memset(Unit, 0, Size);
  Unit->Init = AZ1_DefUnitInit;
  Unit->Close = AZ1_DefUnitClose;
  Unit->Response = AZ1_DefUnitResponse;
  Unit->Render = AZ1_DefUnitRender;
  return Unit;
} /* End of 'AZ1_UnitCreate' function */

VOID AZ1_AnimAdd( az1UNIT *Unit )
{
  if (AZ1_NumOfUnits >= AZ1_MAX_UNITS)
    return;
  AZ1_Units[AZ1_NumOfUnits++] = Unit;
} /* End of 'AZ1_AnimAdd' function */


AZ1_POINT WorldToScreen( VEC P )
{
  VEC P1, P2, P3, P4;
  AZ1_POINT pt;
  FLT Xs, Ys;

  AZ1_Anim.Mworld = MatrUnit();
  AZ1_Anim.Mview = MatrViewLookAt(VecSet(0, 0, -3), VecSet(AZ1_Anim.Jx, AZ1_Anim.Jy, AZ1_Anim.Jr), VecSet(0, 1, 0)); 
  AZ1_Anim.Mproj = MatrProject(-1, 1, 1, -1, 1, 10);

  AZ1_Anim.Wp = 1.0;
  AZ1_Anim.Hp = 1.0;
  AZ1_Anim.PD = 1;

  P1 = VecMulMatr(P, AZ1_Anim.Mworld);
  P2 = VecMulMatr(P1, AZ1_Anim.Mview);
  P3 = VecMulMatr(P2, AZ1_Anim.Mproj);

  P4.X = P2.X * AZ1_Anim.PD/P2.Z;
  P4.Y = P2.Y * AZ1_Anim.PD/P2.Z;

  P4.X *= 2/AZ1_Anim.Wp;
  P4.Y *= 2/AZ1_Anim.Hp;

  Xs = (P4.X + 1) * (AZ1_Anim.Ws - 1);
  Ys = (-P4.Y + 1) * (AZ1_Anim.Hs - 1);
  pt.x = Xs;
  pt.y = Ys;
  return pt;
}

/* END OF 'ANIM.C' FILE */
