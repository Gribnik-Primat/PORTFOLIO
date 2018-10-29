#ifndef __VEC_H_
#define __VEC_H_


#include "def.h"
#include <math.h>
#include <windows.h>

#define PI 3.14159265358979323846
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)
typedef double DBL;
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

typedef struct tagMATR
{
  DBL A[4][4];
} MATR;


static VEC LOC,AT,UP;

/* функции реализации */

__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                           DBL A21, DBL A22, DBL A23,
                           DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 -
    A11 * A23 * A32 -
    A12 * A21 * A33 +
    A12 * A23 * A31 +
    A13 * A21 * A32 - 
    A13 * A22 * A31;
}

__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
} /* End of 'VecSet' function */


__inline VEC VecAddVec( VEC V1, VEC V2 )  //действи€ с векторами
{
  VEC V3;
  V3.X = V1.X + V2.X;
  V3.Y = V1.Y + V2.Y;
  V3.Z = V1.Z + V2.Z;
  return V3;
}
__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  VEC V3;
  V3.X = V1.X + (-V2.X);
  V3.Y = V1.Y + (-V2.Y);
  V3.Z = V1.Z + (-V2.Z);
  return V3;
}
__inline VEC VecMulNum( VEC V1, DBL N )
{
  V1.X = V1.X * N;
  V1.Y = V1.Y * N;
  V1.Z = V1.Z * N;
  return V1;
}
__inline VEC VecDivNum( VEC V1, DBL N )
{
  V1.X = V1.X / N;
  V1.Y = V1.Y / N;
  V1.Z = V1.Z / N;
  return V1;
}
__inline VEC VecNeg( VEC V )  // помен€ть знак
{
  V.X = -V.X;
  V.Y = -V.Y;
  V.Z = -V.Z;
  return V;
}
__inline DBL VecDotVec( VEC V1, VEC V2 ) //скал€рное 
{
  DBL sum;
  return sum = V1.X*V2.X + V1.Y*V2.Y + V1.Z*V2.Z;
}

__inline DBL VecLen( VEC V ) //len
{
  DBL len;               
  return len = sqrt(V.X*V.X+V.Y*V.Y+V.Z*V.Z);
}
__inline DBL VecLen2( VEC V ) //sqrt
{
  DBL len;               
  return len = V.X*V.X+V.Y*V.Y+V.Z*V.Z;
}

__inline VEC VecNormalize( VEC V )
{
  double len = VecLen(V);
  return VecSet(V.X / len, V.Y / len, V.Z / len);
}

__inline VEC VecMulMatr( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]
  );
} 

/* единична€ матрица */
static MATR UnitMatrix =
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
};

__inline MATR MatrUnit( VOID )
{
  return UnitMatrix;
}





__inline MATR MatrTranslate( DBL Dx, DBL Dy, DBL Dz )
{
  MATR m = UnitMatrix;

  m.A[3][0] = Dx;
  m.A[3][1] = Dy;
  m.A[3][2] = Dz;
  return m;
}
__inline MATR MatrScale( DBL Sx, DBL Sy, DBL Sz )
{
  MATR m = UnitMatrix;

  m.A[0][0] = Sx;
  m.A[1][1] = Sy;
  m.A[2][2] = Sz;
  return m;
}

__inline MATR MatrRotateX( DBL AngleeDegree )
{
  MATR m = UnitMatrix;

  m.A[1][1] = cos(AngleeDegree);
  m.A[1][2] = -sin(AngleeDegree);
  m.A[2][1] = sin(AngleeDegree);
  m.A[2][2] = cos(AngleeDegree);
  return m;
}

__inline MATR MatrRotateY( DBL AngleeDegree )
{
  MATR m = UnitMatrix;

  m.A[0][0] = cos(AngleeDegree);
  m.A[2][0] = -sin(AngleeDegree);
  m.A[0][2] = sin(AngleeDegree);
  m.A[2][2] = cos(AngleeDegree);
  return m;
}


__inline MATR MatrRotateZ( DBL AngleeDegree )
{
  MATR m = UnitMatrix;

  m.A[0][0] = cos(AngleeDegree);
  m.A[0][1] = -sin(AngleeDegree);
  m.A[1][0] = sin(AngleeDegree);
  m.A[1][1] = cos(AngleeDegree);
  return m;
}

__inline MATR MatrRotate( DBL AngleeDegree, DBL X, DBL Y, DBL Z )
{
  MATR m;
  DBL
    a = D2R(AngleeDegree), si = sin(a), co = cos(a),
    len = X * X + Y * Y + Z * Z;

  if (len != 0 && len != 1)
    len = sqrt(len),  X /= len, Y /= len, Z /= len;
  X *= si;
  Y *= si;
  Z *= si;

  m.A[0][0] = 1 - 2 * (Y * Y + Z* Z);
  m.A[0][1] = 2 * X * Y + 2 * co * Z;
  m.A[0][2] = 2 * X * Z - 2 * co * Y; 
  m.A[0][3] = 0;

  m.A[1][0] = 2 * X * Y - 2 * co * Z;
  m.A[1][1] = 1 - 2 * (X * X + Z* Z);
  m.A[1][2] = 2 * X * co - 2 * Z * Y;   
  m.A[1][3] = 0;

  m.A[2][0] = 2 * co * Y + 2 * X * Z;
  m.A[2][1] = 2 * Y * Z - 2 * co * X;
  m.A[2][2] = 1 - 2 * (X * X + Y* Y); 
  m.A[2][3] = 0;

  m.A[3][0] = 0;
  m.A[3][1] = 0;
  m.A[3][2] = 0;
  m.A[3][3] = 1;

  return m;
}

__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  INT i, j, k;
  MATR M;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (M.A[i][j] = 0, k = 0; k < 4; k++)
        M.A[i][j] += M1.A[i][k] * M2.A[k][j];
}




__inline DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
    M.A[2][1], M.A[2][2], M.A[2][3],
    M.A[3][1], M.A[3][2], M.A[3][3]) - 
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
    M.A[2][0], M.A[2][2], M.A[2][3],
    M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
    M.A[2][0], M.A[2][1], M.A[2][3],
    M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
    M.A[2][0], M.A[2][1], M.A[2][2],
    M.A[3][0], M.A[3][1], M.A[3][2]);
}

__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(
    V1.Y * V2.Z - V1.Z * V2.Y,
    V1.Z * V2.X - V1.X * V2.Z,
    V1.X * V2.Y - V1.Y * V2.X
    );
}

__inline MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);

  if (det == 0)
    return UnitMatrix;

  r.A[0][0] =
    MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
    M.A[2][1], M.A[2][2], M.A[2][3],
    M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][0] = 
    -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
    M.A[2][0], M.A[2][2], M.A[2][3],
    M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][0] =
    MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
    M.A[2][0], M.A[2][1], M.A[2][3],
    M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
    M.A[2][0], M.A[2][1], M.A[2][2],
    M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][1] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
    M.A[2][1], M.A[2][2], M.A[2][3],
    M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] = 
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
    M.A[2][0], M.A[2][2], M.A[2][3],
    M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
    M.A[2][0], M.A[2][1], M.A[2][3],
    M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
    M.A[2][0], M.A[2][1], M.A[2][2],
    M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][2] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
    M.A[1][1], M.A[1][2], M.A[1][3],
    M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] = 
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
    M.A[1][0], M.A[1][2], M.A[1][3],
    M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
    M.A[1][0], M.A[1][1], M.A[1][3],
    M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
    M.A[1][0], M.A[1][1], M.A[1][2],
    M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][3] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
    M.A[1][1], M.A[1][2], M.A[1][3],
    M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.A[1][3] = 
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
    M.A[1][0], M.A[1][2], M.A[1][3],
    M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[2][3] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
    M.A[1][0], M.A[1][1], M.A[1][3],
    M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
    M.A[1][0], M.A[1][1], M.A[1][2],
    M.A[2][0], M.A[2][1], M.A[2][2]) / det;

  return r;  
}



__inline MATR MatrViewLookAt(VEC Loc, VEC At, VEC Up)
{
  VEC 
    Dir = VecSubVec(At, Loc),
    Right = VecCrossVec(Dir, Up);
  MATR m;


  Dir = VecNormalize(Dir);
  Right = VecNormalize(Right);
  Up = VecCrossVec(Right, Dir);


  m.A[0][0] = Right.X;
  m.A[0][1] = Up.X;
  m.A[0][2] = Dir.X;
  m.A[0][3] = 0;
  m.A[1][0] = Right.Y;
  m.A[1][1] = Up.Y;
  m.A[1][2] = Dir.Y;
  m.A[1][3] = 0;
  m.A[2][0] = Right.Z;
  m.A[2][1] = Up.Z;
  m.A[2][2] = Dir.Z;
  m.A[2][3] = 0;
  m.A[3][0] = VecDotVec(Loc, Right);
  m.A[3][1] = VecDotVec(Loc, Up);
  m.A[3][2] = -VecDotVec(Loc, Dir);
  m.A[3][3] =  1;

  return m;
}

__inline MATR MatrFrustrum(INT l,INT t,INT r,INT b,INT n,INT f)
{
  MATR M = UnitMatrix;
  M.A[0][0] = (2*n)/(r-l);
  M.A[1][1] = (2*n)/(t-b);
  M.A[2][0] = (r+l)/(r-l);
  M.A[2][1] = (t+b)/(t-b);
  M.A[2][2] = -(f+n)/(f-n);
  M.A[2][3] = -1;
  M.A[3][2] = (-2*n*f)/(f-n);

  return M;
}

__inline MATR MatrOrto(INT l,INT t,INT r,INT b,INT n,INT f)
{
  MATR M = UnitMatrix;
  M.A[0][0] = 2/(r-l);
  M.A[1][1] = 2/(t-b);
  M.A[2][2] = -2/(f-n);
  M.A[3][1] = -(t+b)/(t-b);
  M.A[3][2] = -(f+n)/(f-n);
  M.A[3][0] = -(r+l)/(r-l);
  M.A[3][3] = 1;

  return M;
}


#endif /* __VEC_H_ */

