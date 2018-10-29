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

void lineBres(int xa, int ya, int xb, int yb)
{
	int dx = abs(xa - xb), dy = abs(ya - yb);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy, twoDyDx = 2 * (dy - dx);
	int x, y, xEnd;

	if (xa > xb)
	{
		x = xb;
		y = yb;
		xEnd = xa;
	}
	else
	{
		x = xa;
		y = ya;
		xEnd = xb;
	}
	setPixel(x, y, 255, 255, 255);
	while (x < xEnd)
	{
		x++;
		if (p < 0)
			p += twoDy;
		else
		{
			y++;
			p += twoDyDx;
		}
		setPixel(x, y, 255, 255, 255);
	}
}


void Display(void)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	gluOrtho2D(0, WIDTH, 0, HEIGHT);
	lineBres(100, 100, 300,  600);
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
	glutCreateWindow("TASK02");


	glutDisplayFunc(Display);

	glutMainLoop();
	return 30;
}