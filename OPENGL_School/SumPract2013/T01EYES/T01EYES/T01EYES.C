#include <windows.h>
#include <math.h>
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
  INT dx1,dy1,dx2,dy2;
  double  d1,d2;
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

      hBr = CreateSolidBrush(RGB(255, 255, 255));
      hOldBr = SelectObject(hDC, hBr);
      SetBkColor(hDC, RGB(0, 0, 255));
      SetBkMode(hDC, TRANSPARENT); 
      /* SetBkMode(hDC, OPAQUE); */

      hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
      hOldPen = SelectObject(hDC, hPen);

      Ellipse(hDC, 210, 320, 370, 160);
      Ellipse(hDC, 400, 320, 560, 160);

      hBr = CreateSolidBrush(RGB(0,0,0));
      hOldBr = SelectObject(hDC, hBr);

      dx1 =  x - 290;
      dy1 =  y - 240;
      d1 = sqrt((dx1*dx1)+(dy1*dy1));

      Ellipse(hDC, 275 + dx1/d1 * 60,225 + dy1/d1*60,305 + dx1/d1*60,265 + dy1/d1*60);
     
      dx2 = x - 480;
      dy2 = y - 240;
      d2 = sqrt((dx2*dx2)+(dy2*dy2));

      Ellipse(hDC, 465 + dx2/d2 * 60,225 + dy2/d2 * 60,495 + dx2/d2 * 60,265 + dy2/d2 * 60);

      SelectObject(hDC, hOldPen);                                                      
      DeleteObject(hPen);
      
      SelectObject(hDC, hOldBr);
      DeleteObject(hBr);


      SelectObject(hDC, GetStockObject(DC_PEN));
      SelectObject(hDC, GetStockObject(DC_BRUSH));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      SetDCBrushColor(hDC, RGB(155, 30, 155));
     

      

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
