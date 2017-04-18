#include <windows.h>
#include <stdio.h>
#include "heightmap.h"
#include "anim.h"

#define AUTOGEN_DENSITY 150
#define HOUSES_NUM 1

/* Загрузка файла в формате RAW, он же Photoshop RAW */
void LoadRawFile( LPSTR strName, int nSize )
{
  FILE *pFile = NULL;
  pFile = fopen( strName, "rb" );
  if (pFile == NULL) 
  {
    MessageBox(NULL, "Can't find the height map", "Error", MB_OK);
    return;
  } 
  fread( g_HeightMap, 1, sizeof(g_HeightMap), pFile );
  {
    int result = ferror( pFile );
    if (result)
    {
      MessageBox(NULL, "Failed to get data from height map", "Error", MB_ICONERROR);
    }
    fclose(pFile);
  }
}


/* Вырисовываем ландшафт */
void RenderHeightMap( void )
{
  int i, j, x, y, z, IsObject;

  srand(32);
  glEnable(GL_TEXTURE_2D);
  //glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );	

  for (i= 0; i<MAP_SIZE-2; i++)
  {
    for (j=0; j<MAP_SIZE-2; j++)
    { 
      //	TextureNumber = rand() % GROUND_TEXTURES_NUM;
      IsObject = rand() % AUTOGEN_DENSITY;

      x = i, z = j;

      glBegin( GL_TRIANGLE_FAN );	

      y = g_HeightMap[x][z];
      glColor3f( 0, 0, y/200.0f+0.1f );
      glVertex3f( x/2, y/7-5, z/2 );

      y = g_HeightMap[x+2][z];
      glColor3f( 0, 0, y/200.0f+0.1f );
      glVertex3f( (x+2)/2, y/7-5, z/2 );

      y = g_HeightMap[x+2][z+2];
      glColor3f( 0, 0, y/200.0f+0.1f );
      glVertex3f( (x+2)/2, y/7-5, (z+2)/2 );

      y = g_HeightMap[x][z+2];
      glColor3f( 0, 0, y/200.0f+0.1f );
      glVertex3f( x/2, y/7-5, (z+2)/2 );

 /*     if(!IsObject)
      {	
        int HouseNumber = rand() % HOUSES_NUM;
        CHAR Extension[]=".object", FileNameEnding[2], FileName[]={"Models//Houses//house\0\0\0\0\0\0\0\0"};

        /* Строим путь * /
        _itoa( HouseNumber+1, FileNameEnding, 10 );
        strcat( FileName, FileNameEnding );
        strcat( FileName, Extension );
        AZ1_AnimAdd(TestUnitCreate(x, g_HeightMap[x][z], z, FileName));
      } 			
  */

      glEnd();
    }
  }
  //glDisable(GL_TEXTURE_2D);
}