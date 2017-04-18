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

  hWnd = CreateWindowA(WND_CLASS_NAME, "> 30 <",
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
  INT x, y;
  CHAR Buf[100];

  switch (Msg)
  {
  case  WM_LBUTTONDOWN:
    SetCapture(hWnd);
    return 0;
  case WM_LBUTTONUP:
    SetCapture(NULL);
    return 0;
  case WM_MOUSEMOVE:
    x = (SHORT)(lParam & 0xFFFF);
    y = (SHORT)((lParam >> 16) & 0xFFFF);
    wsprintf(Buf, "%5i|%5i  %X", x, y, wParam);
    if (wParam & MK_LBUTTON)
    {
      HDC hDC;
      HPEN hPen, hOldPen;
      HBRUSH hBr, hOldBr;

      hDC = GetDC(hWnd);

      /* hBr = CreateSolidBrush(RGB(255, 255, 0)); */
      hBr = CreateHatchBrush(HS_DIAGCROSS, RGB(255, 255, 0));
      hOldBr = SelectObject(hDC, hBr);
      SetBkColor(hDC, RGB(0, 0, 255));
      SetBkMode(hDC, TRANSPARENT);
      /* SetBkMode(hDC, OPAQUE); */

      hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
      hOldPen = SelectObject(hDC, hPen);

      Ellipse(hDC, x - 30, y - 30, x + 30, y + 30);

      SelectObject(hDC, hOldPen);
      DeleteObject(hPen);
      
      SelectObject(hDC, hOldBr);
      DeleteObject(hBr);


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


      ReleaseDC(hWnd, hDC);
    }
    SetWindowText(hWnd, Buf);
    return 0;
  case WM_DESTROY:
    /* Послать сообщение 'WM_QUIT' с параметром 'wParam' 0 - код возврата,
     * замена вызов: PostMessage(hWnd, WM_QUIT, код_возврата, 0); */
    PostQuitMessage(30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'T00PARAMS.C' FILE */
