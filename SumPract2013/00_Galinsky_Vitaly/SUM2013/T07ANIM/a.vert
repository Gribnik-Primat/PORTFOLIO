/* FILE NAME   : a.vert
 * PURPOSE     : Simple vertex shader.
 * PROGRAMMER  : VG4.
 * LAST UPDATE : 10.06.2013
 */

#version 430

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vTexCoord;
layout(location = 2) in vec4 vNormal;
layout(location = 3) in vec4 vColor;

uniform vec4 UseColor;
uniform mat4 Matr, MatrWorld;

out vec4 DrawColor;
out vec4 DrawPos;
out vec4 DrawNormal;
out vec4 DrawTexCoord;

vec4 mmm( vec4 V, mat4 A )
{
  float w = V.x * A[0][3] + V.y * A[1][3] + V.z * A[2][3] + A[3][3];

  return vec4((V.x * A[0][0] + V.y * A[1][0] + V.z * A[2][0] + A[3][0]),
              (V.x * A[0][1] + V.y * A[1][1] + V.z * A[2][1] + A[3][1]),
              (V.x * A[0][2] + V.y * A[1][2] + V.z * A[2][2] + A[3][2]),
              w);
}

vec4 GetColor( vec3 N )
{
  vec3 L = vec3(1, 1, 1);
  vec3 LightColor = vec3(0.8, 0.8, 0.8);
  L = normalize(L);
  float nl = dot(L, N);

  if (nl < 0.2)
    nl = 0.2;
  return vec4(vColor.rgb * LightColor * nl, 1);
}

/* Main function */
void main( void )
{
  mat4 MIT = transpose(inverse(MatrWorld));
  vec4 N = MIT * vNormal;
  //vec4 r = mmm(vPosition, Matr);
  gl_Position = Matr * vPosition;
  DrawPos = vPosition;
  DrawNormal = N;
  DrawColor = vColor;
  DrawTexCoord = vTexCoord;
} /* End of 'main' function */

/* End of 'a.vert' file */
