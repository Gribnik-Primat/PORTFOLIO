/*
* FILE NAME: DEF.H
* PROGRAMMER: bg3
* DATE: 06.06.2013
* PURPOSE: Common definition declaration module.
*/

#ifndef __DEF_H_
#define __DEF_H_

/* Win32 libs */
#ifdef WIN32
#pragma comment(lib, "glew32")
#include "glew.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma warning(disable: 4305 4244 4996)
#endif

typedef unsigned char BYTE;
typedef long long INT64;
typedef unsigned long long UINT64;
typedef double DBL;
typedef float FLT;


#endif

/* END OF 'DEF.H' FILE */