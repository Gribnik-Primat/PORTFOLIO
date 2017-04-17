#include <glut.h>

#include <stdio.h>
#include <stdlib.h>

#define ROUND(a) ((int)(a+0.5))

#define LEFT_EDGE 0x1
#define RIGHT_EDGE 0x2
#define BOTTOM_EDGE 0x4
#define TOP_EDGE 0x8

#define TRUE 1
#define FALSE 0

#define INSIDE(a) (!a)
#define REJECT(a,b) (a&b)
#define ACCEPT(a,b) (!(a|b))

#define WIDTH 1920
#define HEIGHT 1080

int Pixels[HEIGHT][WIDTH][3];

typedef struct tagPoint Point;
struct tagPoint
{
	int x, y;
};

void setPixel(int x, int y, int R, int G, int B)
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
	setPixel(ROUND(x), ROUND(y), 255, 0, 0);
	for (k = 0; k < steps; k++)
	{
		x += xIncrement;
		y += yIncrement;
		setPixel(x, y, 255, 0, 0);
	}

}

unsigned char encode(Point pt, Point winMin, Point winMax)
{
	unsigned char code = 0x00;

	if (pt.x < winMin.x)
		code = code | LEFT_EDGE;
	if (pt.x > winMax.x)
		code = code | RIGHT_EDGE;
	if (pt.y < winMin.y)
		code = code | BOTTOM_EDGE;
	if (pt.y > winMax.y)
		code = code | TOP_EDGE;
	return code;
}

void swapPts(Point *p1, Point *p2)
{
	Point tmp;
	tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
}

void swapCodes(unsigned char *c1, unsigned char*c2)
{
	unsigned char tmp;
	tmp = *c1;
	*c1 = *c2;
	*c2 = tmp;
}

int clipTest(float p, float q, float * u1, float *u2)
{
	float r;
	int retVal = TRUE;
	if (p < 0.0)
	{
		r = q / p;
		if (r > *u2)
			retVal = FALSE;
		else
			if (r > *u1)
				*u1 = r;
	}
	else
		if (p > 0.0)
		{
			r = q / p;
			if (r < *u1)
				retVal = FALSE;
			else if (r < *u2)
				*u2 = r;
		}
		else
			if (q < 0.0)
				retVal = FALSE;
	return(retVal);
}

void clipLine(Point winMin, Point winMax, Point p1, Point p2)
{
	float u1 = 0.0, u2 = 1.0, dx = p2.x - p1.x, dy;
	if(clipTest(-dx,p1.x + winMin.x,&u1,&u2))
		if (clipTest(dx, winMax.x - p1.x, &u1, &u2))
		{
			dy = p2.y - p1.y;
			if (clipTest(-dy, p1.y - winMin.y, &u1, &u2))
				if (clipTest(dx, winMax.y - p1.y, &u1, &u2))
				{
					if (u2 < 1.0)
					{
						p2.x = p1.x + u2*dx;
						p2.y = p1.y + u2*dy;
					}
					if (u1 > 0.0)
					{
						p1.x += u1*dx;
						p1.y += u1*dy;
					}
					lineDDA(ROUND(p1.x), ROUND(p1.y), ROUND(p2.x), ROUND(p2.y));
				}
		}
}



void Display(void)
{
	Point winMin, winMax, p1, p2;
	winMax.x = 300;
	winMax.y = 300;
	winMin.x = 0;
	winMin.y = 0;
	p1.x = 50;
	p1.y = 50;
	p2.x = 100;
	p2.y = 500;
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	clipLine(winMin, winMax, p1, p2);

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
	glutCreateWindow("TASK05");


	glutDisplayFunc(Display);

	glutMainLoop();
	return 30;
}