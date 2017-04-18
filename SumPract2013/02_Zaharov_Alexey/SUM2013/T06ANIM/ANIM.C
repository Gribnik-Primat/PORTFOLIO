/* Zakharov Alexey, 10-1, Summer Practise 2013 */

#include <stdlib.h>
#include <string.h>
#include <string.h>
//#include <winbase.h>

#include "anim.h"

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
  HDC hDC;
  LARGE_INTEGER li;

  AZ1_Anim.hWnd = hWnd;
  hDC = GetDC(hWnd);
  AZ1_Anim.hDC  = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
  
  QueryPerformanceFrequency (&li); 
  TimeFreq  = li.QuadPart;
  QueryPerformanceCounter(&li); 
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = TimeShift = 0;
  FrameCounter = 0;

  AZ1_IsInit = TRUE;

  for (i = 0; i < AZ1_NumOfUnits; i++)
    AZ1_Units[i]->Init(AZ1_Units[i], &AZ1_Anim);

  return TRUE;
} /* End of 'AZ1_AnimInit' function */

VOID AZ1_AnimClose( VOID )
{
  INT i;

  if (AZ1_Anim.hDC != NULL)
    DeleteDC(AZ1_Anim.hDC), AZ1_Anim.hDC = NULL;
  if (AZ1_Anim.hFrame != NULL)
    DeleteObject(AZ1_Anim.hFrame), AZ1_Anim.hFrame = NULL;

  for (i = 0; i < AZ1_NumOfUnits; i++)
    AZ1_Units[i]->Close(AZ1_Units[i]);

  AZ1_IsInit = FALSE;
} /* End of 'AZ1_AnimClose' function */

VOID AZ1_AnimResize( INT W, INT H )
{
  BITMAPINFOHEADER bih;

  if (!AZ1_IsInit)
    return;

  if (AZ1_Anim.hFrame != NULL)
    DeleteObject(AZ1_Anim.hFrame);

  bih.biSize = sizeof(BITMAPINFOHEADER);
  bih.biWidth = AZ1_Anim.W = W;
  bih.biHeight = -(AZ1_Anim.H = H);
  bih.biPlanes = 1;  
  bih.biBitCount = 32; /* xRGB */
  bih.biCompression = BI_RGB;
  bih.biSizeImage = W * H * 4;
  bih.biClrImportant = 0;
  bih.biClrUsed = 0;
  bih.biXPelsPerMeter = bih.biYPelsPerMeter = 0;

  AZ1_Anim.hFrame = CreateDIBSection(NULL, (BITMAPINFO *)&bih,
    DIB_RGB_COLORS, (VOID *)&AZ1_Anim.Bits, NULL, 0);

  SelectObject(AZ1_Anim.hDC, AZ1_Anim.hFrame);
    
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
  
  AZ1_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  AZ1_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;
  
  if (AZ1_Anim.IsPause)
    AZ1_Anim.DeltaTime = 0, TimePause += li.QuadPart - TimeOld;
  else
    AZ1_Anim.DeltaTime = AZ1_Anim.GlobalDeltaTime;

  AZ1_Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause - TimeShift) /
    TimeFreq;

  if (TimeFPS > TimeFreq)
  {
    AZ1_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
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

        if (ji.dwPOV == 0xFFFF)
          AZ1_Anim.Jpov = 0;
        else
          AZ1_Anim.Jpov = ji.dwPOV / 4500 + 1;
      }
    }
  }

  for (i = 0; i < AZ1_NumOfUnits; i++)
    AZ1_Units[i]->Response(AZ1_Units[i], &AZ1_Anim);

  SelectObject(AZ1_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(AZ1_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(AZ1_Anim.hDC, RGB(50, 150, 200));
  Rectangle(AZ1_Anim.hDC, 0, 0, AZ1_Anim.W, AZ1_Anim.H);
  
  for (i = 0; i < AZ1_NumOfUnits; i++)
  {
    SelectObject(AZ1_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(AZ1_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(AZ1_Anim.hDC, RGB(0, 0, 0));
    SetDCPenColor(AZ1_Anim.hDC, RGB(255, 255, 255));
    AZ1_Units[i]->Render(AZ1_Units[i], &AZ1_Anim);
  }
} /* End of 'AZ1_AnimRender' function */

VOID AZ1_AnimCopyFrame( VOID )
{
  HDC hDC;

  if (!AZ1_IsInit)
    return;
  /* Выводм кадр в окно анимации */
  hDC = GetDC(AZ1_Anim.hWnd);
  BitBlt(hDC, 0, 0, AZ1_Anim.W, AZ1_Anim.H, AZ1_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(AZ1_Anim.hWnd, hDC);
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


POINT WorldToScreen( VEC P)
{
  VEC P1, P2, P3, P4;
  POINT pt;
  DBL Xs, Ys;

  AZ1_Anim.Mworld = MatrMulMatr(MatrRotateX(AZ1_Anim.Jx), MatrRotateY(AZ1_Anim.Jy));
  AZ1_Anim.Mview = MatrViewLookAt(VecSet(500, 500, 500), VecSet(0, 0, 0), VecSet(0, 1, 0)); 
  AZ1_Anim.Mproj = MatrProject(-100, 100, -100, 100, 100, 10000);

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
