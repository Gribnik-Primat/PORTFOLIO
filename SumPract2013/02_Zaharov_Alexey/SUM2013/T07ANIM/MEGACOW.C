/* Zakharov Alexey, 10-1, Summer Practise 2013 */

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

#include "anim.h"

/* w, h of window */
extern INT w, h;

typedef struct
{
  AZ1_UNIT_BASE_FUNCS;
  FLOAT X, Y; 
  INT RandShift;
  FLT RandScale;
  INT Who; 
} COW;

static UINT CowProg;

/* Vertex array */
VEC *Vertexes;
static VEC *VertexesProj;
INT NumOfVertexes;

/* Facet array */
INT (*Facets)[3];
INT NumOfFacets;

/* Load cow function */
VOID LoadCow( VOID )
{
  FILE *F;
  INT fn = 0, vn = 0;
  static CHAR Buf[1000];

  if ((F = fopen("cube.object", "rt")) == NULL)
    return;

  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      vn++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      fn++;

  if ((Vertexes = malloc(2 * sizeof(VEC) * vn)) == NULL)
  {
    fclose(F);
    return;
  }
  if ((Facets = malloc(sizeof(INT[3]) * fn)) == NULL)
  {
    free(Vertexes);
    fclose(F);
    return;
  }
  NumOfVertexes = vn;
  NumOfFacets = fn;
  VertexesProj = (Vertexes + NumOfVertexes);

  vn = 0;
  fn = 0;
  rewind(F);
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      FLT x, y, z;

      sscanf(Buf + 2, "%f %f %f", &x, &y, &z);
      Vertexes[vn].X = x;
      Vertexes[vn].Y = y;
      Vertexes[vn].Z = z;
      vn++;
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ') //&& Buf[2] == ' ')
    {
      INT n1, n2, n3;

      sscanf(Buf + 2, "%d%d%d", &n1, &n2, &n3);
      Facets[fn][0] = n1 - 1;
      Facets[fn][1] = n2 - 1;
      Facets[fn][2] = n3 - 1;
      fn++;
    }

    fclose(F);
} /* End of 'LoadCow' function */

static VOID CowRender( COW *Unit, az1ANIM *Ani )
{
  FLT t = (FLT)clock() / CLOCKS_PER_SEC;
  INT
    i, j,
    m1,
    LocTime;
  MATR m = 
  {
    {
      {1, 0, 0 ,0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };
  static CHAR Buf[1000];

  t = clock();
  sprintf(Buf, "J: %f %f %f %f %i", Ani->Jx, Ani->Jy, Ani->Jz, Ani->Jr, Ani->Jpov);
  SetWindowText(Ani->hWnd, Buf);
  AZ1_Anim.Mworld = MatrUnit();
  AZ1_Anim.Mview = MatrViewLookAt(VecSet(Ani->Jx, Ani->Jy, Ani->Jz-3), VecSet(0, 0, 0), VecSet(0, -1, 0)); 
  AZ1_Anim.Mproj = MatrProject(-1, 1, -1, 1, 1, 100);
  m = MatrMulMatr(MatrMulMatr(AZ1_Anim.Mworld, AZ1_Anim.Mview), AZ1_Anim.Mproj);

  switch (Ani->Jpov)
  {
  case 1:
    Unit->Y -= 0.1;
    break;
  case 2:
    Unit->X += sqrt(2) * 0.1;
    Unit->Y -= sqrt(2) * 0.1;
    break;
  case 3:
    Unit->X -= 0.1;  
    break;
  case 4:
    Unit->X += sqrt(2) * 0.1;
    Unit->Y += sqrt(2) * 0.1;
    break;
  case 5:
    Unit->Y += 0.1;
    break;
  case 6:
    Unit->X -= sqrt(2) * 0.1;
    Unit->Y += sqrt(2) * 0.1;   
    break;
  case 7:
    Unit->X += 0.1;
    break;
  case 8:
    Unit->Y -= sqrt(2) * 0.1;
    Unit->X -= sqrt(2) * 0.1;  
    break;
  default:
    break;
  };

  CowProg = ShadProgInit("geom.vert", "geom.frag");
  glUseProgram(CowProg);

  LocTime = glGetUniformLocation(CowProg, "Time");
  m1 = glGetUniformLocation(CowProg, "Matr");

  //j = glGetUniformLocation(CowProg, "DrawColor");
  //glUniform4d(j, 1, 0, 0, 1);

  for (i = 0; i < NumOfVertexes; i++)
  {
    VEC v;
    v = Vertexes[i];
  //  v.X += Unit->X;
  //  v.Y += Unit->Y;

  //  VertexesProj[i].X = v.X; 
  //  VertexesProj[i].Y = v.Y;
  //  VertexesProj[i].Z = v.Z;
  }

  for (i = 0; i < NumOfFacets; i++)
  {
 //   VEC p[3];

 //   for (j = 0; j < 3; j++)
 //   {
 //     p[j].X = VertexesProj[Facets[i][j]].X;         
 //     p[j].Y = VertexesProj[Facets[i][j]].Y;
 //     p[j].Z = VertexesProj[Facets[i][j]].Z;
 //   }                                                        
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUniform1f(LocTime, clock());
    glUniformMatrix4fv(m1, 1, FALSE, m.A[0]);
    glBegin(GL_TRIANGLES);
    for (j = 0; j < 3; j++)
    {
      glVertex3f(Vertexes[Facets[i][j]].X + Unit->X, Vertexes[Facets[i][j]].Y + Unit->Y, Vertexes[Facets[i][j]].Z);
    }
    glEnd();
  } 
  glUseProgram(0); 
}

az1UNIT *CowCreate( INT X, INT Y )
{
  COW *Unit;

  if ((Unit = (COW *)AZ1_UnitCreate(sizeof(COW))) == NULL)
    return NULL;

  Unit->Render = (az1UNIT_RENDER)CowRender;
  Unit->X = X;
  Unit->Y = Y;
  Unit->Who = 0;
  Unit->RandShift = rand() % 1000;
  Unit->RandScale = 0.75 + 5.5 * rand() / (FLT)RAND_MAX;
  return (az1UNIT *)Unit;
} /* End of 'CowCreate' function */

/* END OF 'MEGACOW.C' FILE */