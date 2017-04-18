#include "anim.h"

static MATR UnitMatrix =
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
};

MATR MatrUnit( VOID )
{
  return UnitMatrix;
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

MATR MatrRotateZ( FLT AngleDegree )
{
  MATR m = UnitMatrix;
  DOUBLE si = sin(AngleDegree), co = cos(AngleDegree);

  m.A[0][0] = co;
  m.A[0][1] = -si;
  m.A[1][1] = co;
  m.A[1][0] = si;
  return m;

}

MATR MatrRotateY( FLT AngleDegree )
{
  MATR m = UnitMatrix;
  DOUBLE si = sin(AngleDegree), co = cos(AngleDegree);

  m.A[0][0] = co;
  m.A[0][2] = -si;
  m.A[2][0] = si;
  m.A[2][2] = co;
  return m;
}

MATR MatrRotateX( FLT AngleDegree )
{
  MATR m = UnitMatrix;
  DOUBLE si = sin(AngleDegree), co = cos(AngleDegree);

  m.A[0][0] = co;
  m.A[0][1] = si;
  m.A[1][0] = -si;
  m.A[1][1] = co;
  return m;
}

MATR MatrRotate( FLT AngleDegree, FLT X, FLT Y, FLT Z )
{
  MATR m;
  FLT
    a = D2R(AngleDegree), si = sin(a), co = cos(a),
    len = sqrt(X * X + Y * Y + Z * Z);

  if (len != 0 && len != 1)
    X /= len, Y /= len, Z /= len;
  X *= si;
  Y *= si;
  Z *= si;

  m.A[0][0] = 1 - 2 * (Y * Y + Z* Z);
  m.A[0][1] = 2 * X * Y - 2 * co * Z;
  m.A[0][2] = 2 *co * Y - 2 * co * Z;
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
  MATR M;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (M.A[i][j] = 0, k = 0; k < 4; k++)
        M.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return M;
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


MATR MatrViewLookAt( VEC Loc, VEC At, VEC Up)
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
}


MATR MatrProject( FLT l, FLT r, FLT b, FLT t, FLT n, FLT f )
{
  MATR m = MatrUnit();
  m.A[0][0] = (2 * n) / (r - l);
  m.A[2][0] = (r + l) / (r - l);
  m.A[1][1] = (2 * n) / (t - b);
  m.A[2][1] = -(t + b) / (t - b);
  m.A[2][2] = -(f + n) / (f - n);
  m.A[3][2] = -(2 * n * f) / (f - n);
  m.A[3][3] = 0;
  m.A[2][3] = -1;

  return m;
}


