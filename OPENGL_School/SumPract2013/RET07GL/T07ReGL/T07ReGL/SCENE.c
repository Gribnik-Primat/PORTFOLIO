#include <math.h>
#include <time.h>

#include "def.h"
#include "units.h"
#include "keycodes.h"


typedef struct tagbg3POLYUNIT
{
  /* Extend basic unit */
  BG3_UNIT_BASE_FUNCS;
  /* Is posable */
  BG3_UNIT_POS_FIELDS;
  /* Other transformations */
  MATR transformMatrix;
  /* 3D model */
  POLYGONAL_MODEL * model;

  /* Destination point */
  int destX, destY;

  /* Color */
  float r, g, b;
} POLYUNIT;

static INT CowProg = -2;


/* Function definitions */
VOID SetCamera( bg3ANIM * Ani, VEC pos, VEC lookAt, VEC up );


VOID BG3_SceneCreate(bg3ANIM * Ani)
{
  POLYUNIT *Unit;
  POLYGONAL_MODEL * models;
  MATR modelMatr;
  INT X=0,Y=0,Z=0;

  bg3GOBJ *Go = (VOID *)malloc(sizeof(bg3GOBJ));
  
  Ani->camera.Wh = 2;
  Ani->camera.Hh = 2;
  Ani->camera.PD = 1;
  Ani->camera.projMatr = MatrFrustum(-Ani->camera.Wh/2, Ani->camera.Wh/2,
                                         -Ani->camera.Hh/2, Ani->camera.Hh/2, 1, 10000);
  
  

     
  BG3_AnimAdd(TestUnitCreate( X,Y,Z));
}

VOID BG3_SceneResponse(bg3ANIM * ani)
{
  bg3UNITPOS * unit = (bg3UNITPOS *)ani->units[0];
  DBL time = clock();
  if(ani->keyState.actual['P'])
      ani->isPaused = !ani->isPaused;
  
}

VOID SetCamera( bg3ANIM * Ani, VEC pos, VEC lookAt, VEC up )
{
  Ani->camera.Pos = pos;
  Ani->camera.At = lookAt;
  Ani->camera.Up = up;
  Ani->camera.viewMatr = MatrLookAt(pos, lookAt, up);
  Ani->camera.MVP = MatrMulMatr(Ani->camera.viewMatr, Ani->camera.projMatr);
}