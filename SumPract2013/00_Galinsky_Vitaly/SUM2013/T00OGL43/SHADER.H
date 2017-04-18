/***************************************************************
 * Copyright (C) 2013
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : SHADER.H
 * PURPOSE     : Shaders handle functions.
 * PROGRAMMER  : CGSG'2012.
 * LAST UPDATE : 23.04.2013.
 * NOTE        : None.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __SHADER_H_
#define __SHADER_H_

#include <windows.h>

#include <glew.h>
#include <glut.h>

#include <commondf.h>

#pragma comment(lib, "glew32")

/* Initialization shader support function */
BOOL ShadInit( VOID );

/* Shader program initialization function */
UINT ShadProgInit( CHAR *VertFileName, CHAR *FragFileName );

/* Shader program deinitialization function */
VOID ShadProgClose( UINT Prg );

#endif /* __SHADER_H_ */

/* END OF 'SHADER.H' FILE */
