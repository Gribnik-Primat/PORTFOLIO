/* FILE NAME: T03DBLBF.C
 * PROGRAMMER: VG4
 * DATE: 03.06.2013
 * PURPOSE: WinAPI double buffered output.
 */

#include <windows.h>

#include <time.h>

#include <math.h>

#include <stdio.h>

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
    10, 10, 750, 750, NULL, NULL, hInstance, NULL);


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
  
  INT x,y,z,i,j,w=0,h=0;
  double theta,phi,phaseshift=0;
  HDC hDC;
  HPEN hPen, hOldPen;
  HBRUSH hBr, hOldBr;
  CREATESTRUCT *cs;
  PAINTSTRUCT ps;
  POINT pt;
  BITMAP Bm;
  BYTE r,g,b;
  static INT WinW, WinH;
  static HDC hMemDCFrame, hMemDC;
  static HBITMAP hBmFrame, hBmBack, hBmAnd, hBmXor;
  SYSTEMTIME st;
  COLORREF c; 


  
  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 30, 5, NULL);
                                                                      
    hBmBack = LoadImage(NULL, "globe.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);


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

    GetLocalTime(&st);

    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SelectObject(hMemDC, GetStockObject(NULL_PEN));

    SetDCBrushColor(hMemDC, RGB(50, 150, 200));
    Rectangle(hMemDC, 0, 0, WinW + 1, WinH + 1);
    SelectObject(hMemDC, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDC, RGB(255, 250, 250));

    
    
    for(i = 0; i <= WinW;i++)
      for(j = 0;j < WinH;j++)
      {    
        theta = i*3.14/WinW;
        phi = j*2*3.14/WinH+phaseshift; 
      
        x = (INT) 350*sin(theta)*cos(phi)+WinW/2;
        y = (INT) 350*sin(theta)*sin(phi)+WinW/2;
        z = (INT) 350*cos(theta);
        
        
        
        if(z>0) 
        {      
          SelectObject(hMemDC,hBmBack);
          c = GetPixel(hMemDC,x,y);
          
          r = GetRValue(c);
          g = GetGValue(c);
          b = GetBValue(c);
          
          SetPixel(hMemDCFrame,x,y,RGB(r,g,b));
        }
        phaseshift += 1*st.wSecond;
      }

  

   
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

    /* Послать сообщение 'WM_QUIT' с параметром 'wParam' 0 - код возврата,
     * замена вызов: PostMessage(hWnd, WM_QUIT, код_возврата, 0); */
    PostQuitMessage(30);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */


/* END OF 'T03DBLBF.C' FILE */
