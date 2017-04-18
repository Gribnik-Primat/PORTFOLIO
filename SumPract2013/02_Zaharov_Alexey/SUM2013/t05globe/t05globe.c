/* Zakharov Alexey, 10-1, Summer Practise 2013 */

#include <windows.h>
#include <mmsystem.h>

#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define WND_CLASS_NAME "MainWindowClass"

#pragma comment(lib, "winmm")


LRESULT CALLBACK TranslateMessages( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );


INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW; 
  wc.cbClsExtra = 0; 
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW; 
  wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.lpszMenuName = NULL; 
  wc.hInstance = hInstance; 
  wc.lpfnWndProc = TranslateMessages;
  wc.lpszClassName = WND_CLASS_NAME; 

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindowA(WND_CLASS_NAME, "T05GLOBE",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
    10, 10, 480, 480, NULL, NULL, hInstance, NULL);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return msg.wParam;
}

static BYTE *mem=NULL;
static INT w, h;
int LoadGlobe( void )
{
  FILE *F;
  
  F = fopen("X:\\PICS\\GLOBE.G24", "rb");
  if (F==NULL)
    return 0;
  fread(&w, 2, 1, F);
  fread(&h, 2, 1, F);
  mem = malloc(w * h * 3);
  if (mem==NULL)
    return 0;
  fread(mem, 3, w*h, F);
  return 1;
}

static FLOAT PhaseShift=0;
static FLOAT AxisShift=30;

#define M 255
#define N (M/2)

LRESULT CALLBACK TranslateMessages( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  static POINT pts[4] = {0}, pt;  
  INT x, y, i, j, R;
  DOUBLE theta, phi, z, GlobePos[N][M][3];
  HDC hDC;
  CREATESTRUCT *cs;
  PAINTSTRUCT ps;
  INT WinW, WinH;
  static HDC hMemDCFrame, hMemDC;
  static HBITMAP hBmFrame, hBmBack, hBmAnd, hBmXor;
  RECT pRect;
  HBRUSH hBr;

  /* Для тов. Джойстика */
  BYTE JBut[32], JButOld[32];
  DOUBLE Jx, Jy, Jz, Jr;
  INT Jpov;

  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 30, 10, NULL);  
    hDC = GetDC(hWnd);
    hMemDCFrame = CreateCompatibleDC(hDC);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    if (!LoadGlobe())
    {
      MessageBox(NULL, "Error loading globe texture", 0, MB_OK);
      exit(1);
    }
    return 0;
  case WM_SIZE:
    WinW = 1;
    WinW = LOWORD(lParam);
    WinH = HIWORD(lParam);
    if (hBmFrame != NULL)
      DeleteObject(hBmFrame);    
    hDC = GetDC(hWnd);
    hBmFrame = CreateCompatibleBitmap(hDC, WinW, WinH);
    ReleaseDC(hWnd, hDC);    
    SelectObject(hMemDCFrame, hBmFrame);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;    
  case WM_TIMER:  
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    /* Опрос Джойстика */
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
          memcpy(JButOld, JBut, 32);
          for (i = 0; i < 32; i++)
            JBut[i] = (ji.dwButtons >> i) & 1;

          Jx = 2.0 * (ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin) - 1;
          Jy = 2.0 * (ji.dwYpos - jc.wYmin) / (jc.wYmax - jc.wYmin) - 1;
          Jz = 2.0 * (ji.dwZpos - jc.wZmin) / (jc.wZmax - jc.wZmin) - 1;
          Jr = 2.0 * (ji.dwRpos - jc.wRmin) / (jc.wRmax - jc.wRmin) - 1;

          if (ji.dwPOV == 0xFFFF)
            Jpov = 0;
          else
            Jpov = ji.dwPOV / 4500 + 1;
        }
      }
    }
    /* Конец опроса Джойстика*/

    GetWindowRect(hWnd, &pRect);

    SelectObject(hMemDCFrame, GetStockObject(DC_BRUSH));  
    SelectObject(hMemDCFrame, GetStockObject(NULL_PEN));
    SetDCBrushColor(hMemDCFrame, RGB(0, 0, 0));
    Rectangle(hMemDCFrame, 0, 0, WinW, WinH);

    for (i = 0; i < N-1; i++)
      for (j = 0; j < M-1; j++)
      {                                                                         
         theta = (i * M_PI) / N;
         phi = (j * 2 * M_PI + (Jx + 1) * 100) / M + PhaseShift;
         R = (WinW > WinH ? WinH : WinW) / 2;
         x = (int)(sin(theta) * sin(phi) * R) + WinW / 2;
         y = -(int)(cos(theta) * R) + WinH / 2;
         z = cos(phi) * sin(theta);
         GlobePos[i][j][0] = x;
         GlobePos[i][j][1] = y;
         GlobePos[i][j][2] = z;        
      }

      for (i = 0; i < N-1; i++)
        for (j = 0; j < M-1; j++)
        {
            static INT x1 = 0, y1 = 0; 
            pts[0].x = GlobePos[i][j][0];
            pts[0].y = GlobePos[i][j][1];

            pts[1].x = GlobePos[i][j+1][0];
            pts[1].y = GlobePos[i][j+1][1];

            pts[2].x = GlobePos[i+1][j+1][0];
            pts[2].y = GlobePos[i+1][j+1][1];

            pts[3].x = GlobePos[i+1][j][0];
            pts[3].y = GlobePos[i+1][j][1];
    
        if (GlobePos[i][j][2]>0 && GlobePos[i][j+1][2]>0 && GlobePos[i+1][j+1][2]>0 && GlobePos[i+1][j][2]>0)
        {
          x1 = j * w / (M + 1);
          y1 = i * h / (N + 1);
          SetDCBrushColor(hMemDCFrame, RGB(mem[(y1 * w + x1) * 3 + 2], mem[(y1 * w + x1) * 3 + 1], mem[(y1 * w + x1) * 3]));
          Polygon(hMemDCFrame, pts, sizeof(pts)/sizeof(pts[0]));  
        }
    }   
    PhaseShift += fabs(Jr + 2) * 0.005 + 0.015f; 
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_PAINT: 
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, WinW, WinH, hMemDCFrame, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    if (hBmFrame != NULL)
      DeleteObject(hBmFrame);
    if (hMemDCFrame != NULL)
      DeleteDC(hMemDCFrame);
    PostQuitMessage(30);
    KillTimer(hWnd, 30);
    return 0;
  } 
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} 