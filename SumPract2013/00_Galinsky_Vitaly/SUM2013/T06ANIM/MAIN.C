/* FILE NAME: MAIN.C
 * PROGRAMMER: VG4
 * DATE: 07.06.2013
 * PURPOSE: Main math support implementation module.
 */

#include <stdlib.h>

#include "anim.h"
#include "units.h"

/* Глобальные определения */
#define WND_CLASS_NAME "My window class"
#define ANIMATION_TIMER 30

/* Функция обработки сообщения окна */
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
  INT i, j;
  WNDCLASSEX wc;
  MSG msg;
  HWND hWnd;

  for (i = -1; i <= 1; i++)
    for (j = -1; j <= 1; j++)
      VG4_AnimAdd(CowCreate(i * 8 , 0, j * 8));
  VG4_AnimAdd(CubeCreate(30, 30));

  /*
  for (i = 0; i < 100; i++)
  {
    VG4_AnimAdd(CubeCreate(rand() % 500, rand() % 500));
  }
  */

  wc.cbSize = sizeof(WNDCLASSEX); /* Размер структуры для совместимости */
  wc.style = CS_VREDRAW | CS_HREDRAW; /* Стиль окна: полностью перерисовывать
                                       * при изменении вертикального или
                                       * горизонтального размеров (еще CS_DBLCLKS) */
  wc.cbClsExtra = 0; /* Дополнительное количество байт для класса */
  wc.cbWndExtra = 0; /* Дополнительное количество байт для окна */
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW; /* Фоновый цвет - выбранный в системе */
  wc.hCursor = LoadCursor(NULL, IDC_ARROW); /* Загрузка курсора (системного) */
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); /* Загрузка пиктограммы (системной) */
  wc.hIconSm = LoadIcon(NULL, IDI_EXCLAMATION); /* Загрузка малой пиктограммы (системной) */
  wc.lpszMenuName = NULL; /* Имя ресурса меню */
  wc.hInstance = hInstance; /* Дескриптор приложения, регистрирующего класс */
  wc.lpfnWndProc = MyWindowFunc; /* Указатель на функцию обработки */
  wc.lpszClassName = WND_CLASS_NAME; /* Имя класса */

  /* Регистрация класса в системе */
  if (!RegisterClassEx(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindowA(WND_CLASS_NAME, "> 30 <",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  while (TRUE)
  {
    /* смотрим, есть ли сообщение и вытаскиваем очередное */
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      /* выход по WM_QUIT */
      if (msg.message == WM_QUIT)
        break;
      /* обрабатываем как всегда */
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
    {
      /* состояние Idle */
      SendMessage(hWnd, WM_TIMER, ANIMATION_TIMER, 0);
    }
  }

  return 0;
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
  INT w, h;
  HDC hDC;
  CREATESTRUCT *cs;
  PAINTSTRUCT ps;

  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    if (!VG4_AnimInit(hWnd))
      return -1;
    SetTimer(hWnd, ANIMATION_TIMER, 5, NULL);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    VG4_AnimResize(w, h);
    return 0;
  case WM_MOUSEWHEEL:
    VG4_MouseWheel = (SHORT)HIWORD(wParam);
    return 0;
  case WM_TIMER:
    VG4_AnimRender();
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    VG4_AnimCopyFrame();
    return 0;
  case WM_DESTROY:
    VG4_AnimClose();
    /* Послать сообщение 'WM_QUIT' с параметром 'wParam' 0 - код возврата,
     * замена вызов: PostMessage(hWnd, WM_QUIT, код_возврата, 0); */
    PostQuitMessage(30);
    KillTimer(hWnd, ANIMATION_TIMER);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'MAIN.C' FILE */
