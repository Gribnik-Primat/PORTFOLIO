/* FILE NAME: ANIM.H
 * PROGRAMMER: IK1
 * DATE: 06.06.2013
 * PURPOSE: Main animation declaration module.
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"
#include "vec.h"


typedef struct tagik1ANIM
{
  HWND hWnd;  
  HDC hDC;     
  HBITMAP hFrame;
  INT W, H;      
  DWORD *Bits;  
  BYTE Keys[256], OldKeys[256];
  INT
    MouseX, MouseY, MouseDeltaX, MouseDeltaY, MouseWheel, MouseBut[10], MouseButOld[10];
  BYTE JBut[32], JButOld[32];
  DBL Jx, Jy, Jz, Jr;
  INT Jpov;
  VEC
    CameraLoc,
    CameraDir,
    CameraUp,
    CameraRight,
    CameraLookAt;
  MATR 
    MatrWorld,
    MatrView,
    MatrProj;
  DBL Wp, Hp, PD;

} ik1ANIM;

typedef struct tagik1UNIT ik1UNIT;

typedef VOID (*ik1UNIT_INIT)( ik1UNIT *Unit, ik1ANIM *Ani );
typedef VOID (*ik1UNIT_CLOSE)( ik1UNIT *Unit );
typedef VOID (*ik1UNIT_RESPONSE)( ik1UNIT *Unit, ik1ANIM *Ani );
typedef VOID (*ik1UNIT_RENDER)( ik1UNIT *Unit, ik1ANIM *Ani );


#define IK1_UNIT_BASE_FUNCS \
  ik1UNIT_INIT Init;         \
  ik1UNIT_CLOSE Close;       \
  ik1UNIT_RESPONSE Response; \
  ik1UNIT_RENDER Render


struct tagik1UNIT
{
  IK1_UNIT_BASE_FUNCS;
};

BOOL IK1_AnimInit( HWND hWnd );

VOID IK1_AnimClose( VOID );

VOID IK1_AnimResize( INT W, INT H );

VOID IK1_AnimRender( VOID );

VOID IK1_AnimCopyFrame( VOID );

ik1UNIT * IK1_UnitCreate( INT Size );

VOID IK1_AnimAdd( ik1UNIT *Unit );

ik1UNIT * CowCreate( INT X, INT Y );

POINT IK1_AnimWorldToScreen(VEC P);

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */