#include <glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROUND(a) ((int)(a+0.5))
#define WIDTH 1920
#define HEIGHT 1080

int Pixels[HEIGHT][WIDTH][3];

void setPixel(int x, int y, int R, int G, int B)
{
	Pixels[y][x][0] = R;
	Pixels[y][x][1] = G;
	Pixels[y][x][2] = B;
}

void ellipsePlotPoints(int xCenter, int yCenter, int x, int y)
{
	setPixel(xCenter + x, yCenter + y, 255, 0, 0);
	setPixel(xCenter - x, yCenter + y, 255, 0, 0);
	setPixel(xCenter + x, yCenter - y, 255, 0, 0);
	setPixel(xCenter - x, yCenter - y, 255, 0, 0);
}

void ellipseMidpoint(int xCenter, int yCenter, int Rx, int Ry)
{
	int Rx2 = Rx*Rx;
	int Ry2 = Ry*Ry;
	int twoRx2 = 2*Rx2;
	int twoRy2 = 2*Ry2;
	int p;
	int x = 0;
	int y = Ry;
	int px = 0;
	int py = twoRx2 * y;

	ellipsePlotPoints(xCenter, yCenter, x, y);
	/*region 1*/
	p = ROUND(Ry2 - (Rx2*Ry) + (0.25*Rx2));
	while(px<py)
	{
		x++;
		px += twoRy2;
		if (p < 0)
			p += Ry2 + px;
		else
		{
			y--;
			py -= twoRx2;
			p += Ry2 + px - py;
		}

		ellipsePlotPoints(xCenter, yCenter, x, y);
	}
	/*region 1*/
	p = ROUND(Ry2*(x+0.5)*(x + 0.5)+Rx2*(y-1)*(y - 1)-Rx2*Ry2);
	while (y>0)
	{
		y--;
		px -= twoRx2;
		if (p > 0)
			p += Rx2 - py;
		else
		{
			x++;
			px -= twoRy2;
			p -= Rx2 - py + px;
		}

		ellipsePlotPoints(xCenter, yCenter, x, y);
	}
}

void Display(void)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	gluOrtho2D(0, WIDTH, 0, HEIGHT);
	ellipseMidpoint(200, 200, 100, 20);
	glDrawPixels(WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, Pixels);

	glFinish();
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("TASK04");


	glutDisplayFunc(Display);

	glutMainLoop();
	return 30;
}