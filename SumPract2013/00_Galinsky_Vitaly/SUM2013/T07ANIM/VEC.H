/* FILE NAME: VEC.H
 * PROGRAMMER: VG4
 * DATE: 14.06.2013
 * PURPOSE: Main math support declaration module.
 */

#ifndef __VEC_H_
#define __VEC_H_

#include <math.h>

#include "def.h"

#define VG4_PI 3.14159265358979323846
#define VG4_D2R(A) ((A) * (VG4_PI / 180.0))
#define VG4_Degree2Radian(a) VG4_D2R(a)
#define VG4_R2D(A) ((A) * (180.0 / VG4_PI))
#define VG4_Radian2Degree(a) VG4_R2D(a)

/* множитель для перевода градусов в радианы */
#define VG4_DEGREE2RADIANL 0.01745329251994329576L
static long double VG4_MultiplierDegree2Radian = VG4_DEGREE2RADIANL;

/* базовый вещественный тип */
typedef float FLT;

/* тип для вектора в простанстве */
typedef struct tagVEC
{
  FLT X, Y, Z; /* компоненты вектора */
} VEC;

/* тип для матрицы - массив в структуре */
typedef struct tagMATR
{
  FLT A[4][4]; /* элементы матрицы */
} MATR;

/***
 * Функции работы с векторами
 ***/

__inline VEC VecSet( FLT X, FLT Y, FLT Z )
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
} /* End of 'VecSet' function */
 
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
} /* End of 'VecAddVec' function */

__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
} /* End of 'VecSubVec' function */

__inline VEC VecMulNum( VEC V1, FLT N )
{
  return VecSet(V1.X * N, V1.Y * N, V1.Z * N);
} /* End of 'VecMulNum' function */

__inline VEC VecDivNum( VEC V1, FLT N )
{
  return VecSet(V1.X / N, V1.Y / N, V1.Z / N);
} /* End of 'VecDivNum' function */

__inline VEC VecNeg( VEC V )
{
  return VecSet(-V.X, -V.Y, -V.Z);
} /* End of 'VecNeg' function */

/* скалярное произведение векторов */
__inline FLT VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
} /* End of 'VecDotVec' function */

/* векторное произведение векторов */
__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y,
                V1.Z * V2.X - V1.X * V2.Z,
                V1.X * V2.Y - V1.Y * V2.X);
} /* End of 'VecCrossVec' function */

__inline FLT VecLen2( VEC V )
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
} /* End of 'VecLen2' function */

__inline FLT VecLen( VEC V )
{
  FLT len = V.X * V.X + V.Y * V.Y + V.Z * V.Z;
  if (len != 0 && len != 1)
    return sqrtf(len);
  return len;
} /* End of 'VecLen' function */

__inline VEC VecNormalize( VEC V )
{
  FLT len = V.X * V.X + V.Y * V.Y + V.Z * V.Z;
  if (len != 0 && len != 1)
  {
    len = sqrtf(len);
    return VecSet(V.X / len, V.Y / len,  V.Z / len);
  }
  return V;
} /* End of 'VecNormalize' function */

__inline VEC VecMulMatr( VEC V, MATR M )
{
  FLT w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
} /* End of 'VecMulMatr' function */

/***
 * Функции работы с матрицами
 ***/

/* единичная матрица */
#define VG4_UNIT_MATR \
{                 \
  {               \
    {1, 0, 0, 0}, \
    {0, 1, 0, 0}, \
    {0, 0, 1, 0}, \
    {0, 0, 0, 1}  \
  }               \
}

__inline MATR MatrUnit( VOID )
{
  static MATR m = VG4_UNIT_MATR;

  return m;
} /* End of 'MatrUnit' function */

__inline MATR MatrTranslate( FLT Dx, FLT Dy, FLT Dz )
{
  MATR m = VG4_UNIT_MATR;

  m.A[3][0] = Dx;
  m.A[3][1] = Dy;
  m.A[3][2] = Dz;
  return m;
} /* End of 'MatrTranslate' function */

__inline MATR MatrScale( FLT Sx, FLT Sy, FLT Sz )
{
  MATR m = VG4_UNIT_MATR;

  m.A[0][0] = Sx;
  m.A[1][1] = Sy;
  m.A[2][2] = Sz;
  return m;
} /* End of 'MatrScale' function */

__inline MATR MatrRotateX( FLT AngleInDegree )
{
  FLT sine, cosine;
  MATR m = VG4_UNIT_MATR;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul VG4_MultiplierDegree2Radian

    /* FST(0) - cos, FST(1) - sin */
    fsincos

    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  m.A[1][1] = cosine;
  m.A[2][2] = cosine;
  m.A[1][2] = sine;
  m.A[2][1] = -sine;
  return m;
} /* End of 'MatrRotateX' function */

__inline MATR MatrRotateY( FLT AngleInDegree )
{
  FLT sine, cosine;
  MATR m = VG4_UNIT_MATR;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul VG4_MultiplierDegree2Radian

    /* FST(0) - cos, FST(1) - sin */
    fsincos

    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  m.A[0][0] = cosine;
  m.A[2][2] = cosine;
  m.A[2][0] = sine;
  m.A[0][2] = -sine;
  return m;
} /* End of 'MatrRotateY' function */

__inline MATR MatrRotateZ( FLT AngleInDegree )
{
  FLT sine, cosine;
  MATR m = VG4_UNIT_MATR;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul VG4_MultiplierDegree2Radian

    /* FST(0) - cos, FST(1) - sin */
    fsincos

    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  m.A[0][0] = cosine;
  m.A[1][1] = cosine;
  m.A[0][1] = sine;
  m.A[1][0] = -sine;
  return m;
} /* End of 'MatrRotateZ' function */

__inline MATR MatrRotate( FLT AngleInDegree,
                          FLT X, FLT Y, FLT Z )
{
  FLT sine, cosine, len;
  MATR m = VG4_UNIT_MATR;

  AngleInDegree /= 2;
  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul VG4_MultiplierDegree2Radian

    /* FST(0) - cos, FST(1) - sin */
    fsincos

    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }

  /* определили длину вектора */
  len = X * X + Y * Y + Z * Z;
  if (len != 0 && len != 1)
  {
    len = sqrtf(len);
    X /= len;
    Y /= len;
    Z /= len;
  }

  X *= sine;
  Y *= sine;
  Z *= sine;
  m.A[0][0] = 1 - 2 * (Y * Y + Z * Z);
  m.A[0][1] = 2 * X * Y - 2 * cosine * Z;
  m.A[0][2] = 2 * cosine * Y + 2 * X * Z;
  m.A[0][3] = 0;
  m.A[1][0] = 2 * X * Y + 2 * cosine * Z;
  m.A[1][1] = 1 - 2 * (X * X + Z * Z);
  m.A[1][2] = 2 * Y * Z - 2 * cosine * X;
  m.A[1][3] = 0;
  m.A[2][0] = 2 * X * Z - 2 * cosine * Y;
  m.A[2][1] = 2 * cosine * X + 2 * Y * Z;
  m.A[2][2] = 1 - 2 * (X * X + Y * Y);
  m.A[2][3] = 0;
  m.A[3][0] = 0;
  m.A[3][1] = 0;
  m.A[3][2] = 0;
  m.A[3][3] = 1;
  return m;
} /* End of 'MatrRotate' function */
 
__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  INT i, j, k;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (r.A[i][j] = 0, k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
} /* End of 'MatrMulMatr' function */
 
__inline MATR MatrTranspose( MATR M )
{
  INT i, j;
  MATR r;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[i][j] = M.A[j][i];
  return r;
} /* End of 'MatrTranspose' function */

__inline FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                            FLT A21, FLT A22, FLT A23,
                            FLT A31, FLT A32, FLT A33 )
{
  return A11 * A22 * A33 -
         A11 * A23 * A32 -
         A12 * A21 * A33 +
         A12 * A23 * A31 +
         A13 * A21 * A32 - 
         A13 * A22 * A31;
} /* End of 'MatrDeterm3x3' function */

__inline FLT MatrDeterm( MATR M )
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
} /* End of 'MatrDeterm' function */

__inline MATR MatrInverse( MATR M )
{
  MATR r;
  FLT det = MatrDeterm(M);

  if (det == 0)
  {
    static MATR m = VG4_UNIT_MATR;

    return m;
  }

  /* строим присоединенную матрицу (adjoint matrix) */
  r.A[0][0] =
    MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][0] =
    MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]);

  r.A[0][1] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[2][0], M.A[2][2], M.A[2][3],
                  M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][1] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[2][0], M.A[2][1], M.A[2][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[2][0], M.A[2][1], M.A[2][2],
                  M.A[3][0], M.A[3][1], M.A[3][2]);

  r.A[0][2] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                 M.A[1][1], M.A[1][2], M.A[1][3],
                 M.A[3][1], M.A[3][2], M.A[3][3]);
  r.A[1][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]);
  r.A[2][2] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]);
  r.A[3][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]);

  r.A[0][3] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3]);
 
  r.A[1][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[2][0], M.A[2][2], M.A[2][3]);
  r.A[2][3] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[2][0], M.A[2][1], M.A[2][3]);
  r.A[3][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[2][0], M.A[2][1], M.A[2][2]);

  /* делим на определитель */
  r.A[0][0] /= det;
  r.A[1][0] /= det;
  r.A[2][0] /= det;
  r.A[3][0] /= det;
  r.A[0][1] /= det;
  r.A[1][1] /= det;
  r.A[2][1] /= det;
  r.A[3][1] /= det;
  r.A[0][2] /= det;
  r.A[1][2] /= det;
  r.A[2][2] /= det;
  r.A[3][2] /= det;
  r.A[0][3] /= det;
  r.A[1][3] /= det;
  r.A[2][3] /= det;
  r.A[3][3] /= det;

  return r;
} /* End of 'MatrInverse' function */

__inline MATR MatrViewLookAt( VEC Loc, VEC At, VEC Up )
{
  VEC
    Dir = VecSubVec(At, Loc),
    Right = VecCrossVec(Dir, Up);
  MATR m;

  Dir = VecNormalize(Dir);
  Right = VecNormalize(Right);
  Up = VecCrossVec(Right, Dir);

  m.A[0][0] = Right.X;
  m.A[1][0] = Right.Y;
  m.A[2][0] = Right.Z;
  m.A[3][0] = -VecDotVec(Right, Loc);
  m.A[0][1] = Up.X;
  m.A[1][1] = Up.Y;
  m.A[2][1] = Up.Z;
  m.A[3][1] = -VecDotVec(Up, Loc);
  m.A[0][2] = -Dir.X;
  m.A[1][2] = -Dir.Y;
  m.A[2][2] = -Dir.Z;
  m.A[3][2] = VecDotVec(Dir, Loc);
  m.A[0][3] = 0;
  m.A[1][3] = 0;
  m.A[2][3] = 0;
  m.A[3][3] = 1;
  return m;
} /* End of 'MatrViewLookAt' function */

__inline MATR MatrProjection( FLT Left, FLT Right, FLT Bottom, FLT Top, FLT Near, FLT Far )
{
  MATR m = {{0}};

  m.A[0][0] = 2 * Near / (Right - Left);
  m.A[1][1] = 2 * Near / (Top - Bottom);
  m.A[2][0] = (Right + Left) / (Right - Left);
  m.A[2][1] = (Top + Bottom) / (Top - Bottom);
  m.A[2][2] = -(Far + Near) / (Far - Near);
  m.A[2][3] = -1;
  m.A[3][2] = -2 * Near * Far / (Far - Near);
  return m;
} /* End of 'MatrProjection' function */

#endif /* __VEC_H_ */

/* END OF 'VEC.H' FILE */
