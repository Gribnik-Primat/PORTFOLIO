#include "ANIM.h"
#include "curve.h"
#include <cstdio>
using namespace std;

class surf : public bgl::object
{
private:
	Vec p[4][4];
	Vec v[4][4];
	Vec pp, q0, q1, q2;
	static const int W = 16, H = 16;

	Vec G[H][W];
	FILE *F;
public:
	surf(void)
	{
		if ((F = fopen("COORDS.TXT", "rt")) == NULL)
			exit(0);
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				for (int c = 0; c < 3; c++)
					p[i][j][c] = 0;


		/*for(int i = 0;i < 4;i++)
		for(int j = 0; j<4;j++)
		p[i][j] = Vec::prnd(),v[i][j] = Vec::vrnd();*/


	}
	void Handle(bgl::anim & ANI)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				p[i][j] = p[i][j] + v[i][j] * 6;
				for (int c = 0; c < 3; c++)
				{
					if (p[i][j][c] >= 5)
						v[i][j][c] = -v[i][j][c];
					if (p[i][j][c] <= -5)
						v[i][j][c] = -v[i][j][c];
				}
			}
	}



	void DrawG(void)
	{
		glColor3d(1, 0.3, 1);
		for (int i = 0; i < H - 1; i++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			for (int j = 0; j < W; j++)
			{
				glVertex3dv(&G[i + 1][j].X);
				glVertex3dv(&G[i][j].X);
			}
			glEnd();
		}
		glPointSize(3);
		glColor3d(1, 1, 1);
		glBegin(GL_POINTS);
		for (int i = 0; i < H; i++)
			for (int j = 0; j < W; j++)
				glVertex3dv(&G[i][j].X);
		glEnd();

		for (int i = 0; i < H; i++)
		{
			glBegin(GL_LINE_STRIP);
			for (int j = 0; j < W; j++)
				glVertex3dv(&G[i][j].X);
			glEnd();
		}
		for (int j = 0; j < W; j++)
		{
			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < H; i++)
				glVertex3dv(&G[i][j].X);
			glEnd();
		}
	}
	void Render(bgl::anim &ani)
	{
		glDisable(GL_LIGHTING);
		glColor3d(0, 1, 0);
		glPointSize(5);
		glBegin(GL_POINTS);
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				glVertex3dv(&p[i][j].X);
		glEnd();
		/* glBegin(GL_LINE_LOOP);

		for(int i = 0;i < 4;i++)
		for(int j = 0; j<4;j++)
		for(int c = 0; c<3;c++)
		glVertex3dv(&p[i][j][c]);
		*/
		glEnd();




		double u, v, C[4] = { 1,3,3,1 };
		int m, n, k, l, count = 0, flag = 0;

		while (flag != 1)
		{
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++) {
					if (fscanf(F, "%lf %lf %lf", &p[i][j][0], &p[i][j][1], &p[i][j][2]) == -1);
					count++;
				}

			if (count == 512)
			{
				rewind(F);
				count = 0;
				flag = 1;
				break;
			}
			for (int i = 0; i < H; i++)
				for (int j = 0; j < W; j++)
					for (int c = 0; c < 3; c++)
						G[i][j][c] = 0;

			for (k = 0, v = 0; k < H; k++, v += 1.0 / (H - 1))
				for (l = 0, u = 0; l < W; l++, u += 1.0 / (W - 1))
				{
					double us[4] = { 1,u,u*u,u*u*u };
					double u1s[4] = { 1,(1 - u),(1 - u)*(1 - u),(1 - u)*(1 - u)*(1 - u) };
					double vs[4] = { 1,v,v*v,v*v*v };
					double v1s[4] = { 1,(1 - v),(1 - v)*(1 - v),(1 - v)*(1 - v)*(1 - v) };
					pp = Vec(0, 0, 0);
					for (m = 0; m < 4; m++)
						for (n = 0; n < 4; n++)
							for (int c = 0; c < 3; c++)
								pp[c] = pp[c] + p[m][n][c] * C[m] * C[n] * u1s[n] * us[3 - n] * v1s[m] * vs[3 - m];
					G[k][l] = pp;
				}


			DrawG();
		}
	}
};



void main(void)
{
	bgl::anim &My = bgl::anim::GetRef();
	My << new surf();

	My.Run();
}