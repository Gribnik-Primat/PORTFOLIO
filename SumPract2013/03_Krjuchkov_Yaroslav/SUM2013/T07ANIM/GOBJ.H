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
  CHAR
    Name[MAX_NAME], /* имя материала */
    MapD[MAX_NAME]; /* текстура цвета */     
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

BOOL IB1_GeomAddMat( IB1GOBJ *Go, IB1MATERIAL *Mat );

VOID IB1_GeomClose( IB1GOBJ *Go );


