/* FILE NAME: DEF.H
 * PROGRAMMER: VG4
 * DATE: 14.06.2013
 * PURPOSE: Common definition declaration module.
 */

#ifndef __DEF_H_
#define __DEF_H_

#include <windows.h>
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32")

/* поддержка звука */
#include <al.h>
#include <alc.h>
#include <alut.h>

#pragma comment(lib, "openal32")
#pragma comment(lib, "alut")


#pragma warning(disable: 4305 4244 4996)

/* Debug memory allocation support */
#ifdef _DEBUG
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
     _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else /* _DEBUG */
#  define SetDbgMemHooks() ((VOID)0)
#endif /* _DEBUG */


/* Общие типы */
typedef __int64 INT64;
typedef unsigned __int64 UINT64;
typedef DOUBLE DBL;
typedef FLOAT FLT;

#endif /* __DEF_H_ */

/* END OF 'DEF.H' FILE */
