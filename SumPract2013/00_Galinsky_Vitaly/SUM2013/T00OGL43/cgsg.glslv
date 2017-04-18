/* FILE NAME   : cgsg.glslv
 * PURPOSE     : Simple vertex shader.
 * PROGRAMMER  : VG4.
 * LAST UPDATE : 08.06.2013
 */

#version 430

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vPosition1;

/* Main function */
void main( void )
{
  //gl_Position = vec4(vPosition.x / 2, vPosition.y, vPosition.z, 1);
  vec4 a = vPosition;

  //a.xyz /= 5;
  //a.w = 0.5;
  gl_Position = a; //vec4(vec3() / 2, 1);
} /* End of 'main' function */

/* End of 'cgsg.glslv' file */
