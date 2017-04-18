/* FILE NAME   : cgsg.glslf
 * PURPOSE     : Simple fragment shader.
 * PROGRAMMER  : VG4.
 * LAST UPDATE : 08.06.2013
 */

#version 430

out vec4 fColor;

uniform vec4 color30;

/* Main function */
void main( void )
{
  fColor = vec4(0.0, 0.0, 1.0, 1.0);
  //fColor = color30;
} /* End of 'main' function */

/* End of 'cgsg.glslf' file */
