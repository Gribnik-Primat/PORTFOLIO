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

void clipLine(Point winMin, Point winMax, Point p1, Point p2)
{
	unsigned char code1, code2;
	int done = FALSE, draw = FALSE;
	float m;
	while (!done)
	{
		code1 = encode(p1, winMin, winMax);
		code2 = encode(p2, winMin, winMax);
		if (ACCEPT(code1, code2))
		{
			done = TRUE;
			draw = TRUE;
		}
		else
			if (REJECT(code1, code2))
				done = TRUE;
			else
			{
				if (INSIDE(code1))
				{
					swapPts(&p1, &p2);
					swapCodes(&code1, &code2);
				}
				if (p2.x != p1.x)// using slope m to find linr-clipEdge intersection
					m = (p2.y - p1.y) / (p2.x - p1.x);
				if (code1 & LEFT_EDGE)
				{
					p1.y += (winMin.x - p1.x)*m;
					p1.x = winMin.x;
				}
				if (code1 & RIGHT_EDGE)
				{
					p1.y += (winMax.x - p1.x)*m;
					p1.x = winMax.x;
				}
				if (code1 & BOTTOM_EDGE)
				{
					if(p2.x != p1.x)
						p1.x += (winMin.y - p1.y)*m;
					p1.y = winMin.y;
				}
				if (code1 & TOP_EDGE)
				{
					if (p2.x != p1.x)
						p1.x += (winMax.y - p1.y)*m;
					p1.y = winMax.y;
				}
			}
	}
	if (draw)
		lineDDA(ROUND(p1.x), ROUND(p1.y), ROUND(p2.x), ROUND(p2.y));
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