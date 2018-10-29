/* FILE NAME : SHADER.H
* PURPOSE : Shaders handle functions.
* PROGRAMMER : bg3.
* LAST UPDATE : 08.06.2013.
*/

#ifndef __SHADER_H_
#define __SHADER_H_

#include "def.h"

/* Shader program initialization function */
UINT ShadProgInit( CHAR *VertFileName, CHAR *FragFileName );

/* Shader program deinitialization function */
VOID ShadProgClose( UINT Prg );

#endif /* __SHADER_H_ */

/* END OF 'SHADER.H' FILE */