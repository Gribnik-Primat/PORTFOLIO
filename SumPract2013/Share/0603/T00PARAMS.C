#include <windows.h>

#define WND_CLASS_NAME "My window class"

/* Ссылки вперед */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK MouseHook( INT Code, WPARAM wParam, LPARAM lParam );

/* Глобальные переменные */
HWND hWndMain;
INT GlobalX, GlobalY;


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

  hWndMain = CreateWindowA(WND_CLASS_NAME, "> 30 <",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
    10, 10, 1000, 500, NULL, NULL, hInstance, NULL);

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
  POINT pt;
  HDC hDC;
  CHAR Buf[100];
  CREATESTRUCT *cs;
  PAINTSTRUCT ps;
  static INT x, y, w, h;

  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    /* SetTimer(hWnd, 30, 500, NULL); */
    SetWindowsHookEx(WH_MOUSE_LL, MouseHook, cs->hInstance, 0);
    return 0;
  case WM_TIMER:
    //GetCursorPos(&pt);
    pt.x = GlobalX;
    pt.y = GlobalY;
    ScreenToClient(hWnd, &pt);
    x = pt.x;
    y = pt.y;

    wsprintf(Buf, "%5i|%5i", pt.x, pt.y);
    SetWindowText(hWnd, Buf);
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
  case WM_ERASEBKGND:
    hDC = (HDC)wParam;
    SelectObject(hDC, GetStockObject(NULL_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(130, 130, 130));
    Rectangle(hDC, 0, 0, w + 1, h + 1);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);

    SelectObject(hDC, GetStockObject(DC_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCPenColor(hDC, RGB(0, 140, 0));
    SetDCBrushColor(hDC, RGB(155, 30, 155));
    RoundRect(hDC, x + 30, y + 30, x + 59, y + 59, 15, 15);

    SetDCPenColor(hDC, RGB(0, 255, 255));
    MoveToEx(hDC, x - 59, y - 59, NULL);
    LineTo(hDC, x + 59, y + 59);
    MoveToEx(hDC, x + 59, y - 59, NULL);
    LineTo(hDC, x - 59, y + 59);

    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    /* Послать сообщение 'WM_QUIT' с параметром 'wParam' 0 - код возврата,
     * замена вызов: PostMessage(hWnd, WM_QUIT, код_возврата, 0); */
    PostQuitMessage(30);
    /* KillTimer(hWnd, 30); */
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* Функция обработки мышинных сообщений.
 * АРГУМЕНТЫ:
 *   - код сообщения:
 *       INT Code;
 *   - параметр сообшения ('word parameter'):
 *       WPARAM wParam;
 *   - параметр сообшения ('long parameter'):
 *       LPARAM lParam;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (LRESULT) - в зависимости от сообщения.
 */
LRESULT CALLBACK MouseHook( INT Code, WPARAM wParam, LPARAM lParam )
{
  MSLLHOOKSTRUCT *hs = (MSLLHOOKSTRUCT *)lParam;

  switch (wParam)
  {
  case WM_MOUSEMOVE:
    GlobalX = hs->pt.x;
    GlobalY = hs->pt.y;
    SendMessage(hWndMain, WM_TIMER, 0, 0);
    return 0;
  }
  return 0;
} /* End of 'MouseHook' function */

/* END OF 'T00PARAMS.C' FILE */
