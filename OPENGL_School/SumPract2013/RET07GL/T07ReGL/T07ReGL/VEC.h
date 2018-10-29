               /*
* FILE NAME: vec.h
* PROGRAMMER: bg3
* DATE: 05.06.2013
* PURPOSE: Vector & matrix library definitions
*/

#ifndef __VEC_H_
#define __VEC_H_

#include "def.h"

#define PI 3.14159265358979323846
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)
#define arcsc(s, c) (s >= 0 ? arccos(c) : -arccos(c))

typedef struct tagVEC
{
  FLT X, Y, Z;
} VEC;

typedef struct tagMATR
{
  FLT A[4][4];
} MATR;

VEC VecSet( FLT X, FLT Y, FLT Z );
VEC VecAddVec( VEC V1, VEC V2 );
VEC VecSubVec( VEC V1, VEC V2 );
VEC VecMulNum( VEC V1, FLT N );
VEC VecDivNum( VEC V1, FLT N );
VEC VecNeg( VEC V );
FLT VecDotVec( VEC V1, VEC V2 );
VEC VecCrossVec( VEC V1, VEC V2 );
FLT VecLen2( VEC V );
FLT VecLen( VEC V );
VEC VecNormalize( VEC V );
VEC VecMulMatr( VEC V, MATR M );

MATR MatrSet( FLT * a );
MATR MatrUnit( VOID );
MATR MatrTranslate( FLT Dx, FLT Dy, FLT Dz );
MATR MatrScale( FLT Sx, FLT Sy, FLT Sz );
MATR MatrRotateX( FLT AngleeDegree );
MATR MatrRotateY( FLT AngleeDegree );
MATR MatrRotateZ( FLT AngleeDegree );
MATR MatrRotate( FLT AngleeDegree, FLT X, FLT Y, FLT Z );
MATR MatrFrustum( FLT l, FLT r, FLT u, FLT b, FLT n, FLT f );
MATR MatrLookAt( VEC Loc, VEC At, VEC Up );
MATR MatrMulMatr( MATR M1, MATR M2 );
FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                     FLT A21, FLT A22, FLT A23,
                     FLT A31, FLT A32, FLT A33 );
FLT MatrDeterm( MATR M );
MATR MatrInverse( MATR M );

#endif