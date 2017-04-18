#define MAP_SIZE 256
#define STEP_SIZE 1
static BYTE g_HeightMap[MAP_SIZE][MAP_SIZE];

void LoadRawFile( LPSTR strName, int nSize );
void RenderHeightMap( void );