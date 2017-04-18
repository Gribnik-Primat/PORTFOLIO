/* FILE NAME: ANIM.H
 * PROGRAMMER: IB1
 * DATE: 06.06.2013
 * PURPOSE: Main animation declaration module.
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"

/* Тип представления контектса анимации */
typedef struct tagIB1ANIM
{                  
  HWND 
    hWnd;         /* Рабочее окно анимации */
  HDC 
    hDC;           /* Контекст кадра в памяти */
  HBITMAP 
    hFrame;    /* Битмап - буфер кадра */
  INT 
    W, H;          /* Размеры кадра анимации */
  DWORD 
    *Bits;       /* Изображение кадра по точкам (DWORD на точку) */
  DBL 
    Time,            /* время в сукундых со старта программы */
    DeltaTime,       /* время в секундах с прошлого кадра */
    GlobalTime,      /* время в сукундых со старта программы без паузы */
    GlobalDeltaTime, /* время в секундах с прошлого кадра без паузы */
    FPS,             /* количество кадров в секунду */
    Jx,              /*Joystick coordinates X*/
    Jy,              /*Joystick coordinates Y*/
    Jz,              /*Joystick coordinates Z*/
    Jr;              /*Joystick rotation*/
  BOOL
    IsPause;         /* флаг паузы */
  BYTE 
    JBut[32], JButOld[32];
  INT 
    JPov;            /*Point - of - View*/
} IB1ANIM;

/*** Типы внутренних функций-методов объектов-единиц анимации ***/
typedef struct tagIB1UNIT IB1UNIT;

/* Функция инициализации объекта */
typedef VOID (*IB1UNIT_INIT)( IB1UNIT *Unit, IB1ANIM *Ani );
/* Функция деинициализации объекта */
typedef VOID (*IB1UNIT_CLOSE)( IB1UNIT *Unit );
/* Функция реакции на межкадровое взаимодействие объекта */
typedef VOID (*IB1UNIT_RESPONSE)( IB1UNIT *Unit, IB1ANIM *Ani );
/* Функция отрисовки объекта */
typedef VOID (*IB1UNIT_RENDER)( IB1UNIT *Unit, IB1ANIM *Ani );


/* Функции объектов */
#define IB1_UNIT_BASE_FUNCS \
  IB1UNIT_INIT Init;         \
  IB1UNIT_CLOSE Close;       \
  IB1UNIT_RESPONSE Response; \
  IB1UNIT_RENDER Render


/* Тип представления актоивной единицы анимации */
struct tagIB1UNIT
{
  IB1_UNIT_BASE_FUNCS;
};

/* Функция инициализации анимации */
BOOL IB1_AnimInit( HWND hWnd );

/* Функция деинициализации анимации */
VOID IB1_AnimClose( VOID );

/* Функция обработки изменение размера кадра */
VOID IB1_AnimResize( INT W, INT H );

/* Функция построения кадра */
VOID IB1_AnimRender( VOID );

/* Функция копирования рабочего кадра в окно */
VOID IB1_AnimCopyFrame( VOID );

/* Функция сроздания нового объекта-единицы анимации */
IB1UNIT * IB1_UnitCreate( INT Size );

/* Функция добавления объекта анимации к системе */
VOID IB1_AnimAdd( IB1UNIT *Unit );

VOID LoadCow (VOID);

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */