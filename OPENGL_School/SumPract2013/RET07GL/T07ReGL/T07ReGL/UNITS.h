/*
* FILE NAME: units.h
* PROGRAMMER: bg3
* DATE: 06.06.2013
* PURPOSE: Definitions of certain unit types constructors and methods
*/
#ifndef __UNITS_H_
#define __UNITS_H_

#include "anim.h"
#include "def.h"
#include "vec.h"

typedef struct tagFacet
{
  INT vertexes[3];
} FACET;


typedef struct tagPolygonalModel
{
  VEC * vertexes;
  INT vertexesNum;
  FACET * facets;
  INT facetsNum;

  VEC * vertProj;
} POLYGONAL_MODEL;

#define BG3_UNIT_POS_FIELDS DBL X, Y, Z, RotZ

typedef struct tagUnitPosable
{
  BG3_UNIT_BASE_FUNCS;
  BG3_UNIT_POS_FIELDS;
} bg3UNITPOS;



/* megacube.c */
bg3UNIT * CubeCreate( INT X, INT Y );

/* Polygonal unit methods. polyunit.c */
POLYGONAL_MODEL * loadObjModel( char * filename );
bg3UNIT * PolyUnitCreate( INT X, INT Y, INT Z, POLYGONAL_MODEL * model, MATR transformationMatrix );
void PolyUnitSetTransform( bg3UNIT * unit, MATR matr );

/* Controlled units unitcontrolled.c */
bg3UNIT * CreateControlledUnit( bg3UNITPOS * bunit, INT controlType );

#endif