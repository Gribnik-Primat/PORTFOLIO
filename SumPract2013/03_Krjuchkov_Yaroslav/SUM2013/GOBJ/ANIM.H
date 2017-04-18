. . .

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
    MapD[MAX_NAME]; /* текстура цвета */     
  UINT TexNo;       /* идентификатор текстуры */
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

. . .
