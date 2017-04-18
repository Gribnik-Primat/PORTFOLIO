/*Boikov Ivan 10-1*/
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define WND_CLASS_NAME "My window class"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );
static digits[] = {125, 96, 55};

/*void drawdigit (HWND hWnd, INT x, INT Y, int color, int number)
{
  for (i = 0; i < 7; i++)
  if (digits[number] >> i)
} */

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW; /* Стиль окна:  полностью перерисовывать
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

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindowA(WND_CLASS_NAME, "Clock",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
    10, 10, 1900, 1024, NULL, NULL, hInstance, NULL);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return msg.wParam;
}


LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  INT x, y, i;
  CHAR Buf[100];
  CREATESTRUCT *cs;
  HDC hDC;
  SYSTEMTIME lpTime;
  static DOUBLE hour = 0, min = 0, sec = 0;
  RECT rc;
    
  hDC = GetDC(hWnd);
  Ellipse(hDC, 760, 112, 1160, 512);
  
  MoveToEx(hDC, 960, 312, NULL);
  LineTo(hDC, 50*sin(hour) + 960, 312 - 50 * cos(hour));
  MoveToEx(hDC, 960, 312, NULL);                                                         
  LineTo(hDC, 180*sin(min) + 960, 312 - 180 * cos(min));
  MoveToEx(hDC, 960, 312, NULL);
  LineTo(hDC, 100*sin(sec) + 960, 312 - 100 * cos(sec));

  TextOut(hDC, 1135, 312, Buf, sprintf(Buf, "3"));
  TextOut(hDC, 960, 482, Buf, sprintf(Buf, "6"));
  TextOut(hDC, 770, 312, Buf, sprintf(Buf, "9"));
  TextOut(hDC, 955, 122, Buf, sprintf(Buf, "12"));

  ReleaseDC(hWnd, hDC);
  GetClientRect(hWnd, &rc);

  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;                                                                                       
    GetLocalTime(&lpTime);
    hour = lpTime.wHour * M_PI / 6;
    min = lpTime.wMinute * M_PI / 30; 
    sec = lpTime.wSecond * M_PI / 30;
    SetTimer(hWnd, 30, 1000, NULL);
    return 0;
  case WM_TIMER:
    hour += M_PI / 21600;
    min += M_PI / 1800;
    sec += M_PI / 30;
    GetLocalTime(&lpTime);
    hDC = GetDC(hWnd);                               
    TextOut(hDC, rc.right / 2 - 100, rc.bottom - 30, Buf, sprintf(Buf, "Date: %0.2i:%0.2i:%0.2i", lpTime.wDay, lpTime.wMonth, lpTime.wYear));
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_DESTROY:
    PostQuitMessage(30);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} 

