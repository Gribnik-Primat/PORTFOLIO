/* FILE NAME: ANIM.H
 * PROGRAMMER: VG4
 * DATE: 14.06.2013
 * PURPOSE: Main animation declaration module.
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "vec.h"
#include "shader.h"

/* Тип представления контектса анимации */
typedef struct tagvg4ANIM
{
  HWND hWnd;      /* Рабочее окно анимации */
  HDC hDC;        /* Контекст кадра в памяти */
  INT W, H;       /* Размеры кадра анимации */
  HGLRC hGLRC;    /* Контекст рендеринга */

  /* Данные устройств ввода */
  BYTE Keys[256], KeysOld[256]; /* Клавиши клавиатуры */

  INT
    MouseX, MouseY,                /* Позиция курсора мыши */
    MouseDeltaX, MouseDeltaY,      /* Относительный сдвиг курсора мыши */
    MouseWheel,                    /* Относительный сдвиг колеса мыши */
    MouseBut[10], MouseButOld[10]; /* Состояние кнопок мыши */

  BYTE JBut[32], JButOld[32]; /* Кнопки джойстика */
  DBL Jx, Jy, Jz, Jr;         /* Оси джойстика */
  INT JPov;                   /* Point-of-View 0-8 джойстика */

  /* Данные для времени и синхронизации */
  DBL
    Time,            /* время в сукундых со старта программы */
    DeltaTime,       /* время в секундах с прошлого кадра */
    GlobalTime,      /* время в сукундых со старта программы без паузы */
    GlobalDeltaTime, /* время в секундах с прошлого кадра без паузы */
    FPS;             /* количество кадров в секунду */
  BOOL
    IsPause;         /* флаг паузы */

  /* Параметры камеры и проецирования */
  MATR
    MatrWorld,    /* преобразования в мировой СК */
    MatrView,     /* видовые преобразования */
    MatrProj;     /* проекционные преобразования */
  DBL PD, Wp, Hp; /* параметры плоскости проекции */
  VEC
    CameraLoc,	  /* позиция камеры */
    CameraDir,    /* направление камеры вперед */
    CameraUp,     /* направление камеры вверх */
    CameraRight,  /* направление камеры вправо */
    CameraLookAt; /* "точка интереса" камеры */
} vg4ANIM;

/*** Типы внутренних функций-методов объектов-единиц анимации ***/
typedef struct tagvg4UNIT vg4UNIT;

/* Функция инициализации объекта */
typedef VOID (*vg4UNIT_INIT)( vg4UNIT *Unit, vg4ANIM *Ani );
/* Функция деинициализации объекта */
typedef VOID (*vg4UNIT_CLOSE)( vg4UNIT *Unit );
/* Функция реакции на межкадровое взаимодействие объекта */
typedef VOID (*vg4UNIT_RESPONSE)( vg4UNIT *Unit, vg4ANIM *Ani );
/* Функция отрисовки объекта */
typedef VOID (*vg4UNIT_RENDER)( vg4UNIT *Unit, vg4ANIM *Ani );


/* Функции объектов */
#define VG4_UNIT_BASE_FUNCS \
  vg4UNIT_INIT Init;         \
  vg4UNIT_CLOSE Close;       \
  vg4UNIT_RESPONSE Response; \
  vg4UNIT_RENDER Render


/* Тип представления актоивной единицы анимации */
struct tagvg4UNIT
{
  VG4_UNIT_BASE_FUNCS;
};

/* Значение сдвига колеса мыши */
extern INT VG4_MouseWheel;

/* Глобальный контекст */
extern vg4ANIM VG4_Anim;

/* Функция инициализации анимации */
BOOL VG4_AnimInit( HWND hWnd );

/* Функция деинициализации анимации */
VOID VG4_AnimClose( VOID );

/* Функция обработки изменение размера кадра */
VOID VG4_AnimResize( INT W, INT H );

/* Функция построения кадра */
VOID VG4_AnimRender( VOID );

/* Функция копирования рабочего кадра в окно */
VOID VG4_AnimCopyFrame( VOID );

/* Функция сроздания нового объекта-единицы анимации */
vg4UNIT * VG4_UnitCreate( INT Size );

/* Функция добавления объекта анимации к системе */
VOID VG4_AnimAdd( vg4UNIT *Unit );

/* Функция перехода в полноэкранный режим */
BOOL VG4_SetFullScreen( BOOL FullScreenFlag );

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
  VG4_PRIM_TRIMESH, VG4_PRIM_GRID
} vg4PRIM_TYPE;

/* тип примитива - группа вершин и индексов */
typedef struct
{
  vg4PRIM_TYPE Type; /* тип примитива */
  INT
    NumOfV,          /* количество вершин */
    NumOfI,          /* количество индексов */
    GW, GH,          /* размер сетки для PRIM_GRID */
    Mat,             /* номер материала */
    Size;            /* Размер структуры в байтах */
  VERTEX *V;         /* массив вершин */
  INT *I;            /* массив индексов */
  UINT Buffs[2], VertBuf; /* OpenGL буфера */
} vg4PRIM;

/* тип представления материала */
#define MAX_NAME 100
typedef struct
{
  VEC Ka, Kd, Ks;   /* коэффициенты освещения */
  FLT Phong;        /* коэффициент Фонга */
  FLT Trans;        /* коэффициент прозрачности */
  CHAR
    Name[MAX_NAME], /* имя материала */
    MapD[MAX_PATH], /* текстура цвета */     
    Tex2[MAX_PATH]; /* вторая текстура */     
  UINT TexNo;       /* идентификатор текстуры */
  UINT TexNo2;       /* идентификатор текстуры */
} vg4MATERIAL;

/* тип представления геометрического объекта - совокупность
 * примитивов и их материалов */
typedef struct
{
  INT NumOfPrims, NumOfMats; /* размеры массивов */
  vg4PRIM *Prims;            /* массив примитивов */
  vg4MATERIAL *Mats;         /* массив материалов */
} vg4GOBJ;

/* Глобальная переменная - цвет вершины по умолчанию */
extern VEC VG4_PrimDefaultColor;

/* Функция создания примитива */
BOOL VG4_PrimCreate( vg4PRIM *Prim, vg4PRIM_TYPE Type,
                     INT NumOfV, INT NumOfI,
                     VERTEX *Vert, INT *Ind );

/* Функция удаления примитива */
VOID VG4_PrimClose( vg4PRIM *Prim );

/* Функция копирования примитива */
BOOL VG4_PrimDup( vg4PRIM *Prim, vg4PRIM *PrimSrc );

/* Функция рисования примитива */
VOID VG4_PrimDraw( vg4PRIM *Prim );

/* Функция создания примитива плоскость */
BOOL VG4_PrimCreatePlane( vg4PRIM *Prim, INT W, INT H,
                          VEC Loc, VEC Du, VEC Dv );

/* Функция создания примитива плоскость */
BOOL VG4_PrimCreateSphere( vg4PRIM *Prim, INT W, INT H,
                           VEC Loc, FLT Radius );

/* Функция создания примитива рельеф */
BOOL VG4_PrimCreateHeightField( vg4PRIM *Prim, CHAR *FileName,
                                VEC Loc, VEC Du, VEC Dv );

                                /* Функция загрузки геометрического объекта */
BOOL VG4_GeomLoad( vg4GOBJ *Go, CHAR *FileName );

/* Функция удаления геометрического объекта */
VOID VG4_GeomClose( vg4GOBJ *Go );

/* Функция добавления примитива к геометрическому объекту */
BOOL VG4_GeomAddPrim( vg4GOBJ *Go, vg4PRIM *Prim );

/* Функция добавления материала к геометрическому объекту */
BOOL VG4_GeomAddMat( vg4GOBJ *Go, vg4MATERIAL *Mat );

/* Функция рисования геометрического объекта */
VOID VG4_GeomDraw( vg4GOBJ *Go );

/* Функция загрузки изображения из файла */
BOOL VG4_ImageLoad( DWORD **Bits, INT *W, INT *H, CHAR *FileName );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
