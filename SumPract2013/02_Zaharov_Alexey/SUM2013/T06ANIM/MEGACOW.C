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
  INT X, Y; 
  INT RandShift;
  DBL RandScale;
  INT Who;      
} COW;


/* Vertex array */
VEC *Vertexes;
static POINT *VertexesProj;
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

  if ((F = fopen("cow_new1.object", "rt")) == NULL)
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
  if ((Facets = malloc(sizeof(INT [3]) * fn)) == NULL)
  {
    free(Vertexes);
    fclose(F);
    return;
  }
  NumOfVertexes = vn;
  NumOfFacets = fn;
  VertexesProj = (POINT *)(Vertexes + NumOfVertexes);

  vn = 0;
  fn = 0;
  rewind(F);
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      Vertexes[vn].X = x;
      Vertexes[vn].Y = y;
      Vertexes[vn].Z = z;
      vn++;
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
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
  DBL t = (DBL)clock() / CLOCKS_PER_SEC;
  INT
    i, j, y;
  MATR m = 
  {
    {
      {1, 0, 0 ,0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  }, m1;

  SelectObject(Ani->hDC, GetStockObject(NULL_BRUSH));
  SelectObject(Ani->hDC, GetStockObject(DC_PEN));
  SetDCPenColor(Ani->hDC, RGB(255, 255, 255));
  SetDCBrushColor(Ani->hDC, RGB(0, 111, 0));

  m = MatrMulMatr(MatrScale(40, 40, 40), MatrTranslate(0, 0, -100)); 


 switch (Ani->Jpov)
  {
  case 1:
    Unit->Y -= t;
    break;
  case 2:
    Unit->X += sqrt(2)*t;
    Unit->Y -= sqrt(2)*t;
    break;
  case 3:
    Unit->X += t;  
    break;
  case 4:
    Unit->X += sqrt(2)*t;
    Unit->Y += sqrt(2)*t;
    break;
  case 5:
     Unit->Y += t;
    break;
  case 6:
    Unit->X -= sqrt(2)*t;
    Unit->Y += sqrt(2)*t;   
    break;
  case 7:
    Unit->X -= t;
    break;
  case 8:
    Unit->Y -= sqrt(2)*t;
    Unit->X -= sqrt(2)*t;  
    break;
  default:
    break;
  };
  
  for (i = 0; i < NumOfVertexes; i++)
  {
    VEC v;
    v = Vertexes[i];
    v.X += Unit->X;
    v.Y += Unit->Y;
    v.Z += 10;

    VertexesProj[i] = WorldToScreen(v); 
  }

  for (i = 0; i < NumOfFacets; i++)
  {
    POINT p[3];

    for (j = 0; j < 3; j++)
    {
      p[j].x = VertexesProj[Facets[i][j]].x;         
      p[j].y = VertexesProj[Facets[i][j]].y;
    }
    Polygon(Ani->hDC, p, 3);
  } 

  /*for (i = 0; i < NumOfVertexes; i++)
  {
    POINT p;
    INT
      x = VertexesProj[i].x;
      y = VertexesProj[i].y;
      //p = WorldToScreen(VertexesProj[i]);
    //SetPixelV(Ani->hDC, x, y, RGB(255, 255, 255));
  } */
}

az1UNIT * CowCreate( INT X, INT Y )
{
  COW *Unit;

  if ((Unit = (COW *)AZ1_UnitCreate(sizeof(COW))) == NULL)
    return NULL;

  Unit->Render = (az1UNIT_RENDER)CowRender;
  Unit->X = X;
  Unit->Y = Y;
  Unit->Who = 0;
  Unit->RandShift = rand() % 1000;
  Unit->RandScale = 0.75 + 5.5 * rand() / (DBL)RAND_MAX;
  return (az1UNIT *)Unit;
} /* End of 'CowCreate' function */

/* END OF 'MEGACOW.C' FILE */