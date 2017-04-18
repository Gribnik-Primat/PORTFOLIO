/* FILE NAME: T03DBLBF.C
 * PROGRAMMER: VG4
 * DATE: 05.06.2013
 * PURPOSE: WinAPI double buffered output. Globe drawing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <windows.h>

#define WND_CLASS_NAME "My window class"

#pragma warning(disable: 4244)

/* Globe grid size */
#define GW (59 * 8)
#define GH (30 * 8)
BYTE GlobeColor[GH][GW][4];
struct
{
  DOUBLE X, Y, Z;
} GlobePos[GH][GW];

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
    1024, 0, 1000, 500, NULL, NULL, hInstance, NULL);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return msg.wParam;
} /* End of 'WinMain' function */

/***
 * Cow support
 ***/

typedef DOUBLE DBL;
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

/* Vertex array */
VEC *Vertexes, *VertexesProj;
INT NumOfVertexes;

/* Facet array */
INT (*Facets)[3];
INT NumOfFacets;

/* Load cow function */
VOID LoadCow( VOID )
{
  FILE *F;
  INT fn = 0, vn = 0;
  static CHAR Buf[1000];

  if ((F = fopen("cow_new1.object", "rt")) == NULL)
    return;

  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      vn++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      fn++;

  if ((Vertexes = malloc(2 * sizeof(VEC) * vn)) == NULL)
  {
    fclose(F);
    return;
  }
  if ((Facets = malloc(sizeof(INT [3]) * fn)) == NULL)
  {
    free(Vertexes);
    fclose(F);
    return;
  }
  NumOfVertexes = vn;
  NumOfFacets = fn;
  VertexesProj = Vertexes + NumOfVertexes;

  vn = 0;
  fn = 0;
  rewind(F);
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      Vertexes[vn].X = x;
      Vertexes[vn].Y = y;
      Vertexes[vn].Z = z;
      vn++;
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n1, n2, n3;

      sscanf(Buf + 2, "%d%d%d", &n1, &n2, &n3);
      Facets[fn][0] = n1 - 1;
      Facets[fn][1] = n2 - 1;
      Facets[fn][2] = n3 - 1;
      fn++;
    }

  fclose(F);
} /* End of 'LoadCow' function */

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
  INT i, j, x, y, n, N = 5 * 59, M = 5 * 111;
  DOUBLE scale = 1;
  HDC hDC;
  CREATESTRUCT *cs;
  PAINTSTRUCT ps;
  BITMAPINFOHEADER bih;
  LONG tim;
  HDC hMemDC;
  HBITMAP hBmGlobe;
  BITMAP BmGlobe;
  static DWORD *Bits;
  static INT WinW, WinH;
  static HDC hMemDCFrame;
  static HBITMAP hBmFrame;
  static HFONT hFnt;

  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 30, 5, NULL);
    LoadCow();

    hBmGlobe = LoadImage(NULL, "globe.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmGlobe, sizeof(BmGlobe), &BmGlobe);

    hFnt = CreateFont(-30, 0, 0, 0, FW_BOLD,
      TRUE, FALSE, FALSE, RUSSIAN_CHARSET,
      OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
      VARIABLE_PITCH | FF_ROMAN, "Bookman Old Style");

    /*** создаем контекст в памяти ***/
    /* определяем контект окна */
    hDC = GetDC(hWnd);
    /* создаем контекст в памяти, совместимый с контекстом окна */
    hMemDCFrame = CreateCompatibleDC(hDC);

    /* сохраняем цвет планеты */
    hMemDC = CreateCompatibleDC(hDC);
    SelectObject(hMemDC, hBmGlobe);
    for (i = 0; i < GH; i++)
      for (j = 0; j < GW; j++)
      {
        COLORREF c = GetPixel(hMemDC, j * BmGlobe.bmWidth / GW, i * BmGlobe.bmHeight / GH);
        BYTE
          r = GetRValue(c),
          g = GetGValue(c),
          b = GetBValue(c);

        GlobeColor[i][j][0] = b;
        GlobeColor[i][j][1] = g;
        GlobeColor[i][j][2] = r;
      }
    DeleteObject(hBmGlobe);
    DeleteDC(hMemDC);

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
    SelectObject(hMemDCFrame, GetStockObject(DC_BRUSH));
    SelectObject(hMemDCFrame, GetStockObject(NULL_PEN));
    SetDCBrushColor(hMemDCFrame, RGB(30, 30, 30));
    Rectangle(hMemDCFrame, 0, 0, WinW, WinH);

    /* рисуем звезды */
    srand(30);
    n = 30000; 
    while (n-- > 0)
    {
      x = rand() % WinW;
      y = rand() % WinH;
      Bits[y * WinW + x] = RGB(rand() % 256, rand() % 256, rand() % 256);
    }

    /* строим сетку */
    tim = clock();
    for (i = 0; i < GH; i++)
    {
      DOUBLE pi = 3.14159265358979323846;
      DOUBLE theta = i * pi / (GH - 1);

      for (j = 0; j < GW; j++)
      {
        DOUBLE t = tim / (DOUBLE)CLOCKS_PER_SEC, phi = j * 2 * pi / (GW - 1) + t / 5;
        DOUBLE
          r = (WinW > WinH ? WinH : WinW) / 2.30,
          x = r * sin(theta) * sin(phi),
          y = r * cos(theta),
          z = r * sin(theta) * cos(phi),
          a = (59 + sin(t) * 30) * pi / 180, z1, y1;


        y1 = y * cos(a) - z * sin(a);
        z1 = y * sin(a) + z * cos(a);
        z = z1;
        y = y1;

        GlobePos[i][j].X = WinW / 2 + x;
        GlobePos[i][j].Y = WinH / 2 - y;
        GlobePos[i][j].Z = z;
      }
    }

    /* рисуем сетку */
    SelectObject(hMemDCFrame, GetStockObject(DC_BRUSH));
    /*
    if ((INT)(tim / (DOUBLE)CLOCKS_PER_SEC) % 2)
      SelectObject(hMemDCFrame, GetStockObject(DC_PEN));
    else
    */
    SelectObject(hMemDCFrame, GetStockObject(NULL_PEN));
    SetDCPenColor(hMemDCFrame, RGB(255, 255, 255));
    SetDCBrushColor(hMemDCFrame, RGB(0, 0, 0));
    //SelectObject(hMemDCFrame, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDCFrame, RGB(255, 255, 255));
    for (i = 0; i < GH - 1; i++)
    {
      for (j = 0; j < GW - 1; j++)
      {
        if (GlobePos[i][j].Z > 0 && GlobePos[i][j + 1].Z > 0 && 
            GlobePos[i + 1][j].Z > 0 && GlobePos[i + 1][j + 1].Z > 0)
        {
          static POINT pts[4];

          pts[0].x = GlobePos[i][j].X;
          pts[0].y = GlobePos[i][j].Y;

          pts[1].x = GlobePos[i][j + 1].X;
          pts[1].y = GlobePos[i][j + 1].Y;

          pts[2].x = GlobePos[i + 1][j + 1].X;
          pts[2].y = GlobePos[i + 1][j + 1].Y;

          pts[3].x = GlobePos[i + 1][j].X;
          pts[3].y = GlobePos[i + 1][j].Y;

          SetDCBrushColor(hMemDCFrame, RGB(GlobeColor[i][j][2],
                                           GlobeColor[i][j][1],
                                           GlobeColor[i][j][0]));
          /// SetDCBrushColor(hMemDCFrame, RGB(0, 100, 0));
          Polygon(hMemDCFrame, pts, 4);
        }
      }
    }

    SelectObject(hMemDCFrame, GetStockObject(NULL_BRUSH));
    SelectObject(hMemDCFrame, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDCFrame, RGB(255, 0, 155));
    SetDCBrushColor(hMemDCFrame, RGB(0, 111, 0));
    scale = 59;
    /* calculate projection */
    for (i = 0; i < NumOfVertexes; i++)
    {
      DOUBLE a = tim * 0.00030;
      VertexesProj[i].X = WinW / 2 + scale * (Vertexes[i].X * cos(a) - Vertexes[i].Z * sin(a));
      VertexesProj[i].Y = WinH / 2 - scale * (Vertexes[i].Y);
      VertexesProj[i].Z = scale * (Vertexes[i].X * sin(a) + Vertexes[i].Z * cos(a));
    }

    for (i = 0; i < NumOfFacets; i++)
    {
      POINT p[3];

      for (j = 0; j < 3; j++)
      {
        p[j].x = VertexesProj[Facets[i][j]].X;
        p[j].y = VertexesProj[Facets[i][j]].Y;
      }

      Polygon(hMemDCFrame, p, 3);
    }

    for (i = 0; i < NumOfVertexes; i++)
    {
      INT
        x = VertexesProj[i].X;
        y = VertexesProj[i].Y;
      SetPixelV(hMemDCFrame, x, y, RGB(255, 255, 255));
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
