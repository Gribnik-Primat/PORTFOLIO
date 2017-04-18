
#include "glut.h"
#include <windows.h>
#include <stdio.h>

#include "relief.h"


//extern GLuint GroundTextures[GROUND_TEXTURES_NUM];

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

	srand(32);
	glEnable(GL_TEXTURE_2D);

	for (i= 0; i<MAP_SIZE-2; i++)
	{
		for (j=0; j<MAP_SIZE-2; j++)
		{ 
			x = i, z = j;

			glBindTexture( GL_TEXTURE_2D, GroundTextures[TextureNumber]); 
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
	glDisable(GL_TEXTURE_2D);
}

/*void Flatness (double a, double b, double c, double d) 
{
	/* 4 вершины начального квадрата */
//	CVertex3 QPoint1={0, d, 0}, QPoint2={3, d, 0}, QPoint3={3, d, -3}, QPoint4={-3, d, -3};

	/* TODO: нарисовать 20 или сколько надо квадратов около камеры (5х4) 
	glColor3ub (10, 128, 50);
	glBegin(GL_QUADS);
		glVertex3f(QPoint1.x, QPoint1.y, QPoint1.z);    // Нижний левый угол
		glVertex3f(QPoint2.x, QPoint2.y, QPoint2.z);    // Нижний правый угол
		glVertex3f(QPoint3.x, QPoint3.y, QPoint3.z);    // Верхний правый угол
		glVertex3f(QPoint4.x, QPoint4.y, QPoint4.z);    // Верхний левый угол
	glEnd();				
}       */

/* Основная функция */