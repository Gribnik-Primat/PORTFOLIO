#include <math.h>

#include "anim.h"
#include "units.h"
#include "def.h"
#include "keycodes.h"

typedef struct tagbg3UNITCONTROLLED
{
  /* Extend basic unit */
  BG3_UNIT_BASE_FUNCS;
  /* Pos updated to base in Response method */
  BG3_UNIT_POS_FIELDS;
  /* All methods but Response are proxied to base unit */
  bg3UNITPOS * baseUnit;
  /* Control type */
  INT controlType;

  /* Used if controlType = 5 */
  INT destX, destY;
} UNITCONTROLLED;


/* Forward functions defs */
VOID ControlledUnitInit( bg3UNIT *Unit, bg3ANIM *Ani );
VOID ControlledUnitClose( bg3UNIT *Unit );
VOID ControlledUnitRender( bg3UNIT *Unit, bg3ANIM *Ani );
VOID ControlledUnitResponse( bg3UNIT *Unit, bg3ANIM *Ani );

/* C-tor */
bg3UNIT * CreateControlledUnit( bg3UNITPOS * bunit, INT controlType )
{
  UNITCONTROLLED * Unit;
  if ((Unit = (UNITCONTROLLED *)BG3_UnitCreate(sizeof(UNITCONTROLLED))) == NULL)
    return NULL;

  Unit->controlType = controlType;
  Unit->baseUnit = bunit;
  Unit->Init = (bg3UNIT_INIT)ControlledUnitInit;
  Unit->Close = (bg3UNIT_CLOSE)ControlledUnitClose;
  Unit->Response = (bg3UNIT_RESPONSE)ControlledUnitResponse;
  Unit->Render = (bg3UNIT_RENDER)ControlledUnitRender;
  Unit->X = bunit->X;
  Unit->Y = bunit->Y;
  Unit->Z = bunit->Z;

  Unit->destX = -1;
  Unit->destY = -1;
  return (bg3UNIT *)Unit;
}


VOID ControlledUnitInit( bg3UNIT * u, bg3ANIM *Ani )
{
  UNITCONTROLLED * Unit = (UNITCONTROLLED *)u;
  Unit->baseUnit->Init((bg3UNIT*)Unit->baseUnit, Ani);
}
VOID ControlledUnitClose( bg3UNIT * u )
{
  UNITCONTROLLED * Unit = (UNITCONTROLLED *)u;
  Unit->baseUnit->Close((bg3UNIT*)Unit->baseUnit);
}
VOID ControlledUnitRender( bg3UNIT * u, bg3ANIM *Ani )
{
  UNITCONTROLLED * Unit = (UNITCONTROLLED *)u;
  Unit->baseUnit->Render((bg3UNIT*)Unit->baseUnit, Ani);
}
                                                                                                                
VOID ControlledUnitResponse( bg3UNIT * u, bg3ANIM *Ani )
{
  UNITCONTROLLED * Unit = (UNITCONTROLLED *)u;
  VEC dvec = VecSet(0, 0, 0);
  DBL cosine, angle, dz = 0;
  INT updateRot = 1;
  if (!Ani->isPaused)
  {
    switch (Unit->controlType)
    {
    case 0:
      // WASD
    if (BG3_KeyPressed('W'))
        dvec.Y = -Ani->timer.deltaActiveTime * 300, Unit->RotZ = -90;
      else if (BG3_KeyPressed('S'))
        dvec.Y = Ani->timer.deltaActiveTime * 300, Unit->RotZ = 90;
      else if (BG3_KeyPressed('A'))
        dvec.X = -Ani->timer.deltaActiveTime * 300, Unit->RotZ = 180;
      else if (BG3_KeyPressed('D'))
        dvec.X = Ani->timer.deltaActiveTime * 300, Unit->RotZ = 0;
      break;
    case 1:
      // PoV
      if (Ani->joy.pov > 0)
      {
        DBL angleDeg = (-Ani->joy.pov + 3) * 45;
        DBL angle = D2R(angleDeg);
        dvec.X = cos(angle) * 300 * Ani->timer.deltaActiveTime;
        dvec.Y = -sin(angle) * 300 * Ani->timer.deltaActiveTime;
        Unit->RotZ = -angleDeg;
      }
      break;
    case 2:
      // Joy X, Y
      dvec = VecMulNum(VecSet((INT)Ani->joy.x, (INT)Ani->joy.y, 0),
        300 * Ani->timer.deltaActiveTime);
      break;
    case 3:
      // Joy Z, R
      dvec = VecMulNum(VecSet(
        (INT)Ani->joy.r, (INT)Ani->joy.z, 0
        ), 300 * Ani->timer.deltaActiveTime);
      break;
    case 5:
      // Auto
      if (Unit->destX < 0 || VecLen2(VecSubVec(VecSet(Unit->X, Unit->Y, Unit->Z), VecSet(Unit->destX, Unit->destY, Unit->Z))) < 10000) {
        Unit->destX = (rand() % 5000);
        Unit->destY = (rand() % 5000);
      } else {
        VEC delta = VecSubVec(VecSet(Unit->destX, Unit->destY, Unit->Z), VecSet(Unit->X, Unit->Y, Unit->Z));
        delta = VecNormalize(delta);
        dvec = VecMulNum(delta, 500 * Ani->timer.deltaActiveTime);
      }
      break;
    case 6:
      // Joy XY Rotation
      updateRot = FALSE;
      if (BG3_KeyPressed('A'))
        Unit->RotZ += 2;
      else if (BG3_KeyPressed('D'))
        Unit->RotZ -= 2;
      if (BG3_KeyPressed('S'))
        dz = -1;
      if (BG3_KeyPressed('W'))
        dz = 1;
      dvec = VecMulNum(VecSet(cos(D2R(Unit->RotZ)), sin(D2R(Unit->RotZ)), dz), (
              BG3_KeyPressed(KEY_CTRL) ? 1000 : 500) * Ani->timer.deltaActiveTime);
      break;
    }
  }
  Unit->X += dvec.X;
  Unit->Y += dvec.Y;
  Unit->Z += dvec.Z;
  Unit->baseUnit->X = Unit->X;
  Unit->baseUnit->Y = Unit->Y;
  Unit->baseUnit->Z = Unit->Z;
  if (updateRot && VecLen2(dvec) > 1)
  {
    cosine = VecDotVec(dvec, VecSet(1, 0, 0)) / VecLen(dvec);
    angle = acos(cosine);
    if (dvec.Y < 0)
      angle = -angle;
    Unit->RotZ = angle * 180 / PI;
  }
  Unit->baseUnit->RotZ = Unit->RotZ;
  Unit->baseUnit->Response((bg3UNIT*)(Unit->baseUnit), Ani);
}