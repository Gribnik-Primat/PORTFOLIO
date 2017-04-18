/* FILE NAME: T03DBLBF.C
 * PROGRAMMER: VG4
 * DATE: 03.06.2013
 * PURPOSE: WinAPI double buffered output.
 */

#include <stdlib.h>
#include <math.h>

#include <windows.h>

#define WND_CLASS_NAME "My window class"

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

  hWnd = CreateWindowA(WND_CLASS_NAME, "> 30 <",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
    10, 10, 1000, 500, NULL, NULL, hInstance, NULL);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return msg.wParam;
} /* End of 'WinMain' function */

/* Функция отрисовки стрелки.
 * АРГУМЕНТЫ:
 *   - дескриптор контекста окна:
 *       HDC hDC;
 *   - координаты центра:
 *       INT Xc, Yc;
 *   - угол в градусах:
 *       INT Angle;
 *   - утолщение по ширине и высоте:
 *       DOUBLE ScaleX, ScaleY;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DrawArrow( HDC hDC, INT Xc, INT Yc, INT Angle, DOUBLE ScaleX, DOUBLE ScaleY )
{
  INT i;
  POINT pts[] =
  {
    {1, 0}, {1, 4}, {2, 4}, {0, 6}, {-2, 4}, {-1, 4}, {-1, 0} 
  };
  DOUBLE
    a = Angle * 3.14159265358979323846 / 180,
    si = sin(a), co = cos(a);

  for (i = 0; i < sizeof(pts) / sizeof(pts[0]); i++)
  {
    INT
      x = ScaleX * pts[i].x * co - ScaleY * pts[i].y * si + Xc,
      y = -(ScaleX * pts[i].x * si + ScaleY * pts[i].y * co) + Yc;

    pts[i].x = x;
    pts[i].y = y;
  }
  Polygon(hDC, pts, sizeof(pts) / sizeof(pts[0]));

} /* End of 'DrawArrow' function */

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
  INT x, y, n;
  HDC hDC;
  CREATESTRUCT *cs;
  PAINTSTRUCT ps;
  POINT pt;
  BITMAP Bm;
  SYSTEMTIME st;
  RECT rc;
  BITMAPINFOHEADER bih;
  static CHAR Buf[1000];
  static DWORD *Bits;
  static INT WinW, WinH;
  static HDC hMemDCFrame, hMemDC;
  static HBITMAP hBmFrame, hBmBack, hBmAnd, hBmXor;
  static HFONT hFnt;
  static CHOOSEFONT cf;
  static LOGFONT logfnt;
  static POINT pts[3] =
  {
    {0, 0}, {100, 30}, {30, 59}
  };

  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 30, 5, NULL);

    hBmBack = LoadImage(NULL, "cf.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAnd = LoadImage(NULL, "and.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmXor = LoadImage(NULL, "xor.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    cf.lStructSize = sizeof(cf);
    /*
    cf.Flags = 0;
    cf.hwndOwner = NULL;
    cf.lpLogFont = &logfnt;
    ChooseFont(&cf);

    hFnt = CreateFontIndirect(&cf);
    */
    logfnt.lfCharSet = RUSSIAN_CHARSET;
    hFnt = CreateFont(-30, 0, 0, 0, FW_BOLD,
      TRUE, FALSE, FALSE, RUSSIAN_CHARSET,
      OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
      VARIABLE_PITCH | FF_ROMAN, "Bookman Old Style");

    /*** создаем контекст в памяти ***/
    /* определяем контект окна */
    hDC = GetDC(hWnd);
    /* создаем контекст в памяти, совместимый с контекстом окна */
    hMemDCFrame = CreateCompatibleDC(hDC);
    hMemDC = CreateCompatibleDC(hDC);
    SetPixelV(hMemDC, 100, 100, RGB(0, 0, 0));
    /* убираем контекст окна */
    ReleaseDC(hWnd, hDC);

    return 0;
  case WM_SIZE:
    WinW = LOWORD(lParam);
    WinH = HIWORD(lParam);

    /*** выделяем картинку (уничтожая старую) ***/
    if (hBmFrame != NULL)
      DeleteObject(hBmFrame);
    /* создаем в памяти изображение, совместимое с контектом окна */
    hDC = GetDC(hWnd);
    /*
    hBmFrame = CreateCompatibleBitmap(hDC, WinW, WinH);
    */
    /* создаем изображение заданной структуры */
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = WinW;
    bih.biHeight = -WinH;
    bih.biPlanes = 1;    /* всегда */
    bih.biBitCount = 32; /* xRGB */
    bih.biCompression = BI_RGB;
    bih.biSizeImage = WinW * WinH * 4;
    bih.biClrImportant = 0;
    bih.biClrUsed = 0;
    bih.biXPelsPerMeter = bih.biYPelsPerMeter = 0;

    hBmFrame = CreateDIBSection(NULL, (BITMAPINFO *)&bih, DIB_RGB_COLORS,
      (VOID *)&Bits, NULL, 0);

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

    SetROP2(hMemDCFrame, R2_XORPEN);
    MoveToEx(hMemDCFrame, 0, 0, NULL);
    LineTo(hMemDCFrame, WinW, WinH);
    MoveToEx(hMemDCFrame, WinW - 1, 0, NULL);
    LineTo(hMemDCFrame, 0, WinH);
    SetROP2(hMemDCFrame, R2_COPYPEN);

    SetDCBrushColor(hMemDCFrame, RGB(50, 250, 30));
    SetDCPenColor(hMemDCFrame, RGB(255, 0, 0));
    Polygon(hMemDCFrame, pts, 3);

    srand(30);
    n = 30000; 
    while (n-- > 0)
    {
      x = rand() % WinW;
      y = rand() % WinH;
      Bits[y * WinW + x] = RGB(rand() % 256, rand() % 256, rand() % 256);
    }

    GetLocalTime(&st);
    if ((st.wMilliseconds / 500) & 1)
    {   
      /* вывод маскированной картинки */
      SelectObject(hMemDC, hBmAnd);
      GetObject(hBmAnd, sizeof(Bm), &Bm);
      x = pt.x - Bm.bmWidth / 2;
      y = pt.y - Bm.bmHeight / 2;
      BitBlt(hMemDCFrame, x, y, Bm.bmWidth, Bm.bmHeight,
        hMemDC, 0, 0, SRCAND);
      SelectObject(hMemDC, hBmXor);
      GetObject(hBmXor, sizeof(Bm), &Bm);
      BitBlt(hMemDCFrame, x, y, Bm.bmWidth, Bm.bmHeight,
        hMemDC, 0, 0, SRCINVERT);
    }

    SetBkMode(hMemDCFrame, TRANSPARENT);
    SetTextColor(hMemDCFrame, RGB(200, 150, 100));
    SelectObject(hMemDCFrame, hFnt);
    SetRect(&rc, 0, 0, WinW, WinH);
    DrawText(hMemDCFrame, Buf,
      wsprintf(Buf, "ABC\nkjhksdjh\nersdf\n"),
      &rc, DT_SINGLELINE | DT_BOTTOM | DT_CENTER);

    DrawArrow(hMemDCFrame, WinW / 2, WinH / 2, 360 / 10 * st.wMilliseconds / 100, 5, 30);
    DrawArrow(hMemDCFrame, WinW / 2, WinH / 2, -6 * st.wSecond, 10, 10);

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
    /* уничтожаем контекст в памяти и картинку */
    if (hBmFrame != NULL)
      DeleteObject(hBmFrame);
    if (hMemDCFrame != NULL)
      DeleteDC(hMemDCFrame);

    DeleteObject(hFnt);

    /* Послать сообщение 'WM_QUIT' с параметром 'wParam' 0 - код возврата,
     * замена вызов: PostMessage(hWnd, WM_QUIT, код_возврата, 0); */
    PostQuitMessage(30);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */


/* END OF 'T03DBLBF.C' FILE */
