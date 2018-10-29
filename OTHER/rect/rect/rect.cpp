#include "rect.h"
int count_of_fixed_dots = 0;
int color[12] = { rand() % 255 };
rect rubber;
void rect::Display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glLineWidth(1);
  glBegin(GL_QUADS);
  glColor3ub(color[0], color[1], color[2]);
  glVertex2d(this->start.x, this->start.y);
  glColor3ub(color[3], color[4], color[5]);
  glVertex2d(this->end.x, this->start.y);
  glColor3ub(color[6], color[7], color[8]);
  glVertex2d(this->end.x, this->end.y);
  glColor3ub(color[9], color[10], color[11]);
  glVertex2d(this->start.x, this->end.y);
  glEnd();
  glFinish();
  glutSwapBuffers();
  glutPostRedisplay();
}
void  rect::Mouse(int button, int state, int x, int y)
{
  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
  {
	for (int i = 0; i < 12; i++)
	  color[i] = rand() % 255;
  }
  if (button == GLUT_LEFT_BUTTON)
  {
	if (count_of_fixed_dots == 1 && state == GLUT_DOWN)
	{
	  this->end.x = x;
	  this->end.y = win_size_y - y;
	  count_of_fixed_dots = 2;
	}
	if (count_of_fixed_dots != 1 && state == GLUT_DOWN)
	{
	  this->start.x = this->end.x = x;
	  this->start.y = this->end.y = win_size_y - y;
	  count_of_fixed_dots = 1;
	}
  }
}

void  rect::Mousep(int x, int y)
{
  if (count_of_fixed_dots == 1)
  {
	this->end.x = x;
	this->end.y = win_size_y - y;
  }
}
void MousepWrap(int x, int y)
{
  rubber.Mousep(x, y);
}
void MouseWrap(int button, int state, int x, int y)
{
  rubber.Mouse(button,state,x,y);
}
void DisplayWrap(void)
{
  rubber.Display();
}