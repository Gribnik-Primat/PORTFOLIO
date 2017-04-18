/* FILE NAME: MEGACOW.C
 * PROGRAMMER: IK1
 * DATE: 06.06.2013
 * PURPOSE: Cow animation module.
 */

#include <stdlib.h>
#include <stdio.h>

#include <math.h>
#include <time.h>

#include "anim.h"


/* Vertex array */
static VEC *Vertexes; 
static POINT *VertexesProj;
static INT NumOfVertexes;

/* Facet array */
static INT (*Facets)[3];
static INT NumOfFacets;

/* w, h of window */
static INT w, h;


typedef struct
{
  IK1_UNIT_BASE_FUNCS;
  INT X, Y; 
  INT Num;      
} COW;

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
}
static VOID CowRender( COW *Unit, ik1ANIM *Ani )
{ 
  DBL t = (DBL)clock() / CLOCKS_PER_SEC;
  INT
    i, j, y;
  MATR m, m1; 
  /*
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

  SelectObject(Ani->hDC, GetStockObject(NULL_BRUSH));
  SelectObject(Ani->hDC, GetStockObject(DC_PEN));
  SetDCPenColor(Ani->hDC, RGB(200, 150, 100));
  SetDCBrushColor(Ani->hDC, RGB(200, 150, 100));












  Ani->MatrView = MatrViewLookAt(VecSet(500 , 500 , 500), VecSet(0, 0, 0), VecSet(0,1,0)); 


  Ani->MatrWorld = MatrMulMatr(MatrMulMatr(MatrMulMatr(MatrScale(2, 2, 2), MatrTranslate(0,0,-200)), MatrRotateZ((t) * 0.10)), MatrRotateX(0)); 
  Ani->MatrProj = MatrProject(-100, +100, -100, +100, -100, +100);                                                                


  m = MatrMulMatr(MatrMulMatr(Ani->MatrView, Ani->MatrWorld), Ani->MatrProj);

                               /*sdfsdf*/
  for (i = 0; i < NumOfVertexes; i++)
  {
    VEC v, v1;
    v = VecSet(Vertexes[i].X, Vertexes[i].Y, Vertexes[i].Z);
    /*v = VecMulMatr(v, m1);   */    
    VertexesProj[i] = IK1_AnimWorldToScreen(Vertexes[i]);
  }

  for (i = 0; i < NumOfFacets; i++)
  {
    POINT p[3];
    for (j = 0; j < 3; j++)
    {
      
      p[j] = VertexesProj[Facets[i][j]];         
  
     
      //p[j] = IK1_AnimWorldToScreen(VertexesProj[Facets[i][j]]);
    }
    Polygon(Ani->hDC, p, 3);
  
  }  

  /*or (i = 0; i < NumOfVertexes; i++)
  {
    INT
      x = VertexesProj[i].X;
      y = VertexesProj[i].Y;
    SetPixelV(Ani->hDC, x, y, RGB(255, 255, 255));
  }  */
}

ik1UNIT * CowCreate( INT X, INT Y )
{
  COW *Unit;
  LoadCow();

  
  if ((Unit = (COW *)IK1_UnitCreate(sizeof(COW))) == NULL)
    return NULL;



  Unit->Render = (ik1UNIT_RENDER)CowRender;
  Unit->X = X;
  Unit->Y = Y;
  return (ik1UNIT *)Unit;   
} /* End of 'CowCreate' function */

/* END OF 'MEGACOW.C' FILE */