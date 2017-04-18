/* FILE NAME: T05GLOBE.C
 * PROGRAMMER: IK1
 * DATE: 04.06.2013
 * PURPOSE: Earth globe.
 */

#include <windows.h>

#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define WND_CLASS_NAME "MainWindowClass"


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
    0, 0, 1920, 1080, NULL, NULL, hInstance, NULL);

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


LRESULT CALLBACK TranslateMessages( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  static POINT pts[4]=
  {
    {-5, -10}, {0, 5}, {5, -10}, {0, -190}
  }, pt;  
  INT x, y, Xc, Yc, i, j, M=500, N=500 / 2, R=100;
  DOUBLE theta, phi, z;
  HDC hDC;
  CREATESTRUCT *cs;
  PAINTSTRUCT ps;
  static INT WinW, WinH;
  static HDC hMemDCFrame, hMemDC;
  static HBITMAP hBmFrame, hBmBack, hBmAnd, hBmXor;
  RECT pRect;
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

    GetWindowRect(hWnd, &pRect);
    Xc = (pRect.right - pRect.left) / 2;
    Yc = (pRect.bottom - pRect.top) / 2;

    SelectObject(hMemDCFrame, GetStockObject(DC_BRUSH));  
    SelectObject(hMemDCFrame, GetStockObject(NULL_PEN));
    SetDCBrushColor(hMemDCFrame, RGB(0, 0, 0));
    Rectangle(hMemDCFrame, 0, 0, WinW, WinH);

    for (i=0; i<=N; i++)
      for (j=0; j<=M; j++)
      {
        INT x1 = 0, y1 = 0;

        theta = (i * M_PI / N +  PhaseShift);
        phi = (j * 2 * M_PI / M + PhaseShift);
        R = (WinW > WinH ? WinH : WinW) / 2;
        x = (int)(sin(theta) * sin(phi) * R) + WinW / 2 ;
        y = -(int)(cos(theta) * R) + WinH / 2 ;
        z = sin(theta) * cos(phi) ;
        if (z > 0)
        {
          x1 = j * w / (M + 1);
          y1 = i * h / (N + 1);
          SetPixel(hMemDCFrame, x, y, RGB(mem[(y1 * w + x1) * 3 + 2], mem[(y1 * w + x1) * 3 + 1], mem[(y1 * w + x1) * 3]));                        
        }        
      } 
    PhaseShift  += 0.005f; 
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
