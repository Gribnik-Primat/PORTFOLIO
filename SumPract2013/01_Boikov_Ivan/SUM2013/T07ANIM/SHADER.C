/* FILE NAME   : SHADER.C
 * PURPOSE     : Shaders handle functions module.
 * PROGRAMMER  : IB1.
 * LAST UPDATE : 08.06.2013
 */

#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

/* Load text from file function.
 * ARGUMENTS:                               
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) loaded text or NULL if error is occured.
 */
static CHAR * ShadLoadText( CHAR *FileName )
{
  FILE *F;
  LONG N;
  CHAR *Res;

  if ((F = fopen(FileName, "rt")) == NULL)
    return NULL;

  fseek(F, 0, SEEK_END);
  N = ftell(F);

  if ((Res = malloc(N + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }
  memset(Res, 0, N + 1);
  rewind(F);
  fread(Res, 1, N, F);
  fclose(F);
  return Res;
} /* End of 'ShadLoadText' function */

/* Shader program initialization function.
 * ARGUMENTS:                               
 *   - vertex shader file name:
 *       CHAR *VertFileName;
 *   - fragment shader file name:
 *       CHAR *FragFileName;
 * RETURNS:
 *   (UINT) program Id or 0 if error is occured.
 */
UINT ShadProgInit( CHAR *VertFileName, CHAR *FragFileName )
{
  CHAR *text;
  INT res, len;
  UINT shv, shf, prg;
  static char Buf[10000];

  /* Vertex shader initialization */
  if ((text = ShadLoadText(VertFileName)) == NULL)
    return 0;
  if ((shv = glCreateShader(GL_VERTEX_SHADER)) == 0)
  {
    free(text);
    return 0;
  }
  glShaderSource(shv, 1, &text, NULL);
  free(text);
  glCompileShader(shv);
  glGetShaderiv(shv, GL_COMPILE_STATUS, &res);
  if (res != 1)
  {
    glGetShaderInfoLog(shv, sizeof(Buf), &len, Buf);
    /* printf("%s", Buf); */
    glDeleteShader(shv);
    return 0;
  }

  /* Fragment shader initialization */
  if ((text = ShadLoadText(FragFileName)) == NULL)
  {
    glDeleteShader(shv);
    return 0;
  }
  if ((shf = glCreateShader(GL_FRAGMENT_SHADER)) == 0)
  {
    glDeleteShader(shv);
    free(text);
    return 0;
  }
  glShaderSource(shf, 1, &text, NULL);
  free(text);
  glCompileShader(shf);
  glGetShaderiv(shf, GL_COMPILE_STATUS, &res);
  if (res != 1)
  {
    glGetShaderInfoLog(shf, sizeof(Buf), &len, Buf);
    /* printf("%s", Buf); */
    glDeleteShader(shv);
    glDeleteShader(shf);
    return 0;
  }

  /* Shader program initialization */
  if ((prg = glCreateProgram()) == 0)
  {
    glDeleteShader(shv);
    glDeleteShader(shf);
    return 0;
  }
  glAttachShader(prg, shv);
  glAttachShader(prg, shf);

  glLinkProgram(prg);
  glGetProgramiv(prg, GL_LINK_STATUS, &res);
  if (res != 1)
  {
    glDetachShader(prg, shv);
    glDetachShader(prg, shf);
    glGetProgramInfoLog(prg, sizeof(Buf), &len, Buf);
    /* printf("%s", Buf); */
    glDeleteShader(shv);
    glDeleteShader(shf);
    glDeleteProgram(prg);
    return 0;
  }
  return prg;
} /* End of 'ShadProgInit' function */

/* Shader program deinitialization function.
 * ARGUMENTS:                               
 *   - shader program Id:
 *       UINT Prg;
 * RETURNS: None.
 */
VOID ShadProgClose( UINT Prg )
{
  UINT i, n, shdrs[4];

  if (Prg == 0)
    return;
  glGetAttachedShaders(Prg, 4, &n, shdrs);

  for (i = 0; i < n; i++)
  {
    glDetachShader(Prg, shdrs[i]);
    glDeleteShader(shdrs[i]);
  }
  glDeleteProgram(Prg);
} /* End of 'ShadProgClose' function */

/* END OF 'SHADER.C' FILE */
