/*Boikov Ivan 10-1*/
#include <stdio.h>
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define WND_CLASS_NAME "My window class"

#define N 300
#define M 600

POINT pts[4];

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  INT iWidth, iHeight;

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
  static BYTE *mem;
  FILE *F;
  static DOUBLE hour = 0, min = 0, sec = 0, phi, theta, PhaseShift = 0, R, z, GlobePos[N][M][3];
  RECT rc;
  static INT WinW, WinH, w, h, x, i, y, j;
  static HDC hMemDCFrame, hMemDC;
  static HBITMAP hBmFrame, hBmBack;
  static INT x1 = 0, y1 = 0;

  GetClientRect(hWnd, &rc);

  switch (Msg)                              
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 30, 1, NULL);

    hDC = GetDC(hWnd);
    hMemDCFrame = CreateCompatibleDC(hDC);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    F = fopen("GLOBE.g24", "rb");
    fread(&w, 2, 1, F);
    fread(&h, 2, 1, F);
    mem = malloc(w * h * 3);
    fread(mem, 3, w * h, F);
    fclose(F);

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
    SelectObject(hMemDCFrame, GetStockObject(DC_BRUSH));
    SelectObject(hMemDCFrame, GetStockObject(NULL_PEN));
    SetDCBrushColor(hMemDCFrame, RGB(0, 0, 0));
                                                                   
    Rectangle(hMemDCFrame, 0, 0, WinW, WinH);
    srand(30);
    for (i = 0; i < 255; i++) 
      SetPixelV(hMemDCFrame, rand()%rc.right, rand() % rc.bottom, RGB(255, 255, 255));
    for (i = 0; i < N -1; i++)
      for (j = 0; j <= M; j++)
      {     

         theta = (i * M_PI) / N;
         phi = (j * 2 * M_PI) / M + PhaseShift;
         R = (WinW > WinH ? WinH : WinW) / 2;

         GlobePos[i][j][0] = (int)(sin(theta) * sin(phi) * R) + WinW / 2;
         GlobePos[i][j][1] = -(int)(cos(theta) * R) + WinH / 2;
         GlobePos[i][j][2] = cos(phi) * sin(theta);      
      }     
    for (i = 0; i < N; i++)
      for (j = 0; j <= M; j++)
      {

        pts[0].x = (LONG)(GlobePos[i][j][0]);
        pts[0].y = (LONG)(GlobePos[i][j][1]);

        pts[1].x = (LONG)(GlobePos[i][j + 1][0]);
        pts[1].y = (LONG)(GlobePos[i][j + 1][1]);

        pts[2].x = (LONG)(GlobePos[i + 1][j + 1][0]);
        pts[2].y = (LONG)(GlobePos[i + 1][j + 1][1]);

        pts[3].x = (LONG)(GlobePos[i + 1][j][0]);
        pts[3].y = (LONG)(GlobePos[i + 1][j][1]);

        if (GlobePos[i][j][2] > 0 && GlobePos[i][j + 1][2] > 0 && GlobePos[i + 1][j + 1][2] > 0 && GlobePos[i + 1][j][2] > 0 )
        {
          x1 = j * w / (M + 1);
          y1 = i * h / (N + 1);
          SetDCBrushColor(hMemDCFrame, RGB(mem[(y1 * w + x1) * 3 + 2], mem[(y1 * w + x1) * 3 + 1], mem[(y1 * w + x1) * 3]));
          Polygon(hMemDCFrame, pts, sizeof(pts) / sizeof(pts[0]));
        }
      }
    PhaseShift += 0.001;

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
    /* ѕослать сообщение 'WM_QUIT' с параметром 'wParam' 0 - код возврата,
     * замена вызов: PostMessage(hWnd, WM_QUIT, код_возврата, 0); */
    PostQuitMessage(30);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}