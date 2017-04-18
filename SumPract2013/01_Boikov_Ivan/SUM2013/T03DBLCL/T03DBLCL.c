/*Boikov Ivan 10-1*/
#include <stdio.h>
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define WND_CLASS_NAME "My window class"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );
static digits[] = {125, 96, 55};

void DrawArrow (HDC hDC, INT XC, INT YC, DOUBLE ANGLE, LONG SCALEX, LONG SCALEY)
{
  INT i;
  double alpha = ANGLE + M_PI / 2, si = cos(alpha), co = sin(alpha);
  POINT pts[] = 
  {
    {0, -2}, {-2, 8}, {0, 35}, {2, 8}
  };

  for (i = 0; i < sizeof(pts) / sizeof(pts[0]); i++)
  {
    LONG X = (LONG)(SCALEX * pts[i].x * co - SCALEY * pts[i].y * si + XC), 
         Y = (LONG)(-(SCALEX * pts[i].x * si + SCALEY * pts[i].y * co) + YC);
    pts[i].x = X;
    pts[i].y = Y;
  }
  Polygon(hDC, pts, sizeof(pts) / sizeof(pts[0]));
}

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  INT iWidth, iHeight;
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  iWidth = GetSystemMetrics(SM_CXSCREEN);
  iHeight = GetSystemMetrics(SM_CYSCREEN);
  wc.style = CS_VREDRAW | CS_HREDRAW; /* —тиль окна:  полностью перерисовывать
                                       * при изменении вертикального или
                                       * горизонтального размеров */
  wc.cbClsExtra = 0; /* ƒополнительное количество байт дл€ класса */
  wc.cbWndExtra = 0; /* ƒополнительное количество байт дл€ окна */
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW; /* ‘оновый цвет - выбранный в системе */
  wc.hCursor = LoadCursor(NULL, IDC_ARROW); /* «агрузка курсора (системного) */
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); /* «агрузка пиктограммы (системной) */
  wc.lpszMenuName = NULL; /* »м€ ресурса меню */
  wc.hInstance = hInstance; /* ƒескриптор приложени€, регистрирующего класс */
  wc.lpfnWndProc = MyWindowFunc; /* ”казатель на функцию обработки */
  wc.lpszClassName = WND_CLASS_NAME; /* »м€ класса */

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindowA(WND_CLASS_NAME, "Clock",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
    0, 0, iWidth, iHeight, NULL, NULL, hInstance, NULL);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return msg.wParam;
}


LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  CREATESTRUCT *cs;
  PAINTSTRUCT ps;
  SYSTEMTIME lpTime;
  POINT pt;
  CHAR Buf[100];
  BITMAP Bm;
  static DOUBLE hour = 0, min = 0, sec = 0;
  RECT rc;
  static INT WinW, WinH;
  static HDC hMemDCFrame, hMemDC;
  static HBITMAP hBmFrame, hBmBack, hBmAnd, hBmXor;
  static HFONT hFnt;
  static LOGFONT logfont;
  static CHOOSEFONT cf;

  GetClientRect(hWnd, &rc);
  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 30, 1000, NULL);

    hBmBack = LoadImage(NULL, "cf.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAnd = LoadImage(NULL, "and1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmXor = LoadImage(NULL, "xor1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    hDC = GetDC(hWnd);
    hMemDCFrame = CreateCompatibleDC(hDC);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    GetLocalTime(&lpTime);
    hour = lpTime.wHour * M_PI / 6;
    min = lpTime.wMinute * M_PI / 30;                  
    sec = lpTime.wSecond * M_PI / 30;

    cf.lStructSize = sizeof(cf);
    hFnt = CreateFont(-50, 0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH || FF_ROMAN, "Times"); 
    return 0;
  case WM_SIZE:
    WinW = LOWORD(lParam);
    WinH = HIWORD(lParam);
    /*** выдел€ем картинку (уничтожа€ старую) ***/
    if (hBmFrame != NULL)
      DeleteObject(hBmFrame);
    /* создаем в пам€ти изображение, совместимое с контектом окна */
    hDC = GetDC(hWnd);
    hBmFrame = CreateCompatibleBitmap(hDC, WinW, WinH);
    ReleaseDC(hWnd, hDC);
    /* выбираем ее в контекст пам€ти */
    SelectObject(hMemDCFrame, hBmFrame);

    /* первична€ отрисовка в новых размерах */
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_TIMER:
    /* построение всего кадра */
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt); 

    SelectObject(hMemDCFrame, GetStockObject(DC_BRUSH));
    SelectObject(hMemDCFrame, GetStockObject(NULL_PEN));

    SetDCBrushColor(hMemDCFrame, RGB(255, 255, 255));
    Rectangle(hMemDCFrame, 0, 0, WinW + 1, WinH + 1);
    SelectObject(hMemDCFrame, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDCFrame, RGB(255, 250, 250));

    GetObject(hBmBack, sizeof(Bm), &Bm);
    SelectObject(hMemDC, hBmBack);                                                 
    BitBlt(hMemDCFrame, (rc.right - 720) / 2, (rc.bottom - 720) / 2, Bm.bmWidth, Bm.bmHeight, hMemDC, 0, 0,  SRCCOPY);

    SetDCBrushColor(hMemDCFrame, RGB(0, 0, 0));
    SetDCPenColor(hMemDCFrame, RGB(255, 255, 255));
    
    DrawArrow(hMemDCFrame, rc.right / 2, rc.bottom / 2, min, 8, 8);
    DrawArrow(hMemDCFrame, rc.right / 2, rc.bottom / 2, hour, 6, 3);
    SetDCBrushColor(hMemDCFrame, RGB(255, 0, 0));
    DrawArrow(hMemDCFrame, rc.right / 2, rc.bottom / 2, sec, 6, 6);

    /* вывод маскированной картинки */
    /*SelectObject(hMemDC, hBmAnd);
    GetObject(hBmAnd, sizeof(Bm), &Bm);
    x = pt.x - Bm.bmWidth / 2;
    y = pt.y - Bm.bmHeight / 2;
    BitBlt(hMemDCFrame, x, y, Bm.bmWidth, Bm.bmHeight,
      hMemDC, 0, 0, SRCAND);
    SelectObject(hMemDC, hBmXor);
    GetObject(hBmXor, sizeof(Bm), &Bm);
    BitBlt(hMemDCFrame, x, y, Bm.bmWidth, Bm.bmHeight,
      hMemDC, 0, 0, SRCINVERT); */

    hour += M_PI / 21600;
    min += M_PI / 1800;
    sec += M_PI / 30;
    GetLocalTime(&lpTime);
    hDC = GetDC(hWnd);
    //ChooseFont(&cf);
    SelectObject(hMemDCFrame, hFnt);
    TextOut(hMemDCFrame, rc.right / 2 - 120, rc.bottom - 100, Buf, sprintf(Buf, "%0.2i.%0.2i.%0.2i", lpTime.wDay, lpTime.wMonth, lpTime.wYear));
    ReleaseDC(hWnd, hDC);
    
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
  case WM_DESTROY:
    /* уничтожаем контекст в пам€ти и картинку */
    if (hBmFrame != NULL)
      DeleteObject(hBmFrame);
    if (hMemDCFrame != NULL)
      DeleteDC(hMemDCFrame);

    /* ѕослать сообщение 'WM_QUIT' с параметром 'wParam' 0 - код возврата,
     * замена вызов: PostMessage(hWnd, WM_QUIT, код_возврата, 0); */
    PostQuitMessage(30);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}