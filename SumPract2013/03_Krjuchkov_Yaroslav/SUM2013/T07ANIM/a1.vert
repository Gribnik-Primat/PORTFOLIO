/* FILE NAME   : a.vert
 * PURPOSE     : Simple vertex shader.
 * PROGRAMMER  : VG4.
 * LAST UPDATE : 08.06.2013
 */

#version 400

//uniform vec4 DrawColor;

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;

out vec4 UseColor;

/* Main function */
void main( void )
{
  gl_Position = vPosition;
  UseColor = vColor;
} /* End of 'main' function */

/* End of 'a.vert' file */
