DEF.H фрагмент:
#include <windows.h>
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32")

#pragma warning(disable: 4305 4244 4996)


Init:
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  VG4_Anim.hWnd = hWnd;
  VG4_Anim.hDC = GetDC(hWnd);

  /*** инициализация OpenGL ***/
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 0;
  pfd.cStencilBits = 0;

  i = ChoosePixelFormat(VG4_Anim.hDC, &pfd);
  DescribePixelFormat(VG4_Anim.hDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(VG4_Anim.hDC, i, &pfd);

  VG4_Anim.hGLRC = wglCreateContext(VG4_Anim.hDC);
  wglMakeCurrent(VG4_Anim.hDC, VG4_Anim.hGLRC);

  /* установка базовых параметров OpenGL */
  if (glewInit() != GLEW_OK)
  {
    ReleaseDC(VG4_Anim.hWnd, VG4_Anim.hDC);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(VG4_Anim.hGLRC);
    memset(&VG4_Anim, 0, sizeof(VG4_Anim));
    return FALSE;
  }

  glClearColor(0.3, 0.5, 0.7, 1);

Close:

  ReleaseDC(VG4_Anim.hWnd, VG4_Anim.hDC);
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(VG4_Anim.hGLRC);


Resize:

  /* Изменение размера зоны вывода */
  glViewport(0, 0, W, H);

Render:

  /* Инициализация кадра */
  glClear(GL_COLOR_BUFFER_BIT);

  /* Посылка всем объектам анимации сигнала перерисовки */
  for (i = 0; i < NumOfUnits; i++)
  {
    Units[i]->Render(Units[i], &***_Anim);
  }
  /* Завершение кадра */
  glFinish();

CopyFrame:
  /* Выводим кадр в окно анимации */
  SwapBuffers(VG4_Anim.hDC);




COW:
static UINT CowProg;

CowProg = ShadProgInit("a.vert", "a.frag");

  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
  glUseProgram(CowProg);
  glColor3d(1, 1, 0);
  /*
  glRectd(-1, -1, 0, 1);
  glColor3d(1, 0, 0);
  glRectd(0, 0, 0.5, 0.5);
  glColor3d(0, 1, 0);
  glRectd(0.25, 0.25, 0.75, 0.75);
  glColor3d(0, 0, 1);
  glRectd(0.5, 0.5, 1, 1);
  */

  j = glGetUniformLocation(CowProg, "DrawColor");
  glUniform4d(j, 1, 0, 0, 1);

  glColor3d(1, 1, 0);
  glBegin(GL_TRIANGLES);
    glVertex3d(0, 0, 0);
    glVertex3d(1, 0, 0);
    glVertex3d(0, 1, 0);
  glEnd();
  glUseProgram(0);
















