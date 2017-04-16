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

void circlePlotPoints(int xCenter, int yCenter, int x,int y)
{
	setPixel(xCenter + x, yCenter + y, 255, 0, 0);
	setPixel(xCenter - x, yCenter + y, 255, 0, 0);
	setPixel(xCenter + x, yCenter - y, 255, 0, 0);
	setPixel(xCenter - x, yCenter - y, 255, 0, 0);
	setPixel(xCenter + x, yCenter + y, 255, 0, 0);
	setPixel(xCenter - x, yCenter + y, 255, 0, 0);
	setPixel(xCenter + x, yCenter - y, 255, 0, 0);
	setPixel(xCenter - x, yCenter - y, 255, 0, 0);
}

void circleMidpoint(int xCenter,int yCenter,int radius)
{
	int x = 0;
	int y = radius;
	int p = 1 - radius;
	circlePlotPoints(xCenter, yCenter, x, y);
	while (x < y)
	{
		x++;
		if (p < 0)
			p += 2 * x + 1;
		else
		{
			y--;
			p += 2 * (x - y) + 1;
		}
		circlePlotPoints(xCenter, yCenter, x, y);
	}
	while (y > 0)
	{
		y--;
		if (p < 0)
		{
			p += 2 * y + 1;
		}
		else
		{
			x++;
			p += 2 * (y - x) + 1;
		}
		circlePlotPoints(xCenter, yCenter, x, y);
	}
}


void Display(void)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	gluOrtho2D(0, WIDTH, 0, HEIGHT);
	circleMidpoint(200, 200, 50);
	glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, Pixels);

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
	glutCreateWindow("TASK03");


	glutDisplayFunc(Display);

	glutMainLoop();
	return 30;
}