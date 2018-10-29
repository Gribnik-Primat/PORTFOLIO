/*
* FILE NAME: objloader.c
* PROGRAMMER: bg3
* DATE: 06.06.2013
* PURPOSE: .obj 3D models loader
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "units.h"
#include "vec.h"
#include "def.h"

POLYGONAL_MODEL * loadObjModel(char * filename)
{
  POLYGONAL_MODEL * model;
  FILE *F;
  INT fn = 0, vn = 0, i;
  static CHAR Buf[1000];

  model = malloc(sizeof(POLYGONAL_MODEL));

  if ((F = fopen(filename, "rt")) == NULL)
    return NULL;

  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      vn++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      fn++;

  model->facetsNum = fn;
  model->vertexesNum = vn;
  if ((model->vertexes = malloc(sizeof(VEC) * vn)) == NULL)
  {
    fclose(F);
    return NULL;
  }
  if ((model->vertProj = malloc(sizeof(VEC) * vn)) == NULL)
  {
    fclose(F);
    free(model->vertexes);
    return NULL;
  }
  if ((model->facets = malloc(2 * sizeof(FACET) * fn)) == NULL)
  {
    free(model->vertProj);
    free(model->vertexes);
    fclose(F);
    return NULL;
  }
  model->vertexesNum = vn;
  model->facetsNum = fn;

  vn = 0;
  fn = 0;
  rewind(F);
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;
      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      model->vertexes[vn].X = x;
      model->vertexes[vn].Y = y;
      model->vertexes[vn].Z = z;
      vn++;
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n[10];
      int a[10], j = 0;
      int l = strlen(Buf);

      for (i = 1; i < l; i++)
      {
        if (Buf[i] >= '0' && Buf[i] <= '9' && Buf[i - 1] == ' ')
        {
          a[j++] = i;
        }
      }
      for (i = 0; i < j; i++)
      {
        sscanf(Buf + a[i], "%d", &n[i]);
      }
      for (i = 2; i < j; i++)
      {
        model->facets[fn].vertexes[0] = n[0] - 1;
        model->facets[fn].vertexes[1] = n[i - 1] - 1;
        model->facets[fn].vertexes[2] = n[i] - 1;
        fn++;
      }
      model->vertexesNum += j - 3;
    }

  fclose(F);
  return model;
}