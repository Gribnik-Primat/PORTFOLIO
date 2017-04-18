/* FILE NAME: MAIN.C
 * PROGRAMMER: IK1
 * DATE: 05.06.2013
 * PURPOSE: Vector and matrix library.
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#define WND_CLASS_NAME "My window class"
#include "VEC.H"

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
      Vertexes[vn] = VecSet(x, y, z);
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
  INT x, y, i, j, scale=59;
  HDC hDC;
  CREATESTRUCT *cs;
  PAINTSTRUCT ps;
  POINT pt;
  BITMAP Bm;
  DOUBLE tim;
  static CHAR BUF[100];
  static INT WinW, WinH;
  static HDC hMemDCFrame, hMemDC;
  static HBITMAP hBmFrame, hBmBack, hBmAnd, hBmXor;
  static POINT pts[3] =
  {
    {0, 0}, {100, 30}, {30, 59}
  };
  MATR M =
  {
    {
      {1, 2, 3},
      {0, 5, 3},
      {0, 0, 3}
    }
  };
 switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 30, 5, NULL);

    LoadCow();

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

    SelectObject(hMemDCFrame, GetStockObject(DC_BRUSH));
    SelectObject(hMemDCFrame, GetStockObject(NULL_PEN));

    SetDCBrushColor(hMemDCFrame, RGB(255, 255, 255));
    Rectangle(hMemDCFrame, 0, 0, WinW + 1, WinH + 1);
    SelectObject(hMemDCFrame, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDCFrame, RGB(255, 255, 255));

    GetObject(hBmBack, sizeof(Bm), &Bm);
    SelectObject(hMemDC, hBmBack);
    StretchBlt(hMemDCFrame, 0, 0, WinW, WinH,hMemDC, 0, 0, Bm.bmWidth, Bm.bmHeight, SRCCOPY);
    SelectObject(hMemDCFrame, GetStockObject(DC_BRUSH));  
    SelectObject(hMemDCFrame, GetStockObject(NULL_PEN));
    SetDCBrushColor(hMemDCFrame, RGB(0, 0, 0));
    Rectangle(hMemDCFrame, 0, 0, WinW, WinH);
    tim = clock();
    SelectObject(hMemDCFrame, GetStockObject(NULL_BRUSH));
    SelectObject(hMemDCFrame, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDCFrame, RGB(255, 0, 155));
    SetDCBrushColor(hMemDCFrame, RGB(0, 111, 0));
    scale = 59;
    /* calculate projection */
    for (i = 0; i < NumOfVertexes; i++)
    {
      DOUBLE a = tim * 0.00030;
      VertexesProj[i] = VecSet(WinW / 2 +  Vertexes[i].X, WinH / 2 -  Vertexes[i].Y, Vertexes[i].Z);
      VertexesProj[i] = VecMulMatr( VertexesProj[i], MatrScale(30, 30, 30));
      /*VertexesProj[i].X = WinW / 2 +  Vertexes[i].X ;
      VertexesProj[i].Y = WinH / 2 -  Vertexes[i].Y;
      VertexesProj[i].Z = scale * (Vertexes[i].X * sin(a) + Vertexes[i].Z * cos(a));
      */
    }

    for (i = 0; i < NumOfVertexes; i++)
    {
      DOUBLE a = tim * 0.00030;
     /* VertexesProj[i] = VecSet(WinW / 2 +  Vertexes[i].X, WinH / 2 -  Vertexes[i].Y, Vertexes[i].Z); */
      VertexesProj[i] = VecMulMatr( VertexesProj[i], MatrScale(2, 2, 2));
      /*VertexesProj[i].X = WinW / 2 +  Vertexes[i].X ;
      VertexesProj[i].Y = WinH / 2 -  Vertexes[i].Y;
      VertexesProj[i].Z = scale * (Vertexes[i].X * sin(a) + Vertexes[i].Z * cos(a));
      */
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

    /* Послать сообщение 'WM_QUIT' с параметром 'wParam' 0 - код возврата,
     * замена вызов: PostMessage(hWnd, WM_QUIT, код_возврата, 0); */
    PostQuitMessage(30);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */


/* END OF 'MAIN.C' FILE */

