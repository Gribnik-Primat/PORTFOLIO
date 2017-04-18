#include <windows.h>
#include <stdio.h>

#include "heightmap.h"
#include "anim.h"

#define AUTOGEN_DENSITY 10

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
  int i, j, x, y, z, TextureNumber, IsTree;

  srand(30);
  //glEnable(GL_TEXTURE_2D);

  for (i= 0; i<MAP_SIZE-2; i++)
  {
    for (j=0; j<MAP_SIZE-2; j++)
    { 
      IsTree = rand() % AUTOGEN_DENSITY;

      x = i, z = j;

      if(!IsTree)
      {	
        /* TODO: нарисовать в таких случаях деревцо */
      } 			

      //glBindTexture( GL_TEXTURE_2D, GroundTextures[TextureNumber]); 
      glBegin( GL_TRIANGLE_FAN );	

      y = g_HeightMap[x][z];
      glColor3f( 0, 0, y/200.0f+0.1f );
      //glTexCoord2f(1, 0);
      glVertex3f( x/2, y/7-5, z/2 );

      y = g_HeightMap[x+2][z];
      glColor3f( 0, 0, y/200.0f+0.1f );
      //glTexCoord2f(0, 0);
      glVertex3f( (x+2)/2, y/7-5, z/2 );

      y = g_HeightMap[x+2][z+2];
      glColor3f( 0, 0, y/200.0f+0.1f );
      //glTexCoord2f(0, 1);
      glVertex3f( (x+2)/2, y/7-5, (z+2)/2 );

      y = g_HeightMap[x][z+2];
      glColor3f( 0, 0, y/200.0f+0.1f );
      //glTexCoord2f(1, 1);
      glVertex3f( x/2, y/7-5, (z+2)/2 );

      glEnd();
    }
  }
  //glDisable(GL_TEXTURE_2D);
}
