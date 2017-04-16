#include <glut.h>

#include <stdio.h>
#include <stdlib.h>

#define ROUND(a) ((int)(a+0.5))
#define WIDTH 1920
#define HEIGHT 1080

int Pixels[HEIGHT][WIDTH][3];

void setPixel(int x, int y,int R,int G,int B)
{
	Pixels[y][x][0] = R;
	Pixels[y][x][1] = G;
	Pixels[y][x][2] = B;
}

void lineDDA(int xa, int ya, int xb, int yb)
{
	int dx = xb - xa, dy = yb - ya, steps, k;
	float xIncrement, yIncrement, x = xa, y = ya;

	if (abs(dx) > abs(dy))
		steps = abs(dx);
	else
		steps = abs(dy);
	xIncrement = dx / (float)steps;
	yIncrement = dy / (float)steps;
	setPixel(ROUND(x), ROUND(y),255,0,0);
	for (k = 0; k < steps; k++)
	{
		x += xIncrement;
		y += yIncrement;
		setPixel(x, y, 255, 0, 0);
	}
	
}

void Display(void)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);


	lineDDA(50, 50, 100, 200);
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
	glutCreateWindow("TASK01");


	glutDisplayFunc(Display);

	glutMainLoop();
	return 30;
}