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
  IB1_PRIM_TRIMESH, IB1_PRIM_GRID
} IB1PRIM_TYPE;

/* тип примитива - группа вершин и индексов */
typedef struct
{
  IB1PRIM_TYPE Type; /* тип примитива */
  INT
    NumOfV,          /* количество вершин */
    NumOfI,          /* количество индексов */
    GW, GH,          /* размер сетки для PRIM_GRID */
    Mat,             /* номер материала */
    Size;            /* Размер структуры в байтах */
  VERTEX *V;         /* массив вершин */
  INT *I;            /* массив индексов */
  UINT Buffs[2], VertBuf; /* OpenGL буфера */
} IB1PRIM;

/* тип представления материала */
#define MAX_NAME 100
typedef struct
{
  VEC Ka, Kd, Ks;   /* коэффициенты освещения */
  FLT Phong, Trans;        /* коэффициент Фонга */
  CHAR
    Name[MAX_NAME], /* имя материала */
    MapD[MAX_NAME]; /* текстура цвета */ 
  INT TexNo;
} IB1MATERIAL;

/* тип представления геометрического объекта - совокупность
 * примитивов и их материалов */
typedef struct
{
  INT NumOfPrims, NumOfMats; /* размеры массивов */
  IB1PRIM *Prims;            /* массив примитивов */
  IB1MATERIAL *Mats;         /* массив материалов */
} IB1GOBJ;

/* Глобальная переменная - цвет вершины по умолчанию */
extern VEC IB1_PrimDefaultColor;

/* Функция создания примитива */
BOOL IB1_PrimCreate( IB1PRIM *Prim, IB1PRIM_TYPE Type,
                     INT NumOfV, INT NumOfI,
                     VERTEX *Vert, INT *Ind );

/* Функция удаления примитива */
VOID IB1_PrimClose( IB1PRIM *Prim );

/* Функция копирования примитива */
BOOL IB1_PrimDup( IB1PRIM *Prim, IB1PRIM *PrimSrc );

/* Функция рисования примитива */
VOID IB1_PrimDraw( IB1PRIM *Prim );

VOID IB1_GeomDraw( IB1GOBJ *Go );

VOID IB1_GeomClose( IB1GOBJ *Go );

BOOL IB1_GeomAddPrim( IB1GOBJ *Go, IB1PRIM *Prim );

BOOL IB1_GeomLoad( IB1GOBJ *Go, CHAR *FileName );

BOOL IB1_GeomAddMat( IB1GOBJ *Go, IB1MATERIAL *Mat );

BOOL IB1_PrimCreatePlane( IB1PRIM *Prim, INT W, INT H,
                          VEC Loc, VEC Du, VEC Dv );

BOOL IB1_PrimCreateHeightField( IB1PRIM *Prim, CHAR *FileName, VEC Loc, VEC Du, VEC Dv );

VOID LoadMaterials( IB1GOBJ *Go, CHAR *FileName );


