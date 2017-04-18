/* FILE NAME: ANIM.C
 * PROGRAMMER: IK1
 * DATE: 06.06.2013
 * PURPOSE: Main animation implementation module.
 */

#include <stdlib.h>
#include <string.h>
#include <string.h>

#include "anim.h"


static BOOL IK1_IsInit;
static ik1ANIM IK1_Anim;



#define IK1_MAX_UNITS 1000
INT IK1_NumOfUnits;
static ik1UNIT *IK1_Units[IK1_MAX_UNITS];

BOOL IK1_AnimInit( HWND hWnd )
{
  INT i;
  HDC hDC;

  IK1_Anim.hWnd = hWnd;
  hDC = GetDC(hWnd);
  IK1_Anim.hDC  = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);

  IK1_IsInit = TRUE;

  for (i = 0; i < IK1_NumOfUnits; i++)
    IK1_Units[i]->Init(IK1_Units[i], &IK1_Anim);

  IK1_Anim.MatrWorld = MatrUnit();
  IK1_Anim.MatrView = MatrUnit();
  IK1_Anim.MatrProj = MatrUnit();
  IK1_Anim.PD = 1;
  IK1_Anim.Wp = 1;
  IK1_Anim.Hp = 1;
  return TRUE;
} /* End of 'IK1_AnimInit' function */

VOID IK1_AnimClose( VOID )
{
  INT i;

  if (IK1_Anim.hDC != NULL)
    DeleteDC(IK1_Anim.hDC), IK1_Anim.hDC = NULL;
  if (IK1_Anim.hFrame != NULL)
    DeleteObject(IK1_Anim.hFrame), IK1_Anim.hFrame = NULL;

  for (i = 0; i < IK1_NumOfUnits; i++)
    IK1_Units[i]->Close(IK1_Units[i]);

  IK1_IsInit = FALSE;
} /* End of 'IK1_AnimClose' function */

VOID IK1_AnimResize( INT W, INT H )
{
  BITMAPINFOHEADER bih;

  if (!IK1_IsInit)
    return;

  if (IK1_Anim.hFrame != NULL)
    DeleteObject(IK1_Anim.hFrame);

  bih.biSize = sizeof(BITMAPINFOHEADER);
  bih.biWidth = IK1_Anim.W = W;
  bih.biHeight = -(IK1_Anim.H = H);
  bih.biPlanes = 1;    /* всегда */
  bih.biBitCount = 32; /* xRGB */
  bih.biCompression = BI_RGB;
  bih.biSizeImage = W * H * 4;
  bih.biClrImportant = 0;
  bih.biClrUsed = 0;
  bih.biXPelsPerMeter = bih.biYPelsPerMeter = 0;

  IK1_Anim.hFrame = CreateDIBSection(NULL, (BITMAPINFO *)&bih,
    DIB_RGB_COLORS, (VOID *)&IK1_Anim.Bits, NULL, 0);

  SelectObject(IK1_Anim.hDC, IK1_Anim.hFrame);
    
  IK1_AnimRender();
} /* End of 'IK1_AnimResize' function */

VOID IK1_AnimRender( VOID )
{
  INT i;

  if (!IK1_IsInit)
    return;


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
        memcpy(IK1_Anim.JButOld, IK1_Anim.JBut, 32);
        for (i = 0; i < 32; i++)
          IK1_Anim.JBut[i] = (ji.dwButtons >> i) & 1;

        /* оси переводим в диапазон -1..1 */
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


  for (i = 0; i < IK1_NumOfUnits; i++)
    IK1_Units[i]->Response(IK1_Units[i], &IK1_Anim);

  SelectObject(IK1_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(IK1_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(IK1_Anim.hDC, RGB(50, 150, 200));
  Rectangle(IK1_Anim.hDC, 0, 0, IK1_Anim.W, IK1_Anim.H);
  
  for (i = 0; i < IK1_NumOfUnits; i++)
  {
    SelectObject(IK1_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(IK1_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(IK1_Anim.hDC, RGB(0, 0, 0));
    SetDCPenColor(IK1_Anim.hDC, RGB(255, 255, 255));
    IK1_Units[i]->Render(IK1_Units[i], &IK1_Anim);
  }
} /* End of 'IK1_AnimRender' function */

VOID IK1_AnimCopyFrame( VOID )
{
  HDC hDC;

  if (!IK1_IsInit)
    return;
  /* Выводм кадр в окно анимации */
  hDC = GetDC(IK1_Anim.hWnd);
  BitBlt(hDC, 0, 0, IK1_Anim.W, IK1_Anim.H, IK1_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(IK1_Anim.hWnd, hDC);
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

POINT IK1_AnimWorldToScreen(VEC P)
{
  /*
  
  VEC P1, P2, P3, P4;  
  INT XS, YS;
  POINT pt;

  P1 = VecMulMatr(P, IK1_Anim.MatrWorld);
  P2 = VecMulMatr(P1,IK1_Anim.MatrView);
  P3 = VecMulMatr(P2,IK1_Anim.MatrProj);
  P4.X = P3.X *  IK1_Anim.PD / -P3.Z;
  P4.Y = -P3.Y *  IK1_Anim.PD / -P3.Z;

  
  P4.X *= 2/(IK1_Anim.Wp);
  P4.Y *= 2/(IK1_Anim.Hp);
                             
  XS = (P4.X + 1)  *  ((IK1_Anim.W - 1) / 2);
  YS = (P4.Y + 1)  * ((IK1_Anim.H - 1) / 2);

  pt.x = XS;
  pt.y = YS;

  return pt;
    */
  VEC P1, P2, P3, P4;
  POINT pt;
  DBL Dx = 0;
                 
  MatrWorld = MatrMulMatr(MatrRotateX(IB1_Anim.Jx), MatrRotateY(IB1_Anim.Jy));
  //MatrView = MatrUnit();
  //MatrWorld = MatrUnit();
  MatrView = MatrViewLookAt(VecSet(500, 500, 500), VecSet(0, 0, 0), VecSet(0, 1, 0)); 
  //MatrWorld = MatrTranslate(IB1_Anim.Time, Dy, Dz);
  MatrProj = MatrProject(-100, 100, -100, 100, 100, 10000);

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
