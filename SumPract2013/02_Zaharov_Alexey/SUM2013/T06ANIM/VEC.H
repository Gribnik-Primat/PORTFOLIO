
#define _USE_MATH_DEFINES

#include <math.h>
#include <windows.h>

#define D2R(A) ((A) * (M_PI / 180.0))

typedef double DBL;
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

typedef struct tagMATR
{
  DBL A[4][4];
} MATR;


__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC v;
  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
} 

__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
}

__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
}

__inline VEC VecMulNum( VEC V1, DBL N )
{
  return VecSet(V1.X * N, V1.Y * N, V1.Z * N);
}

__inline VEC VecDivNum( VEC V1, DBL N )
{
  return VecSet(V1.X / N, V1.Y / N, V1.Z / N);
}

__inline VEC VecNeg( VEC V )
{
  return VecMulNum(V, -1);
}

__inline DBL VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
}

__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y, 
    V1.Z * V2.X - V1.X * V2.Z,
    V1.X * V2.Y - V1.Y * V2.X);
}

__inline DBL VecLen2( VEC V )
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
}

__inline DBL VecLen( VEC V )
{
  return sqrt(V.X * V.X + V.Y * V.Y + V.Z * V.Z);
}
__inline VEC VecNormalize( VEC V )
{
  return VecDivNum(V, VecLen(V));
}

__inline VEC VecMulMatr( VEC V, MATR M )                  
{
  return VecSet(V.X * M.A[0][0]+ V.Y * M.A[1][0] + V.Z * M.A[2][0],
                V.X * M.A[0][1]+ V.Y * M.A[1][1] + V.Z * M.A[2][1],
                V.X * M.A[0][2]+ V.Y * M.A[1][2] + V.Z * M.A[2][2]);
}

MATR MatrUnit( VOID );
MATR MatrTranslate( DBL Dx, DBL Dy, DBL Dz );
MATR MatrScale( DBL Sx, DBL Sy, DBL Sz );
MATR MatrRotateX( DBL AngleDegree );
MATR MatrRotateY( DBL AngleDegree );
MATR MatrRotateZ( DBL AngleDegree );
MATR MatrRotate( DBL AngleDegree, DBL X, DBL Y, DBL Z );
MATR MatrInverse( MATR M );
DBL MatrDeterm( MATR M );
DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                     DBL A21, DBL A22, DBL A23,
                     DBL A31, DBL A32, DBL A33 );
MATR MatrMulMatr( MATR M1, MATR M2 );
MATR MatrViewLookAt( VEC Loc, VEC At, VEC Up);
MATR MatrProject( DBL l, DBL r, DBL b, DBL t, DBL n, DBL f );
POINT WorldToScreen( VEC P );



