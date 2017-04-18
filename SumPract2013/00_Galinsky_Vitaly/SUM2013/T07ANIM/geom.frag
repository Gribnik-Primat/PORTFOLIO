/* FILE NAME   : a.frag
 * PURPOSE     : Simple fragment shader.
 * PROGRAMMER  : VG4.
 * LAST UPDATE : 14.06.2013
 */

#version 330

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
uniform float UsePart;
uniform float AllParts;
uniform vec4 Joy, JoyB;

in vec4 DrawColor;
in vec4 DrawPos;
in vec4 DrawNormal;
in vec4 DrawTexCoord;
in vec4 N1;

uniform sampler2D UseTexture;
uniform sampler2D UseTexture2;

vec4 GetColor( vec3 N )
{
  vec4 TC = texture(UseTexture, DrawTexCoord.st);
  ivec2 s = textureSize(UseTexture, 1);
  if (s.x == 0)
    TC = vec4(1, 1, 1, 1);

  vec4 TC2 = texture(UseTexture2, DrawTexCoord.st * 5);
  ivec2 s2 = textureSize(UseTexture2, 1);
  if (s2.x == 0)
    TC2 = vec4(0, 0, 0, 0);
  else
    TC.xyz = 1.2 * (TC2.xyz - vec3(0.5, 0.5, 0.5)) * 2;//TC = (TC * 4 + TC2 * 1) / 5;//N -= TC2.xyz/1;

  vec3 L = normalize(vec3(1, 1, 1));
  vec3 LightColor = vec3(1, 1, 1);

  N = faceforward(normalize(N), CamDir, N);

  /* Ambient */
  vec3 res_color = Ka * vec3(0.1, 0.1, 0.1);

  /* Diffuse */
  float nl = dot(L, vec3(N));
  if (nl > 0.00001)
    res_color.xyz += (Kd.xyz * TC.xyz) * nl;

  /* Specular */
  vec3 R = reflect(CamDir, N);
  float rn = dot(R, N);
  if (rn > 0.00001)
    res_color.xyz += Ks.xyz * pow(rn, Phong);

  return vec4(DrawColor.xyz * (N * JoyB.x + R * JoyB.y  + res_color) / 1, 1 - Alpha);
}

/* Main function */
void main( void )
{
  
  int a = int((1 - abs(sin(Time))) * AllParts), b = a - 59, c = a + 59;

  if (AllParts > 100 && (UsePart > c || UsePart < b))
    ;//discard;
  if (JoyB.z != 0 && DrawPos.z > 0.7 + 1.1 * sin(Time * 3.0))
    discard;
  if (int(Time) % 3 == 0)
    if ((int(DrawPos.x * 5) + int(DrawPos.y * 5) + int(DrawPos.z * 5)) % 2 == 0)
      discard;
  /*
  */
  /*
  if ((DrawPos.y + DrawPos.x) > 0.9 + 1.8 * sin(Time * 5))
    discard;
  if (DrawPos.x > 0.5 + 0.5 * sin(Time * 5.3))
    discard;
  */
  OutColor = GetColor(normalize(vec3(DrawNormal))); // + vec4(1, 1, 1, 1) * sin(Time);
} /* End of 'main' function */

/* End of 'a.frag' file */
