                 /*
* FILE NAME: anim.h
* PROGRAMMER: bg3
* DATE: 06.06.2013
* PURPOSE: Animation system headers
*/

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"
#include "vec.h"


typedef struct
{
  FLT U, V; /* текстурные координаты (в OpenGL -> st[vq]) */
} UV;
__inline UV UVSet( FLT U, FLT V )
{
  UV r;

  r.U = U;
  r.V = V;
  return r;
} /* End of 'UVSet' function */

/* тип представления вершины */
typedef struct
{
  VEC P; /* позиция вершины */
  UV T;  /* текстурные координаты */
  VEC N; /* нормаль вершины */
  VEC C; /* цвет вершины */
} VERTEX;

/* виды примитивов */
typedef enum
{
  BG3_PRIM_TRIMESH, BG3_PRIM_GRID
} bg3PRIM_TYPE;

/* тип примитива - группа вершин и индексов */
typedef struct
{
  bg3PRIM_TYPE Type; /* тип примитива */
  INT
    NumOfV,          /* количество вершин */
    NumOfI,          /* количество индексов */
    GW, GH,          /* размер сетки для PRIM_GRID */
    Mat,             /* номер материала */
    Size;            /* Размер структуры в байтах */
  VERTEX *V;         /* массив вершин */
  INT *I;            /* массив индексов */
  UINT Buffs[2], VertBuf; /* OpenGL буфера */
} bg3PRIM;

/* тип представления материала */
#define MAX_NAME 100
typedef struct
{
  VEC Ka, Kd, Ks;   /* коэффициенты освещения */
  FLT Phong;        /* коэффициент Фонга */
  FLT Trans;        /* коэффициент прозрачности */
  CHAR
    Name[MAX_NAME], /* имя материала */
    MapD[MAX_NAME]; /* текстура цвета */     
  UINT TexNo;       /* идентификатор текстуры */
} bg3MATERIAL;

/* тип представления геометрического объекта - совокупность
 * примитивов и их материалов */
typedef struct
{
  INT NumOfPrims, NumOfMats,X,Y,Z; /* размеры массивов */
  bg3PRIM *Prims;            /* массив примитивов */
  bg3MATERIAL *Mats;         /* массив материалов */
} bg3GOBJ;


typedef struct CameraState
{
  /* Camera position */
  VEC Pos;
  /* Camera up */
  VEC Up;
  /* Camera looks at */
  VEC At;
  /* Perspctive view */
  DBL PD;
  /* Projection plane W and H */
  DBL Wh;
  DBL Hh;

  MATR viewMatr;
  MATR projMatr;

  MATR MVP;
} CAMERASTATE;

typedef struct tagJoyState
{
  BYTE buttons[32];
  BYTE buttonsOld[32];
  /* Axis position (-1 ~ 1)*/
  DBL x, y, z, r;
  /* PoV state. 0-9 */
  INT pov;
} JOYSTATE;
typedef struct tagKeysState
{
  /* Keys state, 1 - pressed, 0 - released */
  BYTE old[256];
  BYTE actual[256];
} KEYSSTATE;

typedef struct tagMouseState
{
  /* Mouse position */
  INT x, y, deltaX, deltaY;
  /* Wheel rotation */
  INT deltaWheel;
  /* Mouse buttons state, 1 - pressed, 0 - not pressed */
  BYTE buttons[10];
  BYTE oldButtons[10];
} MOUSESTATE;

typedef struct tagTimerState
{
/* All time values stored in seconds */
/* Time since animation system start */
DBL globalTime;
/* Active (not paused) time since animation system start */
DBL activeTime;
/* Time since last render */
    DBL deltaGlobalTime;
/* If not paused - time since last render, otherwise - 0 */
    DBL deltaActiveTime;
/* Time shift for debug purposes */
DBL timeShift;
/* Animation system start time */
DBL animStart;
/* Time from FPS counting second start */
    DBL fpsTime;
} TIMERSTATE;

/* Animation unit */
typedef struct tagbg3UNIT bg3UNIT;

/* Animation context */
typedef struct tagbg3ANIM
{
  /* Platform-specific window */
#ifdef WIN32
  HWND hWnd;
  HDC hDC;
  HGLRC hGLRC;
#endif

  /* Window size */
  INT W, H,cam,angle,AtX,AtY,x,y,z,s;

  /* Input state */
  KEYSSTATE keyState;
  TIMERSTATE timer;
  MOUSESTATE mouse;
  JOYSTATE joy;
  INT fpsCounter;
  INT lastFPS;

  /* Camera */
  CAMERASTATE camera;

  /* Is paused */
  BOOL isPaused;
  
  /* Units list */
  bg3UNIT * units[1000];

  INT unitsNum;
} bg3ANIM;

/* Unit methods */
typedef VOID (*bg3UNIT_INIT)( bg3UNIT *Unit, bg3ANIM *Ani );
typedef VOID (*bg3UNIT_CLOSE)( bg3UNIT *Unit );
typedef VOID (*bg3UNIT_RESPONSE)( bg3UNIT *Unit, bg3ANIM *Ani );
typedef VOID (*bg3UNIT_RENDER)( bg3UNIT *Unit, bg3ANIM *Ani );

/* Base unit methods */
#define BG3_UNIT_BASE_FUNCS \
bg3UNIT_INIT Init; \
bg3UNIT_CLOSE Close; \
bg3UNIT_RESPONSE Response; \
bg3UNIT_RENDER Render

/* Unit "class" */
struct tagbg3UNIT
{
  BG3_UNIT_BASE_FUNCS;
};

/* Animation system functions */

/* Platform-specific init */
#ifdef WIN32
BOOL BG3_AnimInit( HWND wnd );
#endif

VOID BG3_AnimClose( VOID );
VOID BG3_AnimWheel( INT delta );
VOID BG3_AnimResize( INT W, INT H );
VOID BG3_AnimRender( VOID );
VOID BG3_AnimCopyFrame( VOID );
VEC World2Screen( VEC point );

/* Create default unit */
bg3UNIT * BG3_UnitCreate( INT Size );

/* Put the unit to scene */
VOID BG3_AnimAdd( bg3UNIT *Unit );

/* Called from main to initialize scene. Platform-independent. */
VOID BG3_SceneCreate( bg3ANIM * anim );
VOID BG3_SceneResponse( bg3ANIM * anim );

/* Is pressed now? */
BOOL BG3_KeyPressed( BYTE key );
/* Is pressed now and not before */

/* Is pressed before? */
BOOL BG3_KeyOldPressed( BYTE key );

extern VEC BG3_PrimDefaultColor;

/* Функция создания примитива */
BOOL BG3_PrimCreate( bg3PRIM *Prim, bg3PRIM_TYPE Type,
                     INT NumOfV, INT NumOfI,
                     VERTEX *Vert, INT *Ind );

/* Функция удаления примитива */
VOID BG3_PrimClose( bg3PRIM *Prim );

/* Функция копирования примитива */
BOOL BG3_PrimDup( bg3PRIM *Prim, bg3PRIM *PrimSrc );

/* Функция рисования примитива */
VOID BG3_PrimDraw( bg3PRIM *Prim );

/* Функция создания примитива плоскость */
BOOL BG3_PrimCreatePlane( bg3PRIM *Prim, INT W, INT H,
                          VEC Loc, VEC Du, VEC Dv );

/* Функция создания примитива плоскость */
BOOL BG3_PrimCreateSphere( bg3PRIM *Prim, INT W, INT H,
                           VEC Loc, FLT Radius );

BOOL BG3_PrimCreateHeightField(bg3PRIM *Prim,CHAR *FileName,VEC Loc, VEC Du, VEC Dv );

/* Функция загрузки геометрического объекта */
BOOL BG3_GeomLoad( bg3GOBJ *Go, CHAR *FileName );

/* Функция удаления геометрического объекта */
VOID BG3_GeomClose( bg3GOBJ *Go );

/* Функция добавления примитива к геометрическому объекту */
BOOL BG3_GeomAddPrim( bg3GOBJ *Go, bg3PRIM *Prim );

/* Функция добавления материала к геометрическому объекту */
BOOL BG3_GeomAddMat( bg3GOBJ *Go, bg3MATERIAL *Mat );

/* Функция рисования геометрического объекта */
VOID BG3_GeomDraw( bg3GOBJ *Go );

BOOL BG3_GeomAddMat( bg3GOBJ *Go, bg3MATERIAL *Mat );

VOID LoadMaterials( bg3GOBJ *Go, CHAR *FileName );



#endif