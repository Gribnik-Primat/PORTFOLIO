/* FILE NAME   : a.vert
 * PURPOSE     : Simple vertex shader.
 * PROGRAMMER  : VG4.
 * LAST UPDATE : 14.06.2013
 */

#version 400

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vTexCoord;
layout(location = 2) in vec4 vNormal;
layout(location = 3) in vec4 vColor;

uniform mat4 MatrWorldViewProj, MatrWorld;
uniform vec4 UseColor;

out vec4 DrawColor;
out vec4 DrawPos;
out vec4 DrawNormal;
out vec4 DrawTexCoord;

/* Main function */
void main( void )
{
  mat4 MIT = transpose(inverse(MatrWorld));
  vec4 N = MIT * vNormal;

  gl_Position = MatrWorldViewProj * vPosition;
  DrawPos = vPosition;
  DrawNormal = N;
  DrawColor = vColor;
  DrawTexCoord = vTexCoord;
} /* End of 'main' function */

/* End of 'a.vert' file */
