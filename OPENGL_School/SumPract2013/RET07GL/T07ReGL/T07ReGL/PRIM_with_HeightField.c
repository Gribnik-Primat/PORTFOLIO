/* FILE NAME: PRIM.C
 * PROGRAMMER: VG4
 * DATE: 14.06.2013
 * PURPOSE: Geometry object handle.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "anim.h"

/* ���������� ���������� - ���� ������� �� ��������� */
VEC VG4_PrimDefaultColor = {1, 1, 1};

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ����������� ��������:
 *       vg4PRIM *Prim;
 *   - ��� ���������:
 *       vg4PRIM_TYPE Type;
 *   - ���������� ������ � ��������:
 *       INT NumOfV, NumOfI;
 *   - ��������� �� ������� (����� ���� NULL):
 *       VERTEX *Vert;
 *   - ��������� �� ������� (����� ���� NULL):
 *       INT *Ind;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE ��� �������� ������.
 */
BOOL VG4_PrimCreate( vg4PRIM *Prim, vg4PRIM_TYPE Type,
                     INT NumOfV, INT NumOfI,
                     VERTEX *Vert, INT *Ind )
{
  INT nv, ni, size, i, j, k;

  memset(Prim, 0, sizeof(vg4PRIM));
  if (Type == VG4_PRIM_GRID)
    nv = NumOfV * NumOfI, ni = NumOfV * (NumOfI - 1) * 2;
  else
    nv = NumOfV, ni = NumOfI;

  /* ���������� ������ ���������� ������ */
  size = sizeof(VERTEX) * nv + sizeof(INT) * ni;
  if ((Prim->V = malloc(size)) == NULL)
    return FALSE;
  memset(Prim->V, 0, size);
  Prim->Type = Type;
  Prim->I = (INT *)(Prim->V + nv);
  Prim->NumOfV = nv;
  Prim->NumOfI = ni;
  Prim->Size = size;
  Prim->Mat = -1;
  if (Vert == NULL)
    for (i = 0; i < nv; i++)
      Prim->V[i].C = VG4_PrimDefaultColor;
  else
    /* �������� ������� */
    memcpy(Prim->V, Vert, sizeof(VERTEX) * nv);
  if (Ind != NULL)
    /* �������� ������� */
    memcpy(Prim->I, Ind, sizeof(INT) * ni);
  if (Type == VG4_PRIM_GRID)
  {
    /* ������ ������� ��� ����� */
    Prim->GW = NumOfV;
    Prim->GH = NumOfI;

    k = 0;
    for (i = 0; i < Prim->GH - 1; i++)
      for (j = 0; j < Prim->GW; j++)
      {
        Prim->I[k++] = (i + 1) * Prim->GW + j;
        Prim->I[k++] = i * Prim->GW + j;
      }
  }
  return TRUE;
} /* End of 'VG4_PrimCreate' function */

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������� ��������:
 *       vg4PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID VG4_PrimClose( vg4PRIM *Prim )
{
  if (Prim->V != NULL)
    free(Prim->V);
  glDeleteBuffers(2, Prim->Buffs);
  glDeleteVertexArrays(1, &Prim->VertBuf);
  memset(Prim, 0, sizeof(vg4PRIM));
} /* End of 'VG4_PrimClose' function */

/* ������� ����������� ���������.
 * ���������:
 *   - ��������� �� ����������� ��������:
 *       vg4PRIM *Prim;
 *   - ��������� �� ���������� ��������:
 *       vg4PRIM *PrimSrc;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE ��� �������� ������.
 */
BOOL VG4_PrimDup( vg4PRIM *Prim, vg4PRIM *PrimSrc )
{
  VERTEX *v;

  memset(Prim, 0, sizeof(vg4PRIM));
  if ((v = malloc(PrimSrc->Size)) == NULL)
    return FALSE;

  /* �������� ������ */
  *Prim = *PrimSrc;
  Prim->V = v;
  Prim->I = (INT *)(Prim->V + PrimSrc->NumOfV);
  Prim->Buffs[0] = Prim->Buffs[1] = Prim->VertBuf = 0;
  Prim->Mat = -1;
  return TRUE;
} /* End of 'VG4_PrimDup' function */

/* ������� ��������� ���������.
 *   - ��������� �� ����������� ��������:
 *       vg4PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID VG4_PrimDraw( vg4PRIM *Prim )
{
  INT i;

  if (Prim->Buffs[0] == 0)
  {
    /* �������� ������ � ������ ��� */
    glGenBuffers(2, Prim->Buffs);
    glGenVertexArrays(1, &Prim->VertBuf);

    /*** ���������� ������ � �������� ������ ***/

    /* ������ ������ */
    glBindBuffer(GL_ARRAY_BUFFER, Prim->Buffs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX) * Prim->NumOfV,
      Prim->V, GL_STATIC_DRAW);
    /* ������ � ��������� */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Prim->Buffs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * Prim->NumOfI,
      Prim->I, GL_STATIC_DRAW);
  }

  /* ������������ ������ */
  glBindVertexArray(Prim->VertBuf);
  glBindBuffer(GL_ARRAY_BUFFER, Prim->Buffs[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Prim->Buffs[1]);

  /* ���������� ������� ������ */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(VERTEX), (VOID *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(VERTEX), (VOID *)(sizeof(VEC)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(VERTEX), (VOID *)(sizeof(VEC) + sizeof(UV)));
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(3, 3, GL_FLOAT, FALSE, sizeof(VERTEX), (VOID *)(sizeof(VEC) * 2 + sizeof(UV)));
  glEnableVertexAttribArray(3);

  if (Prim->Type == VG4_PRIM_GRID)
  {
    for (i = 0; i < Prim->GH - 1; i++)
    {
      glBindVertexArray(Prim->VertBuf);
      glBindBuffer(GL_ARRAY_BUFFER, Prim->Buffs[0]);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Prim->Buffs[1]);
      glDrawElements(GL_TRIANGLE_STRIP, Prim->GW * 2, GL_UNSIGNED_INT, (VOID *)(i * Prim->GW * 2 * sizeof(UINT)));
    }
  }
  else
  {
    glBindVertexArray(Prim->VertBuf);
    glBindBuffer(GL_ARRAY_BUFFER, Prim->Buffs[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Prim->Buffs[1]);
    glDrawElements(GL_TRIANGLES, Prim->NumOfI, GL_UNSIGNED_INT, (VOID *)0);
  }
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  /* ������ �������: * /
  glBegin(GL_LINES);
  for (i = 0; i < Prim->NumOfV; i++)
  {
    VEC p;

    glColor3d(1, 1, 1);
    glNormal3d(1, 1, 1);
    glVertex3fv(&Prim->V[i].P.X);
    p = VecAddVec(Prim->V[i].P, VecMulNum(Prim->V[i].N, 1.30));
    glVertex3fv(&p.X);
  }
  glEnd();
  /**/
} /* End of 'VG4_PrimDraw' function */

/* ������� �������� ��������� ���������.
 * ���������:
 *   - ��������� �� ����������� ��������:
 *       vg4PRIM *Prim;
 *   - ������ �����:
 *       INT W, H;
 *   - ������ ������ � ����������� �������:
 *       VEC Loc, Du, Dv;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE ��� �������� ������.
 */
BOOL VG4_PrimCreatePlane( vg4PRIM *Prim, INT W, INT H,
                          VEC Loc, VEC Du, VEC Dv )
{
  INT i, j, k = 0;
  VEC norm = VecCrossVec(Du, Dv);

  if (!VG4_PrimCreate(Prim, VG4_PRIM_GRID, W, H, NULL, NULL))
    return FALSE;

  /* ������ ������� ��������� ����� */
  for (i = 0; i < H; i++)
  {
    FLT v = (FLT)i / (H - 1);
    for (j = 0; j < W; j++)
    {
      FLT u = (FLT)j / (W - 1);

      Prim->V[k].P = VecAddVec(Loc,
        VecAddVec(VecMulNum(Du, u), VecMulNum(Dv, v)));
      Prim->V[k].T = UVSet(u, v);
      Prim->V[k].N = norm;
      k++;
    }
  }

  return TRUE;
} /* End of 'VG4_PrimCreatePlane' function */

/* ������� �������� ��������� ���������.
 * ���������:
 *   - ��������� �� ����������� ��������:
 *       vg4PRIM *Prim;
 *   - ������ �����:
 *       INT W, H;
 *   - ������ ������:
 *       VEC Loc;
 *   - ������:
 *       FLT R;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE ��� �������� ������.
 */
BOOL VG4_PrimCreateSphere( vg4PRIM *Prim, INT W, INT H,
                           VEC Loc, FLT Radius )
{
  INT i, j, k = 0;

  if (!VG4_PrimCreate(Prim, VG4_PRIM_GRID, W, H, NULL, NULL))
    return FALSE;

  /* ������ ������� ��������� ����� */
  for (i = 0; i < H; i++)
  {
    FLT
      v = (FLT)i / (H - 1),
      theta = v * VG4_PI;
    for (j = 0; j < W; j++)
    {
      FLT
        u = (FLT)j / (W - 1),
        phi = u * 2 * VG4_PI,
        x = sin(theta) * sin(phi),
        y = cos(theta),
        z = sin(theta) * cos(phi);

      Prim->V[k].P = VecAddVec(Loc, VecMulNum(VecSet(x, y, z), Radius));
      Prim->V[k].T = UVSet(u, v);
      Prim->V[k].N = VecSet(x, y, z);
      k++;
    }
  }
  return TRUE;
} /* End of 'VG4_PrimCreateSphere' function */

/* ������� �������� ��������� ������.
 * ���������:
 *   - ��������� �� ����������� ��������:
 *       vg4PRIM *Prim;
 *   - ��� ����� � ������:
 *       CHAR *FileName;
 *   - ������ ������ � ����������� �������:
 *       VEC Loc, Du, Dv;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE ��� �������� ������.
 */
BOOL VG4_PrimCreateHeightField( vg4PRIM *Prim, CHAR *FileName,
                                VEC Loc, VEC Du, VEC Dv )
{
  HBITMAP hBm, hBmDib;
  BITMAP Bm;
  HDC hDC, hMemDC, hMemDCDib;
  BITMAPINFOHEADER bih;
  DWORD *Bits;

  memset(Prim, 0, sizeof(vg4PRIM));

  if ((hBm = LoadImage(NULL, FileName,
         IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)) != NULL)
  {
    GetObject(hBm, sizeof(BITMAP), &Bm);

    /* ������� ����������� �������� ��������� */
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = Bm.bmWidth;
    bih.biHeight = Bm.bmHeight;
    bih.biPlanes = 1;    /* ������ */
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

      /* �������� �������� */
      SelectObject(hMemDC, hBm);
      SelectObject(hMemDCDib, hBmDib);
      BitBlt(hMemDCDib, 0, 0, Bm.bmWidth, Bm.bmHeight,
        hMemDC, 0, 0, SRCCOPY);

      /* ������� ��������� */
      if (VG4_PrimCreatePlane(Prim, Bm.bmWidth, Bm.bmHeight, Loc, Du, Dv))
      {
        INT i, j;

        for (i = 0; i < Prim->GH; i++)
          for (j = 0; j < Prim->GW; j++)
          {
            INT n = i * Prim->GW + j;
            Prim->V[n].P.Y = ((Bits[n] >> 8) & 0xFF) / 10.0;
          }

        for (i = 0; i < Prim->GH - 1; i++)
          for (j = 0; j < Prim->GW - 1; j++)
          {
            INT n = i * Prim->GW + j;

            Prim->V[n].N =
              VecCrossVec(VecSubVec(Prim->V[n + 1].P, Prim->V[n].P),
                          VecSubVec(Prim->V[n + Prim->GW].P, Prim->V[n].P));
          }
      }
      DeleteDC(hMemDC);
      DeleteDC(hMemDCDib);
      DeleteObject(hBmDib);
    }
    DeleteObject(hBm);
  }

  if (Prim->V == NULL)
    return FALSE;
  return TRUE;
} /* End of 'VG4_PrimCreateHeightField' function */

/* END OF 'PRIM.C' FILE */

