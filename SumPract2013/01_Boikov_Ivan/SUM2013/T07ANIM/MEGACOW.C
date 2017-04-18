/* FILE NAME: MEGACOW.C                            
 * PROGRAMMER: IB1
 * DATE: 06.06.2013
 * PURPOSE: Cow animation module.
 */

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

#include "anim.h"

/* w, h of window */
extern INT w, h;

/*Cow state structure*/
typedef struct
{
  IB1_UNIT_BASE_FUNCS;
  FLT X, Y, Z; 
  INT RandShift;
  DBL RandScale;
  INT Who; 
} COW;
static FLT XPos, YPos, ZPos; /*Camera coordinates*/

/*Shader program id*/
static UINT CowProg;

/* Vertex array */
VEC *Vertexes;
static IB1_POINT *VertexesProj;
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

  if ((F = fopen("cube.obj", "rt")) == NULL)
    return;

  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      vn++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      fn++;

  if ((Vertexes = malloc((sizeof(VEC) + sizeof(IB1_POINT)) * vn)) == NULL)
  {
    fclose(F);
    return;
  }
  if ((Facets = malloc(sizeof(INT [3]) * fn)) == NULL)
  {
    free(Vertexes);
    fclose(F);
    return;
  }
  NumOfVertexes = vn;
  NumOfFacets = fn;
  VertexesProj = (IB1_POINT *)(Vertexes + NumOfVertexes);

  vn = 0;
  fn = 0;
  rewind(F);
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      FLT x, y, z;

      sscanf(Buf + 2, "%f%f%f", &x, &y, &z);
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

static VOID CowInit( COW *Unit, IB1ANIM *Ani )
{
  CowProg = ShadProgInit("a.vert", "a.frag");
} /* End of 'CowInit' function */

static VOID CowRender( COW *Unit, IB1ANIM *Ani )
{

  //static UINT CowProg;


  DBL t = (DBL)clock() / CLOCKS_PER_SEC;
  INT
    i, j, id;
  MATR m = 
  {
    {
      {1, 0, 0 ,0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  Ani->Mworld = MatrScale( 100000000000, 0.5, 100000000000);
  //Ani->Mview = MatrTranslate(Ani->XPos + 5, Ani->YPos + 5, Ani->ZPos + 5);
  /* IB1_Anim.Mview = MatrViewLookAt(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0)); */
  //Ani->Mview = MatrViewLookAt(VecSet(Ani->dx, Ani->dy, Ani->dz + 10), VecSet(0, 0, 0), VecSet(0, 1, 0));
  Ani->Mproj = MatrProject(-1, 1, -1, 1, 1, 100);
  Ani->Mview = MatrUnit();

  m = MatrMulMatr(MatrMulMatr(Ani->Mworld, Ani->Mview), Ani->Mproj);
  /*switch (Ani->Jpov)
  {
  case 1:
    Unit->Y -= 0.1;
    break;
  case 2:
    Unit->X += sqrt(2) * 0.1;
    Unit->Y -= sqrt(2) * 0.1;
    break;
  case 3:
    Unit->X += 0.1;  
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
    Unit->X -= 0.1;
    break;
  case 8:
    Unit->Y -= sqrt(2) * 0.1;
    Unit->X -= sqrt(2) * 0.1;  
    break;
  default:
    break;
  };*/
  
  for (i = 0; i < NumOfVertexes; i++)
  {
    VEC v;

    v = Vertexes[i];
    v.X += Unit->X;
    v.Y += Unit->Y;
    v.Z += 10;
  } 

  glUseProgram(CowProg);
  id = glGetUniformLocation(CowProg, "Matr");
  if (id != -1)
    glUniformMatrix4fv(id, 1, FALSE, m.A[0]);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLE);
  glBegin(GL_TRIANGLES);
  for (i = 0; i < NumOfFacets; i++)
  {
    for (j = 0; j < 3; j++)
    {
      glVertex3d(Vertexes[Facets[i][j]].X + Unit->X, Vertexes[Facets[i][j]].Y + Unit->Y, Vertexes[Facets[i][j]].Z + Unit->Z);
    }
  } 
  glEnd();

  glUseProgram(0); 
}/*End of "CowRender" function*/

IB1UNIT *CowCreate( FLT X, FLT Y, FLT Z )
{
  COW *Unit;

  if ((Unit = (COW *)IB1_UnitCreate(sizeof(COW))) == NULL)
    return NULL;

  Unit->Init = (IB1UNIT_INIT)CowInit;
  Unit->Render = (IB1UNIT_RENDER)CowRender;
  Unit->X = X;
  Unit->Y = Y;
  Unit->Z = Z;
  Unit->Who = X;
  Unit->RandShift = rand() % 1000;
  Unit->RandScale = 0.75 + 5.5 * rand() / (DBL)RAND_MAX;
  return (IB1UNIT *)Unit;
} /* End of 'CowCreate' function */

/* END OF 'MEGACOW.C' FILE */