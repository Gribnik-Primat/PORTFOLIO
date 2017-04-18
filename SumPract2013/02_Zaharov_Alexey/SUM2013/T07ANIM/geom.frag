/* FILE NAME   : a.frag
 * PURPOSE     : Simple fragment shader.
 * PROGRAMMER  : VG4.
 * LAST UPDATE : 10.06.2013
 */

#version 400

out vec4 OutColor;

uniform float Time;

uniform mat4 Matr, MatrWorld;

uniform vec4 UseColor;
in vec4 DrawColor;
in vec4 DrawPos;
in vec4 DrawNormal;
in vec4 DrawTexCoord;

uniform sampler2D UseTexture;

vec4 GetColor( vec3 N )
{
  vec3 L = vec3(1, 1, 1);
  vec3 LightColor = vec3(0.8, 0.8, 0.8);
  L = normalize(L);
  float nl = dot(L, N);

  if (nl < 0.2)
    nl = 0.2;
  return vec4(DrawColor.rgb * LightColor * nl, 1);
}

/* Main function */
void main( void )
{
  /*
  if (DrawPos.x > 3.75 + 5.1 * sin(Time * 3.0))
    discard;
  */
  OutColor = texture(UseTexture, DrawTexCoord.st) * GetColor(normalize(vec3(DrawNormal)));// + vec4(1, 1, 1, 1) * sin(Time);
} /* End of 'main' function */

/* End of 'a.frag' file */
