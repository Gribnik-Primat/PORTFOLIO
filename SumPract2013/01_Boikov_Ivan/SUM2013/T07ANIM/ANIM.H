/* FILE NAME: ANIM.H
 * PROGRAMMER: IB1
 * DATE: 06.06.2013
 * PURPOSE: Main animation declaration module.
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"
#include "vec.h"
#include "shader.h"

INT64
  TimeFreq,  /* единиц измерения времени в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время предыдущего кадра */
  TimePause, /* время простоя в паузу */
  TimeShift, /* время сдвига */
  TimeFPS;   /* время для замера FPS - frame-per-second */
LONG FrameCount;

typedef struct tagIB1ANIM
{
  HWND
    hWnd;  
  HGLRC
    hGLRC;
  HDC 
    hDC;
  INT
    W, H;      
  DWORD
    *Bits;  
  BYTE
    Keys[256], OldKeys[256];
  INT
    MouseX, MouseY, MouseDeltaX, MouseDeltaY, MouseWheel, MouseBut[10], MouseButOld[10];
  BYTE 
    JBut[32], JButOld[32];
  DBL
    Jx, Jy, Jz, Jr;   /*Joystick controls*/
  INT
    Jpov;
  DBL
    Time,   
    DeltaTime,
    GlobalTime,
    GlobalDeltaTime, 
    FPS;             
  BOOL
    IsPause;
  VEC 
    CameraLoc,
    CameraDir,
    CameraUp,
    CameraRight,
    CameraLookAt;
  MATR
    Mworld, Mview, Mproj;
  INT
    Ws, Hs;
  DBL 
    PD, Wp, Hp;
  DBL
    dx, dy, dz;
  DBL
    alpha, betha, gamma;
} IB1ANIM;

typedef struct tagIB1UNIT IB1UNIT;

typedef VOID (*IB1UNIT_INIT)( IB1UNIT *Unit, IB1ANIM *Ani );
typedef VOID (*IB1UNIT_CLOSE)( IB1UNIT *Unit );
typedef VOID (*IB1UNIT_RESPONSE)( IB1UNIT *Unit, IB1ANIM *Ani );
typedef VOID (*IB1UNIT_RENDER)( IB1UNIT *Unit, IB1ANIM *Ani );


#define IB1_UNIT_BASE_FUNCS \
  IB1UNIT_INIT Init;         \
  IB1UNIT_CLOSE Close;       \
  IB1UNIT_RESPONSE Response; \
  IB1UNIT_RENDER Render


struct tagIB1UNIT
{
  IB1_UNIT_BASE_FUNCS;
};

BOOL IB1_AnimInit( HWND hWnd );

VOID IB1_AnimClose( VOID );

VOID IB1_AnimResize( INT W, INT H );

VOID IB1_AnimRender( VOID );

VOID IB1_AnimCopyFrame( VOID );

IB1UNIT * IB1_UnitCreate( INT Size );

VOID IB1_AnimAdd( IB1UNIT *Unit );

IB1UNIT * CowCreate( FLT X, FLT Y, FLT Z );

IB1UNIT * TestUnitCreate( DBL X, DBL Y, DBL Z, CHAR *FN );
VOID Plane (VOID);


#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */