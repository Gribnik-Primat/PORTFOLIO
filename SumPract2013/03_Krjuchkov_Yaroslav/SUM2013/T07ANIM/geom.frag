/* FILE NAME   : a.frag
 * PURPOSE     : Simple fragment shader.
 * PROGRAMMER  : VG4.
 * LAST UPDATE : 14.06.2013
 */

#version 400

out vec4 OutColor;

uniform float Time;

uniform mat4 MatrWorldViewProj, MatrWorld;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Phong;
uniform float Alpha;

uniform vec3 CamDir;
uniform vec4 UseColor;

in vec4 DrawColor;
in vec4 DrawPos;
in vec4 DrawNormal;
in vec4 DrawTexCoord;
in vec4 N1;

uniform sampler2D UseTexture;

vec4 GetColor( vec3 N )
{
  vec4 TC = texture(UseTexture, DrawTexCoord.st);

  vec3 L = normalize(vec3(1, 1, 1));
  vec3 LightColor = vec3(1, 1, 1);

  vec3 dir = CamDir.xyz;
  if (dot(dir, N) > 0)
    N = -N;

  /* Ambient */
  vec3 res_color = Ka * vec3(0.1, 0.1, 0.1);

  /* Diffuse */
  float nl = dot(L, vec3(N));
  if (nl > 0.0001)
    res_color.xyz += (Kd.xyz * TC.xyz) * nl;

  /* Specular */
  vec3 R = reflect(dir.xyz, N);
  float rn = dot(R, N);
  if (rn > 0.0001)
    res_color.xyz += Ks.xyz * pow(rn, 15);

  return vec4(res_color, 1 - Alpha); /// vec4(DrawColor.rgb * LightColor * nl, 1);
}

/* Main function */
void main( void )
{
  /*
  if (DrawPos.x > 3.75 + 5.1 * sin(Time * 3.0))
    discard;
  */

  OutColor = GetColor(normalize(vec3(DrawNormal))); // + vec4(1, 1, 1, 1) * sin(Time);
} /* End of 'main' function */

/* End of 'a.frag' file */
