#define    MAP_SIZE  256    
#define    STEP_SIZE  1     

#include "anim.h"

static BYTE g_HeightMap[MAP_SIZE][MAP_SIZE];    /* �������� ������, ���������� ����� ����� */
static ik1PRIM g_HeightMap_read[MAP_SIZE][MAP_SIZE];
void LoadRawFile( LPSTR strName, int nSize );
void DrawHeightmap( void );