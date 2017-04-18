/* FILE NAME: ANIM.H
 * PROGRAMMER: IK1
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

typedef struct tagik1ANIM
{
  HWND hWnd;  
  HGLRC hGLRC;
  HDC hDC;
  INT W, H;      
  DWORD *Bits;  
  BYTE Keys[256], OldKeys[256];
  INT
    MouseX, MouseY, MouseDeltaX, MouseDeltaY, MouseWheel, MouseBut[10], MouseButOld[10];
  BYTE JBut[32], JButOld[32];
  FLT Jx, Jy, Jz, Jr;
  INT Jpov;
  FLT
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
  MATR MatrWorld, MatrView, MatrProj;
  INT Ws, Hs;
  FLT PD, Wp, Hp, alpha, betha, gamma;
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
/***
 * Функции работы с примитивами и геометрическими объектами
 ***/

/* тип представления текстурных координат */
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
  IK1_PRIM_TRIMESH, IK1_PRIM_GRID
} ik1PRIM_TYPE;

/* тип примитива - группа вершин и индексов */
typedef struct
{
  ik1PRIM_TYPE Type; /* тип примитива */
  INT
    NumOfV,          /* количество вершин */
    NumOfI,          /* количество индексов */
    GW, GH,          /* размер сетки для PRIM_GRID */
    Mat,             /* номер материала */
    Size;            /* Размер структуры в байтах */
  VERTEX *V;         /* массив вершин */
  INT *I;            /* массив индексов */
  UINT Buffs[2], VertBuf; /* OpenGL буфера */
} ik1PRIM;

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
} ik1MATERIAL;

/* тип представления геометрического объекта - совокупность
 * примитивов и их материалов */
typedef struct
{
  INT NumOfPrims, NumOfMats; /* размеры массивов */
  ik1PRIM *Prims;            /* массив примитивов */
  ik1MATERIAL *Mats;         /* массив материалов */
} ik1GOBJ;

/* Глобальная переменная - цвет вершины по умолчанию */
extern VEC IK1_PrimDefaultColor;

static ik1ANIM IK1_Anim;

BOOL IK1_AnimInit( HWND hWnd );

VOID IK1_AnimClose( VOID );

VOID IK1_AnimResize( INT W, INT H );

VOID IK1_AnimRender( VOID );

VOID IK1_AnimCopyFrame( VOID );

ik1UNIT * IK1_UnitCreate( INT Size );

VOID IK1_AnimAdd( ik1UNIT *Unit );

ik1UNIT * PlaneCreate( FLT X, FLT Y, FLT Z );

ik1UNIT * TestUnitCreate( FLT X, FLT Y, FLT Z );

//static VOID TestInit( TEST *Unit, ik1ANIM *Ani );

//staticVOID TestRender( TEST *Unit, ik1ANIM *Ani );

/* Функция создания примитива */
BOOL IK1_PrimCreate( ik1PRIM *Prim, ik1PRIM_TYPE Type,
                     INT NumOfV, INT NumOfI,
                     VERTEX *Vert, INT *Ind );

/* Функция удаления примитива */
VOID IK1_PrimClose( ik1PRIM *Prim );

/* Функция копирования примитива */
BOOL IK1_PrimDup( ik1PRIM *Prim, ik1PRIM *PrimSrc );

/* Функция рисования примитива */
VOID IK1_PrimDraw( ik1PRIM *Prim );

/* Функция создания примитива плоскость */
BOOL IK1_PrimCreatePlane( ik1PRIM *Prim, INT W, INT H,
                          VEC Loc, VEC Du, VEC Dv );

/* Функция создания примитива плоскость */
BOOL IK1_PrimCreateSphere( ik1PRIM *Prim, INT W, INT H,
                           VEC Loc, FLT Radius );

/* Функция загрузки геометрического объекта */
BOOL IK1_GeomLoad( ik1GOBJ *Go, CHAR *FileName );

/* Функция удаления геометрического объекта */
VOID IK1_GeomClose( ik1GOBJ *Go );

/* Функция добавления примитива к геометрическому объекту */
BOOL IK1_GeomAddPrim( ik1GOBJ *Go, ik1PRIM *Prim );

/* Функция добавления материала к геометрическому объекту */
BOOL IK1_GeomAddMat( ik1GOBJ *Go, ik1MATERIAL *Mat );

/* Функция рисования геометрического объекта */
VOID IK1_GeomDraw( ik1GOBJ *Go );

BOOL IK1_PrimCreateHeightField( ik1PRIM *Prim, CHAR *FileName,
                                VEC Loc, VEC Du, VEC Dv );
VOID LoadMaterials( ik1GOBJ *Go, CHAR *FileName );



#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */