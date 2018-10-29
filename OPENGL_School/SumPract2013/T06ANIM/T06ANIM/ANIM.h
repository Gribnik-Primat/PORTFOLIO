/* FILE NAME: ANIM.H
 * PROGRAMMER: VG4
 * DATE: 06.06.2013
 * PURPOSE: Main animation declaration module.
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"

/* Тип представления контектса анимации */

INT64
  TimeFreq,  /* единиц измерения времени в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время предыдущего кадра */
  TimePause, /* время простоя в паузу */
  TimeShift, /* время сдвига */
  TimeFPS;   /* время для замера FPS - frame-per-second */
LONG FrameCounter;




typedef struct tagvg4ANIM
{
  HWND hWnd;      /* Рабочее окно анимации */
  HDC hDC;        /* Контекст кадра в памяти */
  HGLRC hGLRC;
  HBITMAP hFrame; /* Битмап - буфер кадра */
  INT W, H,JPov,JCKID1;/* Размеры кадра анимации */
  DOUBLE Jx,Jz,Jy,Jr;
  BYTE JButOld[32],JBut[32];
  DWORD *Bits; 
  DBL
  Time,            /* время в сукундых со старта программы */
  DeltaTime,       /* время в секундах с прошлого кадра */
  GlobalTime,      /* время в сукундых со старта программы без паузы */
  GlobalDeltaTime, /* время в секундах с прошлого кадра без паузы */
  FPS;             /* количество кадров в секунду */
  BOOL
  IsPause;  /* Изображение кадра по точкам (DWORD на точку) */
} vg4ANIM;

/*** Типы внутренних функций-методов объектов-единиц анимации ***/
typedef struct tagvg4UNIT vg4UNIT;

/* Функция инициализации объекта */
typedef VOID (*vg4UNIT_INIT)( vg4UNIT *Unit, vg4ANIM *Ani );
/* Функция деинициализации объекта */
typedef VOID (*vg4UNIT_CLOSE)( vg4UNIT *Unit );
/* Функция реакции на межкадровое взаимодействие объекта */
typedef VOID (*vg4UNIT_RESPONSE)( vg4UNIT *Unit, vg4ANIM *Ani );
/* Функция отрисовки объекта */
typedef VOID (*vg4UNIT_RENDER)( vg4UNIT *Unit, vg4ANIM *Ani );


/* Функции объектов */
#define VG4_UNIT_BASE_FUNCS \
  vg4UNIT_INIT Init;         \
  vg4UNIT_CLOSE Close;       \
  vg4UNIT_RESPONSE Response; \
  vg4UNIT_RENDER Render


/* Тип представления актоивной единицы анимации */
struct tagvg4UNIT
{
  VG4_UNIT_BASE_FUNCS;
};

/* Функция инициализации анимации */
BOOL AnimInit( HWND hWnd );

/* Функция деинициализации анимации */
VOID AnimClose( VOID );

/* Функция обработки изменение размера кадра */
VOID AnimResize( INT W, INT H );

/* Функция построения кадра */
VOID AnimRender( VOID );

/* Функция копирования рабочего кадра в окно */
VOID AnimCopyFrame( VOID );

/* Функция сроздания нового объекта-единицы анимации */
vg4UNIT * VG4_UnitCreate( INT Size );

/* Функция добавления объекта анимации к системе */
VOID AnimAdd( vg4UNIT *Unit );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
