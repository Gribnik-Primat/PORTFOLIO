/* FILE NAME: VEC.H
 * PROGRAMMER: IK1
 * DATE: 05.06.2013
 * PURPOSE: Vector and matrix library.
 */
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

static MATR UnitMatrix =
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
};

/* функции реализации */

_inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
} /* End of 'VecSet' function */

_inline VEC VecAddVec( VEC V1, VEC V2 )
{ 
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
}
_inline VEC VecSubVec( VEC V1, VEC V2 )
{ 
 return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
}
_inline VEC VecMulNum( VEC V1, DBL N )
{ 
  return VecSet(N * V1.X , N * V1.Y , N * V1.Z);
}
_inline VEC VecDivNum( VEC V1, DBL N )
{ 
  return VecSet( V1.X / N, V1.Y / N , V1.Z /N);
}
_inline VEC VecNeg( VEC V )
{ 
  return VecSet( -V.X , -V.Y , -V.Z);
}
_inline DBL VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z; 
}
_inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y, 
                V1.Z * V2.X - V1.X * V2.Z, 
                V1.X * V2.Y - V1.Y * V2.X);
}
_inline DBL VecLen2( VEC V )
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
}
_inline DBL VecLen( VEC V )
{
  return sqrt(V.X * V.X + V.Y * V.Y + V.Z * V.Z);
}
_inline VEC VecNormalize( VEC V )
{
  return VecDivNum( V, VecLen(V));
}

_inline VEC VecMulMatr( VEC V, MATR M )                  
{
  return VecSet(V.X * M.A[0][0]+ V.Y * M.A[0][1] + V.Z * M.A[0][2],
                V.X * M.A[1][0]+ V.Y * M.A[1][1] + V.Z * M.A[2][2],
                V.X * M.A[3][0]+ V.Y * M.A[3][1] + V.Z * M.A[3][2]);
}

MATR MatrUnit( VOID );
MATR MatrTranslate( DBL Dx, DBL Dy, DBL Dz );
MATR MatrScale( DBL Sx, DBL Sy, DBL Sz );
MATR MatrRotateX( DBL AngleeDegree );
MATR MatrRotateY( DBL AngleeDegree );
MATR MatrRotateZ( DBL AngleeDegree );
MATR MatrRotate( DBL AngleeDegree, DBL X, DBL Y, DBL Z );
MATR MatrMulMatr( MATR M1, MATR M2 );
DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                     DBL A21, DBL A22, DBL A23,
                     DBL A31, DBL A32, DBL A33 );
DBL MatrDeterm( MATR M );
MATR MatrInverse( MATR M );





/*END OF VEC.H file */