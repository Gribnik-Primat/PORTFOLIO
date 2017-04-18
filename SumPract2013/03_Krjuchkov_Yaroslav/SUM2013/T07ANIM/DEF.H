/* FILE NAME: DEF.H
 * PROGRAMMER: IK1
 * DATE: 06.06.2013
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

#pragma warning(disable: 4305 4244 4996)


/* Общие типы */
typedef __int64 INT64;
typedef unsigned __int64 UINT64;
typedef FLOAT FLT;

/* Так как координаты в окне OpenGL от -1 до 1, а в типе POINT два целочисленных поля */
typedef struct {
  FLT x;
  FLT y;
} IK1_POINT;


#endif /* __DEF_H_ */

/* END OF 'DEF.H' FILE */
