/* FILE NAME: IMAGE.C
 * PROGRAMMER: VG4
 * DATE: 15.06.2013
 * PURPOSE: Image loading handle handle.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "anim.h"

/* Функция загрузки изображения из файла.
 * АРГУМЕНТЫ:
 *   - указатель на создаваемый буфер точек (BGRA):
 *       DWORD **Bits;
 *   - указатели на ширину и высоту:
 *       INT *W, *H;
 *   - имя файла изображения:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE при нехватке памати или отсутствии файла.
 */
BOOL VG4_ImageLoad( DWORD **Bits, INT *W, INT *H, CHAR *FileName )
{
  HBITMAP hBm, hBmDib;
  BITMAP Bm;
  HDC hDC, hMemDC, hMemDCDib;
  BITMAPINFOHEADER bih;
  DWORD *BitsDib;

  *Bits = NULL;
  *W = *H = 0;

  if ((hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)) != NULL)
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
          (VOID *)&BitsDib, NULL, 0)) != NULL)
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

      if ((*Bits = malloc(4 * Bm.bmWidth * Bm.bmHeight)) != NULL)
      {
        memcpy(*Bits, BitsDib, 4 * Bm.bmWidth * Bm.bmHeight);
        *W = Bm.bmWidth;
        *H = Bm.bmHeight;
      }

      /* удаляем ресурсы */
      DeleteDC(hMemDC);
      DeleteDC(hMemDCDib);
      DeleteObject(hBmDib);
    }
    DeleteObject(hBm);
  }
  return *Bits != NULL;
} /* End of 'VG4_ImageLoad' function */

/* END OF 'IMAGE.C' FILE */

