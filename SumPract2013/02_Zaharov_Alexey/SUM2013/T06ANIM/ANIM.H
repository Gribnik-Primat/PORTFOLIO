/* Zakharov Alexey,  10-1, Summer Practise 2013 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"
#include "vec.h"

typedef struct tagaz1ANIM
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
  MATR Mworld, Mview, Mproj;
  INT Ws, Hs;
  DBL PD, Wp, Hp;
} az1ANIM;

typedef struct tagaz1UNIT az1UNIT;

typedef VOID (*az1UNIT_INIT)( az1UNIT *Unit, az1ANIM *Ani );
typedef VOID (*az1UNIT_CLOSE)( az1UNIT *Unit );
typedef VOID (*az1UNIT_RESPONSE)( az1UNIT *Unit, az1ANIM *Ani );
typedef VOID (*az1UNIT_RENDER)( az1UNIT *Unit, az1ANIM *Ani );


#define AZ1_UNIT_BASE_FUNCS \
  az1UNIT_INIT Init;         \
  az1UNIT_CLOSE Close;       \
  az1UNIT_RESPONSE Response; \
  az1UNIT_RENDER Render


struct tagaz1UNIT
{
  AZ1_UNIT_BASE_FUNCS;
};

static az1ANIM AZ1_Anim;

BOOL AZ1_AnimInit( HWND hWnd );

VOID AZ1_AnimClose( VOID );

VOID AZ1_AnimResize( INT W, INT H );

VOID AZ1_AnimRender( VOID );

VOID AZ1_AnimCopyFrame( VOID );

az1UNIT * AZ1_UnitCreate( INT Size );

VOID AZ1_AnimAdd( az1UNIT *Unit );

az1UNIT * CowCreate( INT X, INT Y );



#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */