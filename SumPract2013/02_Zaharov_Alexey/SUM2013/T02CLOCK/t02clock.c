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
    10, 10, 640, 480, NULL, NULL, hInstance, NULL);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return msg.wParam;
}

static INT Digits[]={};
VOID DrawDigit (int Digit)
{

}

LRESULT CALLBACK TranslateMessages( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  INT Xc=320, Yc=210, R1=160, R2=140, R3=110;
  SYSTEMTIME tTime;
  INT AlphaSecond, AlphaMinute, AlphaHour;
  HDC hDC;
  CHAR Buf[100];
  CHAR *DaysOfWeek[]={"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"};
  switch (Msg)
  {
    case WM_CREATE:
      SetTimer( hWnd, 1, 1000, NULL);
      SetTimer( hWnd, 2, 1, NULL);

      return 0;
    case WM_TIMER:                    
      hDC = GetDC(hWnd);
      Ellipse( hDC, 160, 50, 480, 370);

      GetLocalTime(&tTime);
      AlphaSecond = tTime.wSecond;
      AlphaMinute = tTime.wMinute;
      AlphaHour = tTime.wHour;
        
      MoveToEx( hDC, Xc, Yc, NULL );
      LineTo( hDC, (INT)(Xc+R1*sin(AlphaSecond*M_PI/30)), (INT)(Yc-R1*cos(AlphaSecond*M_PI/30)) );

      MoveToEx( hDC, Xc, Yc, NULL );
      LineTo( hDC, (INT)(Xc+R2*sin(AlphaMinute*M_PI/30)), (INT)(Yc-R1*cos(AlphaMinute*M_PI/30)) );

      MoveToEx( hDC, Xc, Yc, NULL );
      LineTo( hDC, (INT)(Xc+R3*sin(AlphaHour*M_PI/6)), (INT)(Yc-R1*cos(AlphaHour*M_PI/6)) );
     
      TextOut( hDC, 10, 430, Buf, sprintf(Buf, "Сегодня %s, %02i.%02i.%02i, %02i:%02i:%02i:%04i", 
        DaysOfWeek[tTime.wDayOfWeek], tTime.wDay, tTime.wMonth, tTime.wYear, tTime.wHour, tTime.wMinute, tTime.wSecond, tTime.wMilliseconds) );
      if(wParam==1)
        Beep(1300, 50);
      break;      
    case WM_DESTROY:
      KillTimer( hWnd, 1 );
      KillTimer( hWnd, 2 );
      PostQuitMessage(0);
      return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}