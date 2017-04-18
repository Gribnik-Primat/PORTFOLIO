/* Kriuchkov Iaroslav, 10-1, 01.06*/
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>


#define WND_CLASS_NAME "My window class"

/* Ссылки вперед */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );


INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW; /* Стиль окна: полностью перерисовывать
                                       * при изменении вертикального или
                                       * горизонтального размеров */
  wc.cbClsExtra = 0; /* Дополнительное количество байт для класса */
  wc.cbWndExtra = 0; /* Дополнительное количество байт для окна */
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW; /* Фоновый цвет - выбранный в системе */
  wc.hCursor = LoadCursor(NULL, IDC_ARROW); /* Загрузка курсора (системного) */
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); /* Загрузка пиктограммы (системной) */
  wc.lpszMenuName = NULL; /* Имя ресурса меню */
  wc.hInstance = hInstance; /* Дескриптор приложения, регистрирующего класс */
  wc.lpfnWndProc = MyWindowFunc; /* Указатель на функцию обработки */
  wc.lpszClassName = WND_CLASS_NAME; /* Имя класса */

  /* Регистрация класса в системе */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindowA(WND_CLASS_NAME, "CLOCK",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
    0, 0, 1920, 1280, NULL, NULL, hInstance, NULL);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return msg.wParam;
} 


LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  RECT rc;
  INT x, y;
  CHAR Buf[100];
  static CHAR T1[2],T2[2],T3[2],T4[2],T5[2],T6[2],T7[2],T8[2],T9[2],T10[2],T11[2],T12[2];
  static CHAR BUF[100];
  static CHAR Weekday[15];
  SYSTEMTIME tTime;
      INT AlphaHour, AlphaMinute, AlphaSecond,AlphaDay,AlphaMonth,AlphaYear,AlphaDayOfWeek;
      INT XC=900, YC=300, RS=180,RM=150,RH=120;
  HDC hDC;
  switch (Msg)
  {
  case  WM_CREATE:
    SetTimer(hWnd,30,1000,NULL);
    return 0;
  case  WM_TIMER:
    {
      hDC = GetDC(hWnd);
      Ellipse(hDC,700, 100, 1100 ,500 );
      
      
      GetSystemTime(&tTime); 
      AlphaHour=(INT)(tTime.wHour);
      AlphaMinute=(INT)(tTime.wMinute);
      AlphaSecond=(INT)(tTime.wSecond);
      AlphaMonth=(INT)(tTime.wMonth);
      AlphaDay=(INT)(tTime.wDay);
      AlphaYear=(INT)(tTime.wYear);
      AlphaDayOfWeek=(INT)(tTime.wDayOfWeek);
      if( AlphaDayOfWeek==1)
        sprintf(Weekday,"Monday");
      if( AlphaDayOfWeek==2)
        sprintf(Weekday,"Tuesday");
      if( AlphaDayOfWeek==3)
        sprintf(Weekday,"Wednesday");
      if( AlphaDayOfWeek==4)
        sprintf(Weekday,"Thursday");
      if( AlphaDayOfWeek==5)
        sprintf(Weekday,"Friday");
      if( AlphaDayOfWeek==6)
        sprintf(Weekday,"Saturday");
      if( AlphaDayOfWeek==0)
        sprintf(Weekday,"Sunday");
      MoveToEx(hDC,XC,YC,NULL);
      LineTo(hDC,(INT)(XC+RS*sin(M_PI*AlphaSecond/30)),(INT)(YC-RS*cos(M_PI*AlphaSecond/30)));
      
      MoveToEx(hDC,XC,YC,NULL);
      LineTo(hDC,(INT)(XC+RH*sin(M_PI*AlphaHour/6)),(INT)(YC-RH*cos(M_PI*AlphaHour/6)));
      
      MoveToEx(hDC,XC,YC,NULL);
      LineTo(hDC,(INT)(XC+RM*sin(M_PI*AlphaMinute/30)),(INT)(YC-RM*cos(M_PI*AlphaMinute/30)));
      /*Date and time*/
      sprintf(BUF,"%02i.%02i.%04i  %02i:%02i:%02i ",AlphaDay,AlphaMonth,AlphaYear,AlphaHour,AlphaMinute,AlphaSecond);
      TextOut(hDC,100,100,BUF,sprintf(BUF,"%02i.%02i.%04i  %02i:%02i:%02i",AlphaDay,AlphaMonth,AlphaYear,AlphaHour,AlphaMinute,AlphaSecond));
      TextOut(hDC,100,200,Weekday,100);
      /* Clock matches*/
      sprintf(T12,"12");
      TextOut(hDC,895,105,T12,sprintf(T12,"12"));
      sprintf(T3,"3");
      TextOut(hDC,1080,300,T3,sprintf(T3,"3"));
      sprintf(T6,"6");
      TextOut(hDC,880,485,T6,sprintf(T6,"6"));
      sprintf(T9,"9");
      TextOut(hDC,705,300,T9,sprintf(T9,"9"));

      
      ReleaseDC(hWnd,hDC);
    }
    break;
  case  WM_LBUTTONDOWN:
    SetCapture(hWnd);
    return 0;
  case WM_LBUTTONUP:
    SetCapture(NULL);
    return 0;
  case WM_MOUSEMOVE:
    {            
      
      x = (SHORT)(lParam & 0xFFFF);
      y = (SHORT)((lParam >> 16) & 0xFFFF);
      wsprintf(Buf, "%5i|%5i  %X", x, y, wParam);
          
    }
    SetWindowText(hWnd, Buf);
    return 0;
  case WM_DESTROY:
    PostQuitMessage(30);
    KillTimer(hWnd,30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
