/* FILE NAME: GOBJ.C
 * PROGRAMMER: bg3
 * DATE: 11.06.2013
 * PURPOSE: Geometry object handle.
 */

#include <stdio.h>
#include <stdlib.h>

#include "anim.h"

/* Функция удаления геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на удаляемый объект:
 *       bg3GOBJ *Go;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID BG3_GeomClose( bg3GOBJ *Go )
{
  if (Go->Prims != NULL)
  {
    INT i;

    for (i = 0; i < Go->NumOfPrims; i++)
      BG3_PrimClose(&Go->Prims[i]);
    free(Go->Prims);
  }
  if (Go->Mats != NULL)
    free(Go->Mats);
  memset(Go, 0, sizeof(bg3GOBJ));
} /* End of 'BG3_GeomClose' function */

/* Функция добавления примитива к геометрическому объекту.
 * АРГУМЕНТЫ:
 *   - указатель на объект:
 *       bg3GOBJ *Go;
 *   - указатель на примитив:
 *       bg3PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE при нехватке памати.
 */
BOOL BG3_GeomAddPrim( bg3GOBJ *Go, bg3PRIM *Prim )
{
  bg3PRIM *newblock = malloc(sizeof(bg3PRIM) * (Go->NumOfPrims + 1));

  if (newblock == NULL)
    return FALSE;
  /* копируем старые примитивы */
  memcpy(newblock, Go->Prims, sizeof(bg3PRIM) * Go->NumOfPrims);


  /* освобождаем старую память */
  free(Go->Prims);
  Go->Prims = newblock;

  /* дописываем новый примитив */
  Go->Prims[Go->NumOfPrims++] = *Prim;
  return TRUE;
} /* End of 'BG3_GeomAddPrim' function */

/* Функция добавления материала к геометрическому объекту.
 * АРГУМЕНТЫ:
 *   - указатель на объект:
 *       bg3GOBJ *Go;
 *   - указатель на материал:
 *       bg3MATERIAL *Mat;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE при нехватке памати.
 */
BOOL BG3_GeomAddMat( bg3GOBJ *Go, bg3MATERIAL *Mat )
{
  bg3MATERIAL *newblock = malloc(sizeof(bg3MATERIAL) * (Go->NumOfMats + 1));

  if (newblock == NULL)
    return FALSE;
  /* копируем старые примитивы */
  memcpy(newblock, Go->Mats, sizeof(bg3MATERIAL) * Go->NumOfMats);

  /* освобождаем старую память */
  free(Go->Mats);
  Go->Mats = newblock;

  /* дописываем новый примитив */
  Mat->TexNo = -1;
  Go->Mats[Go->NumOfMats++] = *Mat;
  return TRUE;
} /* End of 'BG3_GeomAddPrim' function */

/* Функция рисования геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на объект:
 *       bg3GOBJ *Go;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID BG3_GeomDraw( bg3GOBJ *Go )
{
  INT i;

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
          HBITMAP hBm, hBmDib;
          BITMAP Bm;
          HDC hDC, hMemDC, hMemDCDib;
          BITMAPINFOHEADER bih;
          DWORD *Bits;

          if ((hBm = LoadImage(NULL, Go->Mats[n].MapD,
                 IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)) != NULL)
          {
            GetObject(hBm, sizeof(BITMAP), &Bm);

            /* создаем изображение заданной структуры */
            bih.biSize = sizeof(BITMAPINFOHEADER);
            bih.biWidth = Bm.bmWidth;
            bih.biHeight = Bm.bmHeight;
            bih.biPlanes = 1;    /* всегда */
            bih.biBitCount = 32; /* xRGB */
            bih.biCompression = BI_RGB;
            bih.biSizeImage = bih.biWidth * bih.biHeight * 4;
            bih.biClrImportant = 0;
            bih.biClrUsed = 0;
            bih.biXPelsPerMeter = bih.biYPelsPerMeter = 0;

            if ((hBmDib = CreateDIBSection(NULL, (BITMAPINFO *)&bih, DIB_RGB_COLORS,
                  (VOID *)&Bits, NULL, 0)) != NULL)
            {
              hDC = GetDC(NULL);
              hMemDC = CreateCompatibleDC(hDC);
              hMemDCDib = CreateCompatibleDC(hDC);
              ReleaseDC(NULL, hDC);

              /* копируем картинку */
              SelectObject(hMemDC, hBm);
              SelectObject(hMemDCDib, hBmDib);
              BitBlt(hMemDCDib, 0, 0, Bm.bmWidth, Bm.bmHeight,
                hMemDC, 0, 0, SRCCOPY);

              /* создаем свободную тектсуру */
              glGenTextures(1, &Go->Mats[n].TexNo);
              /* активируем ее */
              glBindTexture(GL_TEXTURE_2D, Go->Mats[n].TexNo);

              /* засылаем изображение в OpenGL */
              gluBuild2DMipmaps(GL_TEXTURE_2D, 4, Bm.bmWidth, Bm.bmHeight,
                GL_BGRA, GL_UNSIGNED_BYTE, Bits);

              /* деактивируем тектуру */
              glBindTexture(GL_TEXTURE_2D, 0);

              DeleteDC(hMemDC);
              DeleteDC(hMemDCDib);
              DeleteObject(hBmDib);
            }
            DeleteObject(hBm);
          }
        }
      }
      glBindTexture(GL_TEXTURE_2D, Go->Mats[n].TexNo);
    }

    /* рисуем примитив */
    if (i != 14)
      BG3_PrimDraw(&Go->Prims[i]);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
} /* End of 'BG3_GeomDraw' function */

/* END OF 'GOBJ.C' FILE */
