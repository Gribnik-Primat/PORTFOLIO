/* FILE NAME: GOBJ.C
 * PROGRAMMER: VG4
 * DATE: 14.06.2013
 * PURPOSE: Geometry object handle.
 */

#include <stdio.h>
#include <stdlib.h>

#include "anim.h"

/* Глобальный идентификатор программы - шейдера для примитивов с материалами */
static UINT VG4_GeomShadProgId = 0;

/* Функция удаления геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на удаляемый объект:
 *       vg4GOBJ *Go;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_GeomClose( vg4GOBJ *Go )
{
  if (Go->Prims != NULL)
  {
    INT i;

    for (i = 0; i < Go->NumOfPrims; i++)
      VG4_PrimClose(&Go->Prims[i]);
    free(Go->Prims);
  }
  if (Go->Mats != NULL)
    free(Go->Mats);
  memset(Go, 0, sizeof(vg4GOBJ));
} /* End of 'VG4_GeomClose' function */

/* Функция добавления примитива к геометрическому объекту.
 * АРГУМЕНТЫ:
 *   - указатель на объект:
 *       vg4GOBJ *Go;
 *   - указатель на примитив:
 *       vg4PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE при нехватке памати.
 */
BOOL VG4_GeomAddPrim( vg4GOBJ *Go, vg4PRIM *Prim )
{
  vg4PRIM *newblock = malloc(sizeof(vg4PRIM) * (Go->NumOfPrims + 1));

  if (newblock == NULL)
    return FALSE;
  /* копируем старые примитивы */
  memcpy(newblock, Go->Prims, sizeof(vg4PRIM) * Go->NumOfPrims);

  /* освобождаем старую память */
  free(Go->Prims);
  Go->Prims = newblock;

  /* дописываем новый примитив */
  Go->Prims[Go->NumOfPrims++] = *Prim;
  return TRUE;
} /* End of 'VG4_GeomAddPrim' function */

/* Функция добавления материала к геометрическому объекту.
 * АРГУМЕНТЫ:
 *   - указатель на объект:
 *       vg4GOBJ *Go;
 *   - указатель на материал:
 *       vg4MATERIAL *Mat;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE при нехватке памати.
 */
BOOL VG4_GeomAddMat( vg4GOBJ *Go, vg4MATERIAL *Mat )
{
  vg4MATERIAL *newblock = malloc(sizeof(vg4MATERIAL) * (Go->NumOfMats + 1));

  if (newblock == NULL)
    return FALSE;
  /* копируем старые примитивы */
  memcpy(newblock, Go->Mats, sizeof(vg4MATERIAL) * Go->NumOfMats);

  /* освобождаем старую память */
  free(Go->Mats);
  Go->Mats = newblock;

  /* дописываем новый примитив */
  Mat->TexNo = -1;
  Mat->TexNo2 = -1;
  Go->Mats[Go->NumOfMats++] = *Mat;
  return TRUE;
} /* End of 'VG4_GeomAddPrim' function */

/* Функция рисования геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на объект:
 *       vg4GOBJ *Go;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_GeomDraw( vg4GOBJ *Go )
{
  INT i, loc;
  MATR WVP;
  static FLT delta_time = 0;

  delta_time += VG4_Anim.GlobalDeltaTime;
  if (VG4_GeomShadProgId == 0 || delta_time > 1)
  {
    delta_time = 0;
    ShadProgClose(VG4_GeomShadProgId);
    VG4_GeomShadProgId = ShadProgInit("geom.vert", "geom.frag");
  }
  /* выбор программы шейдеров вывода примитивов */
  glUseProgram(VG4_GeomShadProgId);

  /* установка данных для преобразования координат */
  WVP = MatrMulMatr(MatrMulMatr(VG4_Anim.MatrWorld, VG4_Anim.MatrView), VG4_Anim.MatrProj);
  loc = glGetUniformLocation(VG4_GeomShadProgId, "MatrWorldViewProj");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, WVP.A[0]);
  loc = glGetUniformLocation(VG4_GeomShadProgId, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, VG4_Anim.MatrWorld.A[0]);
  loc = glGetUniformLocation(VG4_GeomShadProgId, "CamDir");
  if (loc != -1)
    glUniform3f(loc, -VG4_Anim.MatrView.A[0][2], -VG4_Anim.MatrView.A[1][2], -VG4_Anim.MatrView.A[2][2]);
  loc = glGetUniformLocation(VG4_GeomShadProgId, "Time");
  if (loc != -1)
    glUniform1f(loc, VG4_Anim.Time);

  for (i = 0; i < Go->NumOfPrims; i++)
  {
    /* установка материала */
    if (Go->Prims[i].Mat != -1)
    {
      INT n = Go->Prims[i].Mat;

      if (Go->Mats[n].TexNo == -1)
      {
        /* текстуры нет */
        Go->Mats[n].TexNo = 0;
        if (Go->Mats[n].MapD[0] != 0)
        {
          DWORD *Bits;
          INT W, H;

          if (VG4_ImageLoad(&Bits, &W, &H, Go->Mats[n].MapD))
          {
            /* создаем свободную тектсуру */
            glGenTextures(1, &Go->Mats[n].TexNo);
            /* активируем ее */
            glBindTexture(GL_TEXTURE_2D, Go->Mats[n].TexNo);

            /* засылаем изображение в OpenGL */
            gluBuild2DMipmaps(GL_TEXTURE_2D, 4, W, H, GL_BGRA, GL_UNSIGNED_BYTE, Bits);

            /* деактивируем тектуру */
            glBindTexture(GL_TEXTURE_2D, 0);

            free(Bits);
          }
        }
      }
      if (Go->Mats[n].TexNo2 == -1)
      {
        /* текстуры нет */
        Go->Mats[n].TexNo2 = 0;
        if (Go->Mats[n].Tex2[0] != 0)
        {
          DWORD *Bits;
          INT W, H;

          if (VG4_ImageLoad(&Bits, &W, &H, Go->Mats[n].Tex2))
          {
            /* создаем свободную тектсуру */
            glGenTextures(1, &Go->Mats[n].TexNo2);
            /* активируем ее */
            glBindTexture(GL_TEXTURE_2D, Go->Mats[n].TexNo2);

            /* засылаем изображение в OpenGL */
            gluBuild2DMipmaps(GL_TEXTURE_2D, 4, W, H, GL_BGRA, GL_UNSIGNED_BYTE, Bits);

            /* деактивируем тектуру */
            glBindTexture(GL_TEXTURE_2D, 0);

            free(Bits);
          }
        }
      }
      /* устанавливаем текущую текстуру */
      loc = glGetUniformLocation(VG4_GeomShadProgId, "UseTexture");
      if (loc != -1)
        glUniform1i(loc, 0);
      loc = glGetUniformLocation(VG4_GeomShadProgId, "UseTexture2");
      if (loc != -1)
        glUniform1i(loc, 1);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, Go->Mats[n].TexNo);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, Go->Mats[n].TexNo2);

      /* передаем параметры материалов */
      loc = glGetUniformLocation(VG4_GeomShadProgId, "Ka");
      if (loc != -1)
        glUniform3fv(loc, 1, &Go->Mats[n].Ka.X);
      loc = glGetUniformLocation(VG4_GeomShadProgId, "Kd");
      if (loc != -1)
        glUniform3fv(loc, 1, &Go->Mats[n].Kd.X);
      loc = glGetUniformLocation(VG4_GeomShadProgId, "Ks");
      if (loc != -1)
        glUniform3fv(loc, 1, &Go->Mats[n].Ks.X);
      loc = glGetUniformLocation(VG4_GeomShadProgId, "Phong");
      if (loc != -1)
        glUniform1f(loc, Go->Mats[n].Phong);
      loc = glGetUniformLocation(VG4_GeomShadProgId, "Alpha");
      if (loc != -1)
        glUniform1f(loc, Go->Mats[n].Trans);

      loc = glGetUniformLocation(VG4_GeomShadProgId, "UseColor");
      if (loc != -1)
        glUniform4f(loc, 1.0f, 1.0f, 1.0f, 1.0f);
    }

    /* рисуем примитив */
    loc = glGetUniformLocation(VG4_GeomShadProgId, "UsePart");
    if (loc != -1)
      glUniform1f(loc, i);
    loc = glGetUniformLocation(VG4_GeomShadProgId, "AllParts");
    if (loc != -1)
      glUniform1f(loc, Go->NumOfPrims);
    loc = glGetUniformLocation(VG4_GeomShadProgId, "Joy");
    if (loc != -1)
      glUniform4f(loc, VG4_Anim.Jx, VG4_Anim.Jy, VG4_Anim.Jz, VG4_Anim.Jr);
    loc = glGetUniformLocation(VG4_GeomShadProgId, "JoyB");
    if (loc != -1)
      glUniform4f(loc, VG4_Anim.JBut[0], VG4_Anim.JBut[1], VG4_Anim.JBut[2], VG4_Anim.JBut[3]);

    VG4_PrimDraw(&Go->Prims[i]);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  glUseProgram(0);
} /* End of 'VG4_GeomDraw' function */

/* END OF 'GOBJ.C' FILE */
