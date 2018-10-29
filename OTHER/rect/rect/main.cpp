#include "rect.h"
void main()
{
  glutInitWindowSize(win_size_x, win_size_y);
  glutInitWindowPosition(0,0);
  glutCreateWindow("Task2");
  glClearColor(0, 0, 0, 1);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, win_size_x, 0, win_size_y);
  glutMouseFunc(MouseWrap);
  glutPassiveMotionFunc(MousepWrap);
  glutDisplayFunc(DisplayWrap);
  glutMainLoop();
}