#include "anim.h"

#define    MAP_SIZE  256     // Размер карты вершин 
#define    STEP_SIZE  1      // Ширина и высота каждого квадрата 

static BYTE g_HeightMap[MAP_SIZE][MAP_SIZE]; 

void LoadRawFile( LPSTR strName, int nSize );
void RenderHeightMap( void );


