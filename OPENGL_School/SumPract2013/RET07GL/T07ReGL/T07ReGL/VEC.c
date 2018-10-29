/*
* FILE NAME: vec.c
* PROGRAMMER: bg3
* DATE: 06.06.2013
* PURPOSE: Vector & Matrix library implementation
*/

#include <math.h>
#include "vec.h"

static MATR UnitMatrix =
{
  {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
  }
};

/*
Vector functions
*/
VEC VecSet( FLT X, FLT Y, FLT Z )
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
}

VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
}

VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
}

VEC VecMulNum( VEC V1, FLT N )
{
  return VecSet(V1.X * N, V1.Y * N, V1.Z * N);
}

VEC VecDivNum( VEC V1, FLT N )
{
  return VecSet(V1.X / N, V1.Y / N, V1.Z / N);
}

VEC VecNeg( VEC V1 )
{
  return VecSet(-V1.X, -V1.Y, -V1.Z);
}

FLT VecDotVec( VEC V1, VEC V2 )
{
  return (V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z);
}

VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(
    V1.Y * V2.Z - V1.Z * V2.Y,
    V1.Z * V2.X - V1.X * V2.Z,
    V1.X * V2.Y - V1.Y * V2.X
  );
}

FLT VecLen2( VEC V )
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
}

FLT VecLen( VEC V )
{
  return sqrt(VecLen2(V));
}

VEC VecNormalize( VEC V )
{
  double len = VecLen(V);
  return VecSet(V.X / len, V.Y / len, V.Z / len);
}

VEC VecMulMatr( VEC V, MATR M )
{
  FLT w = (V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3]);
  return VecSet(
    (V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
    (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
    (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w
  );
}


/*
* Matrix functions
*/
MATR MatrUnit( VOID )
{
  return UnitMatrix;
}


MATR MatrSet( FLT * a )
{
  INT i, j;
  MATR m;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      m.A[i][j] = a[4 * i + j];
  return m;
}

MATR MatrTranslate( FLT Dx, FLT Dy, FLT Dz )
{
  MATR m = UnitMatrix;

  m.A[3][0] = Dx;
  m.A[3][1] = Dy;
  m.A[3][2] = Dz;
  return m;
}

MATR MatrScale( FLT Sx, FLT Sy, FLT Sz )
{
  MATR m = UnitMatrix;
  m.A[0][0] = Sx;
  m.A[1][1] = Sy;
  m.A[2][2] = Sz;
  return m;
}

void calculateRotLookAt(FLT * out, VEC Right, VEC Up, VEC Dir, VEC Loc)
{
  INT i;
  FLT result[] = {
    Right.X, Up.X, -Dir.X, 0,
    Right.Y, Up.Y, -Dir.Y, 0,
    Right.Z, Up.Z, -Dir.Z, 0,
    0, 0, 0, 1
  };
  for (i = 0; i < 16; i++)
    out[i] = result[i];
}

void calculateFrustum(FLT l, FLT r, FLT b, FLT t, FLT n, FLT f, FLT * out)
{
  INT i;
  FLT result[] = {
    2 * n / (r - l), 0, 0, 0,
    0, 2 * n / (t - b), 0, 0,
    (r + l) / (r - l), (t+b)/(t-b), -(f+n)/(f-n), -1,
    0, 0, -2*n*f/(f-n), 0
  };
  for (i = 0; i < 16; i++)
    out[i] = result[i];
}


MATR MatrFrustum(FLT l, FLT r, FLT b, FLT t, FLT n, FLT f)
{
  FLT arr[16];
  calculateFrustum(l, r, b, t, n, f, arr);
  return MatrSet(arr);
}

MATR MatrLookAt( VEC Loc, VEC At, VEC Up )
{
  FLT rotArray[16];
  VEC Dir = VecSubVec(At, Loc);
  VEC Right = VecCrossVec(Dir, Up);
  MATR rotateMatr, translateMatr;
  Dir = VecNormalize(Dir);
  Right = VecNormalize(Right);
  Up = VecCrossVec(Right, Dir);
  calculateRotLookAt(rotArray, Right, Up, Dir, Loc);
  rotateMatr = MatrSet(rotArray);
  translateMatr = MatrTranslate(-Loc.X, -Loc.Y, -Loc.Z);
  return MatrMulMatr(translateMatr, rotateMatr);
}

MATR MatrRotateX( FLT AngleeDegree )
{
  MATR m = UnitMatrix;
  FLT co = cos(D2R(AngleeDegree));
  FLT si = sin(D2R(AngleeDegree));
  m.A[1][1] = co;
  m.A[2][1] = -si;

  m.A[1][2] = si;
  m.A[2][2] = co;

  return m;
}

MATR MatrRotateY( FLT AngleeDegree )
{
  MATR m = UnitMatrix;
  FLT co = cos(D2R(AngleeDegree));
  FLT si = sin(D2R(AngleeDegree));
  m.A[0][0] = co;
  m.A[2][0] = -si;

  m.A[0][2] = si;
  m.A[2][2] = co;

  return m;
}

MATR MatrRotateZ( FLT AngleeDegree )
{
  MATR m = UnitMatrix;
  FLT co = cos(D2R(AngleeDegree));
  FLT si = sin(D2R(AngleeDegree));
  m.A[0][0] = co;
  m.A[1][0] = -si;

  m.A[0][1] = si;
  m.A[1][1] = co;

  return m;
}


MATR MatrRotate( FLT AngleeDegree, FLT X, FLT Y, FLT Z )
{
  MATR m;
  FLT
    a = D2R(AngleeDegree), si = sin(a), co = cos(a),
    len = X * X + Y * Y + Z * Z;

  if (len != 0 && len != 1)
    len = sqrt(len), X /= len, Y /= len, Z /= len;
  X *= si;
  Y *= si;
  Z *= si;

  m.A[0][0] = 1 - 2 * (Y * Y + Z* Z);
  m.A[0][1] = 2 * X * Y - 2 * co * Z;
  m.A[0][2] = 2 * co * Y + 2 * X * Z;
  m.A[0][3] = 0;
  m.A[1][0] = 2 * X * Y + 2 * co * Z;
  m.A[1][1] = 1 - 2 * (X * X + Z * Z);
  m.A[1][2] = 2 * Y * Z - 2 * co * X;
  m.A[1][3] = 0;
  m.A[2][0] = 2 * X * Z - 2 * co * Y;
  m.A[2][1] = 2 * co * X + 2 * Y * Z;
  m.A[2][2] = 1 - 2 * (X * X + Y * Y);
  m.A[2][3] = 0;
  m.A[3][0] = 0;
  m.A[3][1] = 0;
  m.A[3][2] = 0;
  m.A[3][3] = 1;
  return m;
}


MATR MatrMulMatr( MATR M1, MATR M2 )
{
  INT i, j, k;
  MATR m;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (m.A[i][j] = 0, k = 0; k < 4; k++)
        m.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return m;
}

FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                     FLT A21, FLT A22, FLT A23,
                     FLT A31, FLT A32, FLT A33 )
{
  return A11 * A22 * A33 -
         A11 * A23 * A32 -
         A12 * A21 * A33 +
         A12 * A23 * A31 +
         A13 * A21 * A32 -
         A13 * A22 * A31;
}


FLT MatrDeterm( MATR M )
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

MATR MatrInverse( MATR M )
{
  MATR r;
  FLT det = MatrDeterm(M);

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