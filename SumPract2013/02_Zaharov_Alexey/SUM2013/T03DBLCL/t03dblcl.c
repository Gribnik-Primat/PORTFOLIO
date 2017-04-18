#include <windows.h>
#include <winbase.h>

#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>

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

  hWnd = CreateWindowA(WND_CLASS_NAME, "T02CLOCK",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
    10, 10, 480, 480, NULL, NULL, hInstance, NULL);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return msg.wParam;
}

VOID DrawArrow (HDC hDC, INT Xc, INT Yc, INT Angle, DOUBLE ScaleX, DOUBLE ScaleY)
{
  INT i;
  POINT pts[]=
  {
    {1, 0}, {1, 2}, {2, 2}, {0, 4}, {-2, 2}, {-1, 2}, {-1, 0}
  };
  DOUBLE 
    a = Angle * M_PI/180,
    si = sin(a),
    co = cos(a);
  for (i=0; i<sizeof(pts)/sizeof(pts[0]); i++)
  {
    INT 
      x = ScaleX * pts[i].x * co - ScaleY * pts[i].y * si + Xc,
      y = ScaleX * pts[i].x * si + ScaleY * pts[i].y * co + Yc;
      pts[i].x = x;
      pts[i].y = y;
  }
  Polygon(hDC, pts, sizeof(pts)/sizeof(pts[0]));
}

LRESULT CALLBACK TranslateMessages( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  static POINT pts[4]=
  {
    {-5, -10}, {0, 5}, {5, -10}, {0, -190}
  }, pt;  
  INT x, y, Xc, Yc;
  HDC hDC;
  CREATESTRUCT *cs;
  PAINTSTRUCT ps;
  BITMAP Bm;
  static INT WinW, WinH;
  static HDC hMemDCFrame, hMemDC;
  static HBITMAP hBmFrame, hBmBack, hBmAnd, hBmXor;
  SYSTEMTIME st;
  RECT pRect;

  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 30, 5, NULL);

    hBmBack = LoadImage(NULL, "cf.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  
    hDC = GetDC(hWnd);
  
    hMemDCFrame = CreateCompatibleDC(hDC);
    hMemDC = CreateCompatibleDC(hDC);
    SetPixelV(hMemDC, 100, 100, RGB(0, 0, 0));
  
    ReleaseDC(hWnd, hDC);

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
    GetSystemTime(&st);
    GetWindowRect(hWnd, &pRect);
    Xc = (pRect.right - pRect.left) / 2;
    Yc = (pRect.bottom - pRect.top) / 2;

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    SelectObject(hMemDCFrame, GetStockObject(DC_BRUSH));
    SelectObject(hMemDCFrame, GetStockObject(NULL_PEN));

    SetDCBrushColor(hMemDCFrame, RGB(50, 150, 200));
    Rectangle(hMemDCFrame, 0, 0, WinW + 1, WinH + 1);
    SelectObject(hMemDCFrame, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDCFrame, RGB(255, 250, 250));

    GetObject(hBmBack, sizeof(Bm), &Bm);
    SelectObject(hMemDC, hBmBack);
    StretchBlt(hMemDCFrame, 0, 0, WinW, WinH,
      hMemDC, 0, 0, Bm.bmWidth, Bm.bmHeight, SRCCOPY);

    DrawArrow(hMemDCFrame, Xc, Yc, st.wSecond, 10, 70);
    
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