/* FILE NAME: T03DBLCL.C
 * PROGRAMMER: IK1
 * DATE: 03.06.2013
 * PURPOSE: Clock screensaver.
 */

#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define WND_CLASS_NAME "My window class"
VOID DrawArrow(HDC hDC, INT XC, INT YC, INT Angle, DOUBLE ScaleX, DOUBLE ScaleY)
{
  int i;
  POINT pts[]=
  {
    {1,0},{1,4},{2,4},{0,6},{-2,4},{-1,4},{-1,0}
  };
  DOUBLE
    a=Angle*M_PI/180,
    si=sin(a), co=cos(a);
  for(i=0; i<sizeof(pts)/sizeof(pts[0]); i++)
  {
    INT
      x = ScaleX * pts[i].x * co - ScaleY * pts[i].y * si + XC,
      y = -(ScaleX * pts[i].x * si + ScaleY * pts[i].y * co) + YC;
    pts[i].x=x;
    pts[i].y=y;
  }
  Polygon(hDC,pts,sizeof(pts)/sizeof(pts[0]));
}

/* Ссылки вперед */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

/* Главная функция программы.
 * АРГУМЕНТЫ:
 *   - дескриптор экземпляра приложения:
 *       HINSTANCE hInstance;	
 *   - дескриптор предыдущего экземпляра приложения
 *     (не используется и должно быть NULL):
 *       HINSTANCE hPrevInstance;
 *   - командная строка:
 *       CHAR *CmdLine;
 *   - флаг показа окна:
 *       INT ShowCmd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) код возврата в операционную систему.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  MSG msg;
  HWND hWnd;

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

  hWnd = CreateWindowA(WND_CLASS_NAME, "T03DBLCL",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
    0, 0, 1920, 1280, NULL, NULL, hInstance, NULL);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return msg.wParam;
} /* End of 'WinMain' function */

/* Функция обработки сообщения окна.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 *   - номер сообщения:
 *       UINT Msg;
 *   - параметр сообшения ('word parameter'):
 *       WPARAM wParam;
 *   - параметр сообшения ('long parameter'):
 *       LPARAM lParam;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (LRESULT) - в зависимости от сообщения.
 */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  INT x, y;
  HDC hDC;
  CREATESTRUCT *cs;
  PAINTSTRUCT ps;
  POINT pt;
  BITMAP Bm;
  CHAR Buf[100];
  static INT WinW, WinH;
  static HDC hMemDCFrame, hMemDC;
  static HBITMAP hBmFrame, hBmBack, hBmAnd, hBmXor;
  INT AlphaHour, AlphaMinute, AlphaSecond,AlphaDay,AlphaMonth,AlphaYear,AlphaDayOfWeek;
  INT XC=947, YC=527, RS=300,RM=250,RH=200;
  SYSTEMTIME tTime;
  
  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 30, 1, NULL);

    hBmBack = LoadImage(NULL, "cf.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    
    /*** создаем контекст в памяти ***/
    /* определяем контект окна */
    hDC = GetDC(hWnd);
     
    /* создаем контекст в памяти, совместимый с контекстом окна */
    hMemDCFrame = CreateCompatibleDC(hDC);
    hMemDC = CreateCompatibleDC(hDC);
    /* убираем контекст окна */
    ReleaseDC(hWnd, hDC);

    return 0;
  case WM_SIZE:
    WinW = LOWORD(lParam);
    WinH = HIWORD(lParam);
    //w=WinW/2;
    //h=WinH/2;
    /*** выделяем картинку (уничтожая старую) ***/
    if (hBmFrame != NULL)
      DeleteObject(hBmFrame);
    /* создаем в памяти изображение, совместимое с контектом окна */
    hDC = GetDC(hWnd);
    hBmFrame = CreateCompatibleBitmap(hDC, WinW, WinH);
     
    ReleaseDC(hWnd, hDC); 
    /* выбираем ее в контекст памяти */
    SelectObject(hMemDCFrame, hBmFrame);

    /* первичная отрисовка в новых размерах */
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_TIMER:
    /* построение всего кадра */
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    GetSystemTime(&tTime); 
      AlphaHour=(INT)(tTime.wHour);
      AlphaMinute=(INT)(tTime.wMinute);
      AlphaSecond=(INT)(tTime.wSecond);
      AlphaMonth=(INT)(tTime.wMonth);
      AlphaDay=(INT)(tTime.wDay);
      AlphaYear=(INT)(tTime.wYear);
    
    SelectObject(hMemDCFrame, GetStockObject(DC_BRUSH));
    SelectObject(hMemDCFrame, GetStockObject(NULL_PEN));

    SetDCBrushColor(hMemDCFrame, RGB(255, 255, 255));
    Rectangle(hMemDCFrame, 0, 0, WinW + 1, WinH + 1);
    SelectObject(hMemDCFrame, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDCFrame, RGB(255, 2, 2));  

    GetObject(hBmBack, sizeof(Bm), &Bm);
    SelectObject(hMemDC, hBmBack);

    BitBlt(hMemDCFrame, 500, 80, WinW, WinH,
      hMemDC, 0, 0,  SRCCOPY);

    DrawArrow(hMemDCFrame, WinW / 2, WinH / 2, 360 * AlphaSecond / 60 , 5, 50);
    DrawArrow(hMemDCFrame, WinW / 2, WinH / 2, 360 * AlphaMinute / 60, 8, 40);
    DrawArrow(hMemDCFrame, WinW / 2, WinH / 2,  AlphaHour , 11, 20);

    /* засыл перерисовки */
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_PAINT:

    /* отрисовка */
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, WinW, WinH, hMemDCFrame, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
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
    /* уничтожаем контекст в памяти и картинку */
    if (hBmFrame != NULL)
      DeleteObject(hBmFrame);
    if (hMemDCFrame != NULL)
      DeleteDC(hMemDCFrame);

    /* Послать сообщение 'WM_QUIT' с параметром 'wParam' 0 - код возврата,
     * замена вызов: PostMessage(hWnd, WM_QUIT, код_возврата, 0); */
    PostQuitMessage(30);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */


/* END OF 'T03DBLBF.C' FILE */
