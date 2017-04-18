/* Zakharov Alexey,  10-1, Summer Practise 2013 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"
#include "vec.h"
#include "shader.h"

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


typedef struct tagaz1ANIM
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
  MATR Mworld, Mview, Mproj;
  INT Ws, Hs;
  FLT PD, Wp, Hp;
  INT alpha, beta, gamma;
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

static az1ANIM AZ1_Anim = {0};

BOOL AZ1_AnimInit( HWND hWnd );

VOID AZ1_AnimClose( VOID );

VOID AZ1_AnimResize( INT W, INT H );

VOID AZ1_AnimRender( VOID );

VOID AZ1_AnimCopyFrame( VOID );

az1UNIT * AZ1_UnitCreate( INT Size );

VOID AZ1_AnimAdd( az1UNIT *Unit );

az1UNIT * CowCreate( INT X, INT Y );

/***
 * Функции работы с примитивами и геометрическими объектами
 ***/

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
  AZ1_PRIM_TRIMESH, AZ1_PRIM_GRID
} az1PRIM_TYPE;

/* тип примитива - группа вершин и индексов */
typedef struct
{
  az1PRIM_TYPE Type; /* тип примитива */
  INT
    NumOfV,          /* количество вершин */
    NumOfI,          /* количество индексов */
    GW, GH,          /* размер сетки для PRIM_GRID */
    Mat,             /* номер материала */
    Size;            /* Размер структуры в байтах */
  VERTEX *V;         /* массив вершин */
  INT *I;            /* массив индексов */
  UINT Buffs[2], VertBuf; /* OpenGL буфера */
} az1PRIM;

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
} az1MATERIAL;

/* тип представления геометрического объекта - совокупность
 * примитивов и их материалов */
typedef struct
{
  INT NumOfPrims, NumOfMats; /* размеры массивов */
  az1PRIM *Prims;            /* массив примитивов */
  az1MATERIAL *Mats;         /* массив материалов */
} az1GOBJ;

/* Глобальная переменная - цвет вершины по умолчанию */
extern VEC AZ1_PrimDefaultColor;

/* Функция создания примитива */
BOOL AZ1_PrimCreate( az1PRIM *Prim, az1PRIM_TYPE Type,
                     INT NumOfV, INT NumOfI,
                     VERTEX *Vert, INT *Ind );

/* Функция удаления примитива */
VOID AZ1_PrimClose( az1PRIM *Prim );

/* Функция копирования примитива */
BOOL AZ1_PrimDup( az1PRIM *Prim, az1PRIM *PrimSrc );

/* Функция рисования примитива */
VOID AZ1_PrimDraw( az1PRIM *Prim );

/* Функция создания примитива плоскость */
BOOL AZ1_PrimCreatePlane( az1PRIM *Prim, INT W, INT H,
                          VEC Loc, VEC Du, VEC Dv );

/* Функция создания примитива плоскость */
BOOL AZ1_PrimCreateSphere( az1PRIM *Prim, INT W, INT H,
                           VEC Loc, FLT Radius );

/* Функция загрузки геометрического объекта */
BOOL AZ1_GeomLoad( az1GOBJ *Go, CHAR *FileName );

/* Функция удаления геометрического объекта */
VOID AZ1_GeomClose( az1GOBJ *Go );

/* Функция добавления примитива к геометрическому объекту */
BOOL AZ1_GeomAddPrim( az1GOBJ *Go, az1PRIM *Prim );

/* Функция добавления материала к геометрическому объекту */
BOOL AZ1_GeomAddMat( az1GOBJ *Go, az1MATERIAL *Mat );

/* Функция рисования геометрического объекта */
VOID AZ1_GeomDraw( az1GOBJ *Go );

az1UNIT * TestUnitCreate( FLT X, FLT Y, FLT Z, CHAR *FileName );


#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */