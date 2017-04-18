–еализаци€ Ч файлы: VEC.H VEC.C 
#include <math.h>

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


/* функции реализации */

VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
} /* End of 'VecSet' function */


VEC VecAddVec( VEC V1, VEC V2 );
VEC VecSubVec( VEC V1, VEC V2 );
VEC VecMulNum( VEC V1, DBL N );
VEC VecDivNum( VEC V1, DBL N );
VEC VecNeg( VEC V );
DBL VecDotVec( VEC V1, VEC V2 );
VEC VecCrossVec( VEC V1, VEC V2 );
DBL VecLen2( VEC V );
DBL VecLen( VEC V );
VEC VecNormalize( VEC V );

VEC VecMulMatr( VEC V, MATR M );

/* единична€ матрица */
static MATR UnitMatrix =
{
  {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
  }
};

MATR MatrUnit( VOID )
{
  return UnitMatrix;
}

MATR MatrTranslate( DBL Dx, DBL Dy, DBL Dz )
{
  MATR m = UnitMatrix;

  m.A[3][0] = Dx;
  m.A[3][1] = Dy;
  m.A[3][2] = Dz;
  return UnitMatrix;
}
MATR MatrScale( DBL Sx, DBL Sy, DBL Sz );
MATR MatrRotateX( DBL AngleeDegree );
MATR MatrRotateY( DBL AngleeDegree );
MATR MatrRotateZ( DBL AngleeDegree );
MATR MatrRotate( DBL AngleeDegree, DBL X, DBL Y, DBL Z )
{
  MATR m;
  DBL
    a = D2R(AngleeDegree), si = sin(a), co = cos(a),
    len = X * X + Y * Y + Z * Z;

  if (len != 0 && len != 1)
    X /= len, Y /= len, Z /= len;
  X *= si;
  Y *= si;
  Z *= si;

  m.A[0][0] = 1 - 2 * (Y * Y + Z* Z);
  m.A[0][1] = 2 * X * Y - 2 * co * Z;
  . . .
  m.A[0][3] = 0;
  m.A[1][3] = 0;
  m.A[2][3] = 0;

  m.A[3][0] = 0;
  m.A[3][1] = 0;
  m.A[3][2] = 0;
  m.A[3][3] = 1;
}

MATR MatrMulMatr( MATR M1, MATR M2 )
{
  INT i, j, k;
  MATR m;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (m.M[i][j] = 0, k = 0; k < 4; k++)
        m.M[i][j] += M1.M[i][k] * M2.M[k][j];
}

DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
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


DBL MatrDeterm( MATR M )
{
  return
    M.M[0][0] * MatrDeterm3x3(M.M[1][1], M.M[1][2], M.M[1][3],
                              M.M[2][1], M.M[2][2], M.M[2][3],
                              M.M[3][1], M.M[3][2], M.M[3][3]) - 
    M.M[0][1] * MatrDeterm3x3(M.M[1][0], M.M[1][2], M.M[1][3],
                              M.M[2][0], M.M[2][2], M.M[2][3],
                              M.M[3][0], M.M[3][2], M.M[3][3]) +
    M.M[0][2] * MatrDeterm3x3(M.M[1][0], M.M[1][1], M.M[1][3],
                              M.M[2][0], M.M[2][1], M.M[2][3],
                              M.M[3][0], M.M[3][1], M.M[3][3]) -
    M.M[0][3] * MatrDeterm3x3(M.M[1][0], M.M[1][1], M.M[1][2],
                              M.M[2][0], M.M[2][1], M.M[2][2],
                              M.M[3][0], M.M[3][1], M.M[3][2]);
}

MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);

  if (det == 0)
    return UnitMatrix;

  r.M[0][0] =
    MatrDeterm3x3(M.M[1][1], M.M[1][2], M.M[1][3],
                 M.M[2][1], M.M[2][2], M.M[2][3],
                 M.M[3][1], M.M[3][2], M.M[3][3]) / det;
  r.M[1][0] = 
    -MatrDeterm3x3(M.M[1][0], M.M[1][2], M.M[1][3],
                  M.M[2][0], M.M[2][2], M.M[2][3],
                  M.M[3][0], M.M[3][2], M.M[3][3]) / det;
  r.M[2][0] =
    MatrDeterm3x3(M.M[1][0], M.M[1][1], M.M[1][3],
                  M.M[2][0], M.M[2][1], M.M[2][3],
                  M.M[3][0], M.M[3][1], M.M[3][3]) / det;
  r.M[3][0] =
    -MatrDeterm3x3(M.M[1][0], M.M[1][1], M.M[1][2],
                  M.M[2][0], M.M[2][1], M.M[2][2],
                  M.M[3][0], M.M[3][1], M.M[3][2]) / det;

  r.M[0][1] =
    -MatrDeterm3x3(M.M[0][1], M.M[0][2], M.M[0][3],
                 M.M[2][1], M.M[2][2], M.M[2][3],
                 M.M[3][1], M.M[3][2], M.M[3][3]) / det;
  r.M[1][1] = 
    MatrDeterm3x3(M.M[0][0], M.M[0][2], M.M[0][3],
                  M.M[2][0], M.M[2][2], M.M[2][3],
                  M.M[3][0], M.M[3][2], M.M[3][3]) / det;
  r.M[2][1] =
    -MatrDeterm3x3(M.M[0][0], M.M[0][1], M.M[0][3],
                  M.M[2][0], M.M[2][1], M.M[2][3],
                  M.M[3][0], M.M[3][1], M.M[3][3]) / det;
  r.M[3][1] =
    MatrDeterm3x3(M.M[0][0], M.M[0][1], M.M[0][2],
                  M.M[2][0], M.M[2][1], M.M[2][2],
                  M.M[3][0], M.M[3][1], M.M[3][2]) / det;

  r.M[0][2] =
    MatrDeterm3x3(M.M[0][1], M.M[0][2], M.M[0][3],
                 M.M[1][1], M.M[1][2], M.M[1][3],
                 M.M[3][1], M.M[3][2], M.M[3][3]) / det;
  r.M[1][2] = 
    -MatrDeterm3x3(M.M[0][0], M.M[0][2], M.M[0][3],
                  M.M[1][0], M.M[1][2], M.M[1][3],
                  M.M[3][0], M.M[3][2], M.M[3][3]) / det;
  r.M[2][2] =
    MatrDeterm3x3(M.M[0][0], M.M[0][1], M.M[0][3],
                  M.M[1][0], M.M[1][1], M.M[1][3],
                  M.M[3][0], M.M[3][1], M.M[3][3]) / det;
  r.M[3][2] =
    -MatrDeterm3x3(M.M[0][0], M.M[0][1], M.M[0][2],
                  M.M[1][0], M.M[1][1], M.M[1][2],
                  M.M[3][0], M.M[3][1], M.M[3][2]) / det;

  r.M[0][3] =
    -MatrDeterm3x3(M.M[0][1], M.M[0][2], M.M[0][3],
                 M.M[1][1], M.M[1][2], M.M[1][3],
                 M.M[2][1], M.M[2][2], M.M[2][3]) / det;
  r.M[1][3] = 
    MatrDeterm3x3(M.M[0][0], M.M[0][2], M.M[0][3],
                  M.M[1][0], M.M[1][2], M.M[1][3],
                  M.M[2][0], M.M[2][2], M.M[2][3]) / det;
  r.M[2][3] =
    -MatrDeterm3x3(M.M[0][0], M.M[0][1], M.M[0][3],
                  M.M[1][0], M.M[1][1], M.M[1][3],
                  M.M[2][0], M.M[2][1], M.M[2][3]) / det;
  r.M[3][3] =
    MatrDeterm3x3(M.M[0][0], M.M[0][1], M.M[0][2],
                  M.M[1][0], M.M[1][1], M.M[1][2],
                  M.M[2][0], M.M[2][1], M.M[2][2]) / det;

  return r;  
}


!!! __inline <- Visual Studio

в заголовках:
VEC.H

__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
}
