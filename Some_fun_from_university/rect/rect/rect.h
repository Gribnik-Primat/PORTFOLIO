#ifndef RECT_H
#define RECT_H
#pragma comment(lib,"D:\\YandexDisk\\Projects(politex)(4)\\c++2\\rect\\rect\\glut32.lib")
#include <glut.h>
#include <cstdlib>

class point
{
public:
  double x, y;
};
class rect
{
private:
  point start, end;
public:
  void Display(void);
  void Mouse(int button, int state, int x, int y);
  void Mousep(int x, int y);
};

static double win_size_x = 1920, win_size_y = 1080;



void DisplayWrap(void);
void MouseWrap(int button, int state, int x, int y);
void MousepWrap(int x, int y);
#endif RECT_H