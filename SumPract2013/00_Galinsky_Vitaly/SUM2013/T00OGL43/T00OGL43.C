/* FILE NAME: T03DBLBF.C
 * PROGRAMMER: VG4
 * DATE: 03.06.2013
 * PURPOSE: WinAPI double buffered output.
 */

#include <stdlib.h>
#include <math.h>

#include <windows.h>
#include "shader.h"

#define WND_CLASS_NAME "My window class"

/* Ссылки вперед */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

VOID OGL_Init( HWND hWnd );
VOID OGL_Close( VOID );
VOID OLG_Size( INT W, INT H );
VOID OGL_Copy( VOID );
VOID OGL_Draw( VOID );


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
    1024, 10, 1000, 500, NULL, NULL, hInstance, NULL);

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
  HDC hDC;
  CREATESTRUCT *cs;
  PAINTSTRUCT ps;

  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 30, 5, NULL);

    OGL_Init(hWnd);
    return 0;
  case WM_SIZE:
    OLG_Size(LOWORD(lParam), HIWORD(lParam));
    /* первичная отрисовка в новых размерах */
    OGL_Draw();
    return 0;

  case WM_TIMER:
    /* построение всего кадра */
    OGL_Draw();
    /* засыл перерисовки */
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_PAINT:
    /* отрисовка */
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    OGL_Copy();
    return 0;
  case WM_DESTROY:
    OGL_Close();
    /* Послать сообщение 'WM_QUIT' с параметром 'wParam' 0 - код возврата,
     * замена вызов: PostMessage(hWnd, WM_QUIT, код_возврата, 0); */
    PostQuitMessage(30);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/*******************************************************************************/

#pragma warning(disable: 4244 4996 4305)
#include <windows.h>

//#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

HWND hWnd;         /* Window handle */
HDC hDC;           /* Window device context handle */
HGLRC hRC;         /* Rendering context */


UINT VertArray[1], Buffers[1];
float vertices[6][3] =
{
  {-0.90, -0.90, 0 }, //Triangle 1
  { 0.85, -0.90, 0 },
  {-0.90, 0.85, 0 },
  { 0.90, -0.85, 0 }, //Triangle 2
  { 0.90, 0.90, 0 },
  {-0.85, 0.90, 0 }
};
#define NumVertices (sizeof(vertices) / sizeof(vertices[0]))

UINT program;

VOID OGL_Init( HWND hWnd )
{
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  /* Setup frame buffer */
  hDC = GetDC(hWnd);

  /*** OpenGL initialization ***/

  /* Setup pixel format */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  // pfd.cDepthBits = 24;
  // pfd.cStencilBits = 8;

  /* Set pixel format to window DC */
  i = ChoosePixelFormat(hDC, &pfd);
  DescribePixelFormat(hDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

  SetPixelFormat(hDC, i, &pfd);

  /*** Setup OpenGL ***/
  hRC = wglCreateContext(hDC);
  wglMakeCurrent(hDC, hRC);

  if (glewInit() != GLEW_OK)
    return;
  if (!(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
    return;

  glGenVertexArrays(1, VertArray);
  glBindVertexArray(VertArray[0]);

  glGenBuffers(1, Buffers);
  glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  program = ShadProgInit("cgsg.glslv", "cgsg.glslf");
  glUseProgram(program);

#define vPosition 0  
#define BUFFER_OFFSET(offset) ((void*)(offset))
  glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vPosition);
  //glVertexAttribPointer(vPosition, 2,GL_FLOAT, GL_FALSE, 0,BUFFER_OFFSET(0));
  //glEnableVertexAttribArray(vPosition);
  //--------------------------------
} /* End of 'OGL_Init' function */

VOID OGL_Close( VOID )
{
  /* Remove contexts */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hRC);
  ReleaseDC(hWnd, hDC);
} /* End of 'OGL_Close' function */

VOID OLG_Size( INT W, INT H )
{
  glViewport(0, 0, W, H);
} /* End of 'OGL_Size' function */


VOID OGL_Copy( VOID )
{
  SwapBuffers(hDC);
} /* End of 'OGL_Copy' function */

VOID OGL_Draw( VOID )
{
  int LocColor;
  BYTE a[4][4] =
  {
    255, 0, 255, 0,
    0, 255, 0, 255,
    255, 0, 255, 0,
    0, 255, 0, 255,
  };

  glClear(GL_COLOR_BUFFER_BIT);


  glUseProgram(program);
  LocColor = glGetUniformLocation(program, "color30");
  glUniform4d(LocColor, 0.5, 0.7, 1, 1);
  glBindVertexArray(VertArray[0]);

  glRectd(0, 0, 1, 1);
  
  glUseProgram(program);
  glDrawArrays(GL_TRIANGLES, 0, NumVertices);

  glUseProgram(0);
  glPixelZoom(30, 30);
  glDrawPixels(4, 4, GL_LUMINANCE, GL_UNSIGNED_BYTE, a);

  glFinish();
} /* End of 'OGL_Draw' function */

/* END OF 'T03DBLBF.C' FILE */
