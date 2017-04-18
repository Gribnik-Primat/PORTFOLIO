/* FILE NAME: VEC.H
 * PROGRAMMER: IB1
 * DATE: 05.06.2013
 * PURPOSE: Vector and matrix library.
 */



#define _USE_MATH_DEFINES

#include <math.h>
#include <windows.h>

#define D2R(A) ((A) * (M_PI / 180.0))

typedef double DBL;
typedef float FLT;
typedef struct tagVEC
{
  FLT X, Y, Z;
} VEC;

typedef struct tagMATR
{
  FLT A[4][4];
} MATR;


__inline VEC VecSet( FLT X, FLT Y, FLT Z )
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

__inline VEC VecMulNum( VEC V1, FLT N )
{
  return VecSet(V1.X * N, V1.Y * N, V1.Z * N);
}

__inline VEC VecDivNum( VEC V1, FLT N )
{
  return VecSet(V1.X / N, V1.Y / N, V1.Z / N);
}

__inline VEC VecNeg( VEC V )
{
  return VecMulNum(V, -1);
}

__inline FLT VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
}

__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y, 
    V1.Z * V2.X - V1.X * V2.Z,
    V1.X * V2.Y - V1.Y * V2.X);
}

__inline FLT VecLen2( VEC V )
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
}

__inline FLT VecLen( VEC V )
{
  return sqrt(V.X * V.X + V.Y * V.Y + V.Z * V.Z);
}
__inline VEC VecNormalize( VEC V )
{
  return VecDivNum(V, VecLen(V));
}

__inline VEC VecMulMatr( VEC V, MATR M )                  
{
  FLT W = V.X * M.A[0][3]+ V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3]; 

  return VecSet((V.X * M.A[0][0]+ V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / W,
                (V.X * M.A[0][1]+ V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / W,
                (V.X * M.A[0][2]+ V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / W);
}

MATR MatrUnit( VOID );
MATR MatrTranslate( FLT Dx, FLT Dy, FLT Dz );
MATR MatrScale( FLT Sx, FLT Sy, FLT Sz );
MATR MatrRotateX( FLT AngleDegree );
MATR MatrRotateY( FLT AngleDegree );
MATR MatrRotateZ( FLT AngleDegree );
MATR MatrRotate( FLT AngleDegree, FLT X, FLT Y, FLT Z );
MATR MatrInverse( MATR M );
FLT MatrDeterm( MATR M );
FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                     FLT A21, FLT A22, FLT A23,
                     FLT A31, FLT A32, FLT A33 );
MATR MatrMulMatr( MATR M1, MATR M2 );
MATR MatrViewLookAt( VEC Loc, VEC At, VEC Up);
MATR MatrProject( FLT l, FLT r, FLT b, FLT t, FLT n, FLT f );
//IB1_POINT WorldToScreen( VEC P );