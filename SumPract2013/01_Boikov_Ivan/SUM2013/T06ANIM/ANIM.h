/* FILE NAME: ANIM.H
 * PROGRAMMER: IB1
 * DATE: 06.06.2013
 * PURPOSE: Main animation declaration module.
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"

/* ��� ������������� ��������� �������� */
typedef struct tagIB1ANIM
{                  
  HWND 
    hWnd;         /* ������� ���� �������� */
  HDC 
    hDC;           /* �������� ����� � ������ */
  HBITMAP 
    hFrame;    /* ������ - ����� ����� */
  INT 
    W, H;          /* ������� ����� �������� */
  DWORD 
    *Bits;       /* ����������� ����� �� ������ (DWORD �� �����) */
  DBL 
    Time,            /* ����� � �������� �� ������ ��������� */
    DeltaTime,       /* ����� � �������� � �������� ����� */
    GlobalTime,      /* ����� � �������� �� ������ ��������� ��� ����� */
    GlobalDeltaTime, /* ����� � �������� � �������� ����� ��� ����� */
    FPS,             /* ���������� ������ � ������� */
    Jx,              /*Joystick coordinates X*/
    Jy,              /*Joystick coordinates Y*/
    Jz,              /*Joystick coordinates Z*/
    Jr;              /*Joystick rotation*/
  BOOL
    IsPause;         /* ���� ����� */
  BYTE 
    JBut[32], JButOld[32];
  INT 
    JPov;            /*Point - of - View*/
} IB1ANIM;

/*** ���� ���������� �������-������� ��������-������ �������� ***/
typedef struct tagIB1UNIT IB1UNIT;

/* ������� ������������� ������� */
typedef VOID (*IB1UNIT_INIT)( IB1UNIT *Unit, IB1ANIM *Ani );
/* ������� ��������������� ������� */
typedef VOID (*IB1UNIT_CLOSE)( IB1UNIT *Unit );
/* ������� ������� �� ����������� �������������� ������� */
typedef VOID (*IB1UNIT_RESPONSE)( IB1UNIT *Unit, IB1ANIM *Ani );
/* ������� ��������� ������� */
typedef VOID (*IB1UNIT_RENDER)( IB1UNIT *Unit, IB1ANIM *Ani );


/* ������� �������� */
#define IB1_UNIT_BASE_FUNCS \
  IB1UNIT_INIT Init;         \
  IB1UNIT_CLOSE Close;       \
  IB1UNIT_RESPONSE Response; \
  IB1UNIT_RENDER Render


/* ��� ������������� ��������� ������� �������� */
struct tagIB1UNIT
{
  IB1_UNIT_BASE_FUNCS;
};

/* ������� ������������� �������� */
BOOL IB1_AnimInit( HWND hWnd );

/* ������� ��������������� �������� */
VOID IB1_AnimClose( VOID );

/* ������� ��������� ��������� ������� ����� */
VOID IB1_AnimResize( INT W, INT H );

/* ������� ���������� ����� */
VOID IB1_AnimRender( VOID );

/* ������� ����������� �������� ����� � ���� */
VOID IB1_AnimCopyFrame( VOID );

/* ������� ��������� ������ �������-������� �������� */
IB1UNIT * IB1_UnitCreate( INT Size );

/* ������� ���������� ������� �������� � ������� */
VOID IB1_AnimAdd( IB1UNIT *Unit );

VOID LoadCow (VOID);

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */