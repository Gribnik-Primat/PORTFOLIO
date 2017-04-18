/*Boikov Ivan 10-1 01.06*/

#include <math.h>
#include <stdio.h>
#include <windows.h>

#define WND_CLASS_NAME "My window class"




LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  INT x, y;
  INT X1=240, Y1=240, X2=560, Y2=240;  
  INT R=64;
  INT Rx1, Ry1, Rx2, Ry2; // координаты центра зрачка
  CHAR Buf[100];                                                          

  switch (Msg)
  {

  case WM_MOUSEMOVE:
    x = (SHORT)(lParam & 0xFFFF);
    y = (SHORT)((lParam >> 16) & 0xFFFF);
    wsprintf(Buf, "%5i|%5i  %X", x, y, wParam);
    if (1)//(wParam && MK_LBUTTON && wParam && MK_CONTROL)
    {
      HDC hDC;
      HPEN hBlackPen;
      HBRUSH hBlackBrush;
      
      hBlackPen = CreatePen( PS_SOLID, 1, RGB(0, 0, 0) );
      hBlackBrush = CreateSolidBrush( RGB(0, 0, 0) );
                                                    
      hDC = GetDC(hWnd); 
      x = (SHORT)LOWORD(lParam);
      y = (SHORT)HIWORD(lParam);

      Ellipse( hDC, 160, 160, 321, 321 );
      Ellipse( hDC, 480, 160, 641, 321 );

      Rx1 = R * abs(X1-x) / (INT)sqrt( (X1-x)*(X1-x) + (Y1-y)*(Y1-y) );
      Ry1 = R * abs(Y1-y) / (INT)sqrt( (X1-x)*(X1-x) + (Y1-y)*(Y1-y) );
      Rx2 = R * abs(X2-x) / (INT)sqrt( (X2-x)*(X2-x) + (Y2-y)*(Y2-y) );
      Ry2 = R * abs(Y2-y) / (INT)sqrt( (X2-x)*(X2-x) + (Y2-y)*(Y2-y) );

      if (x>X1 && x<X2)
        Rx2=-Rx2;
      if (x<X1)
        Rx1=-Rx1, Rx2=-Rx2;
      if (y<Y1)
        Ry1=-Ry1, Ry2=-Ry2;

      SelectObject(hDC, hBlackPen);
      SelectObject(hDC, hBlackBrush);
        Ellipse( hDC, X1+Rx1-16, Y1+Ry1-16, X1+Rx1+16, Y1+Ry1+16 );
        Ellipse( hDC, X2+Rx2-16, Y2+Ry2-16, X2+Rx2+16, Y2+Ry2+16 );
      DeleteObject(hBlackPen);
      DeleteObject(hBlackBrush);
    }
    SetWindowText(hWnd, Buf);
    return 0;
  case  WM_LBUTTONDOWN:
    SetCapture(hWnd);
    return 0;
  case WM_LBUTTONUP:
    SetCapture(NULL);
    return 0;
  case WM_DESTROY:         
    PostQuitMessage(30);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */


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
  wc.lpfnWndProc = MyWindowFunc; 
  wc.lpszClassName = WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindowA(WND_CLASS_NAME, "> 30 <",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
    10, 10, 1000, 500, NULL, NULL, hInstance, NULL);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return msg.wParam;
}