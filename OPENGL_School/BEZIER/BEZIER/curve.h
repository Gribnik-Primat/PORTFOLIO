#ifndef __CURVE_H_
#define __CURVE_H_


#include "ANIM.h"
#include "vec.h"
#include <cmath>
#include <cstdlib>

static int a = 3;
class curve : public bgl::object
{
private:
	static const int MAX = 10;

	Vec POS[MAX];
	Vec VEL[MAX];

public:

	curve(void)
	{
		for (int i = 0; i < MAX; i++) {
			POS[i].X = (10.0*rand() / RAND_MAX - 5);
			POS[i].Y = (10.0*rand() / RAND_MAX - 5);
			POS[i].Z = (10.0*rand() / RAND_MAX - 5);
		}
		for (int i = 0; i < MAX; i++) {
			VEL[i].X = (0.001*rand() / RAND_MAX - 0.005);
			VEL[i].Y = (0.001*rand() / RAND_MAX - 0.005);
			VEL[i].Z = (0.001*rand() / RAND_MAX - 0.005);
		}
	}

	virtual void Handle(bgl::anim & ANI)
	{
		for (int i = 0; i < MAX; i++)
		{
			POS[i] = POS[i] + VEL[i] * 3;
			for (int c = 0; c < 3; c++)
			{
				if (POS[i][c] >= 5)
					VEL[i][c] = -VEL[i][c];
				if (POS[i][c] <= -5)
					VEL[i][c] = -VEL[i][c];
			}

		}
	}
	virtual void Render(bgl::anim & ANI)
	{
		DCMR(POS, MAX);
	}
	void DrawBezier(Vec &P0, Vec &P1, Vec &P2, Vec &P3)
	{
	}
	void DrawBezier(Vec *P)
	{
		double t;
		glBegin(GL_LINE_STRIP);
		for (t = 0; t <= 1; t += 0.01)
		{
			glColor3d(1, 0, 0);
			Vec p;
			p.X = P[0].X*(1 - t)*(1 - t)*(1 - t) +
				P[1].X * 3 * (1 - t)*(1 - t)*t +
				P[2].X * 3 * (1 - t)*t*t +
				P[3].X*t*t*t;
			p.Y = P[0].Y*(1 - t)*(1 - t)*(1 - t) +
				P[1].Y * 3 * (1 - t)*(1 - t)*t +
				P[2].Y * 3 * (1 - t)*t*t +
				P[3].Y*t*t*t;
			p.Z = P[0].Z*(1 - t)*(1 - t)*(1 - t) +
				P[1].Z * 3 * (1 - t)*(1 - t)*t +
				P[2].Z * 3 * (1 - t)*t*t +
				P[3].Z*t*t*t;


			glVertex3d(p.X, p.Y, p.Z);
		}

		glVertex3d(P[3].X, P[3].Y, P[3].Z);
		glEnd();
		glBegin(GL_LINES);
		for (int i = 0; i < 4; i++)
		{
			glColor3d(0, 1, 0);
			glVertex3d(P[i].X, P[i].Y, P[i].Z);
		}
		glEnd();
		glPointSize(7);
		glBegin(GL_POINTS);
		for (int i = 0; i < 4; i++)
		{
			glColor3d(0, 0, 1);
			glVertex3d(P[i].X, P[i].Y, P[i].Z);
		}
		glEnd();
	}
	void DCMR(Vec *POS, int N)
	{
		for (int i = 0; i < N - 1; i++)
		{
			Vec P[4];
			P[0] = POS[i];
			if (i == 0)
				P[1] = (POS[1] - POS[0])*a + POS[0];
			else
				P[1] = ((POS[i + 1]) - (POS[i - 1]))*a + POS[i];
			if (i == N - 2)
				P[2] = -(POS[N - 1] - POS[N - 2])*a + POS[N - 1];
			else
				P[2] = -(POS[i + 2] - POS[i])*a + POS[i + 1];
			P[3] = POS[i + 1];

			DrawBezier(P);
		}
	}
};



#endif