#ifndef __VEC_H_
#define __VEC_H_

#include <cstdlib>

class Vec
{
public:
	double X, Y, Z;
	Vec(void)
	{}
	explicit Vec(double a) :X(a), Y(a), Z(a)
	{}
	Vec(double a, double b, double c) :X(a), Y(b), Z(c)
	{}
	Vec  operator- (void)const
	{
		return Vec(-X, -Y, -Z);
	}
	Vec  operator+ (const Vec &v)const
	{
		return Vec(v.X + X, v.Y + Y, v.Z + Z);
	}

	Vec  operator- (const Vec &v)const
	{
		return Vec(v.X - X, v.Y - Y, v.Z - Z);
	}
	Vec  operator* (const int x)const
	{
		return Vec(X*x, Y*x, Z*x);
	}
	Vec  operator/ (const int x)const
	{
		return Vec(X / x, Y / x, Z / x);
	}
	Vec  operator% (const Vec &v)const
	{
		return Vec(Y * v.Z - v.Y * Z, X * v.Z - v.X * Z, Y * v.X - v.Y * X);
	}
	Vec  operator& (const Vec &v)const
	{
		return Vec(v.X*X, v.Y*Y, v.Z*Z);
	}
	double & operator [](int I)
	{
		return *(&X + I);
	}
	static Vec prnd(void)
	{
		Vec V;
		for (int i = 0; i < 3; i++)
			V[i] = (10.0*rand() / RAND_MAX - 5);
		return V;
	}
	static Vec vrnd(void)
	{
		Vec V;
		for (int i = 0; i < 3; i++)
			V[i] = (0.001*rand() / RAND_MAX - 0.005);
		return V;
	}


};

#endif