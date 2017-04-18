/***
 * Cow support
 ***/

typedef DOUBLE DBL;
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;

/* Vertex array */
VEC *Vertexes, *VertexesProj;
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
  VertexesProj = Vertexes + NumOfVertexes;

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


  INT i, j;
  DOUBLE scale = 1;


    tim = clock();
    SelectObject(hMemDCFrame, GetStockObject(NULL_BRUSH));
    SelectObject(hMemDCFrame, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDCFrame, RGB(255, 0, 155));
    SetDCBrushColor(hMemDCFrame, RGB(0, 111, 0));
    scale = 59;
    /* calculate projection */
    for (i = 0; i < NumOfVertexes; i++)
    {
      DOUBLE a = tim * 0.00030;
      VertexesProj[i].X = WinW / 2 + scale * (Vertexes[i].X * cos(a) - Vertexes[i].Z * sin(a));
      VertexesProj[i].Y = WinH / 2 - scale * (Vertexes[i].Y);
      VertexesProj[i].Z = scale * (Vertexes[i].X * sin(a) + Vertexes[i].Z * cos(a));
    }

    for (i = 0; i < NumOfFacets; i++)
    {
      POINT p[3];

      for (j = 0; j < 3; j++)
      {
        p[j].x = VertexesProj[Facets[i][j]].X;
        p[j].y = VertexesProj[Facets[i][j]].Y;
      }

      Polygon(hMemDCFrame, p, 3);
    }

    for (i = 0; i < NumOfVertexes; i++)
    {
      INT
        x = VertexesProj[i].X;
        y = VertexesProj[i].Y;
      SetPixelV(hMemDCFrame, x, y, RGB(255, 255, 255));
    }
	