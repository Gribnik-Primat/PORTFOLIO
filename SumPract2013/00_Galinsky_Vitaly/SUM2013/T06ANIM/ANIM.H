/* FILE NAME: ANIM.H
 * PROGRAMMER: VG4
 * DATE: 07.06.2013
 * PURPOSE: Main animation declaration module.
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "vec.h"

/* Тип представления контектса анимации */
typedef struct tagvg4ANIM
{
  HWND hWnd;      /* Рабочее окно анимации */
  HDC hDC;        /* Контекст кадра в памяти */
  HBITMAP hFrame; /* Битмап - буфер кадра */
  INT W, H;       /* Размеры кадра анимации */
  DWORD *Bits;    /* Изображение кадра по точкам (DWORD на точку) */

  /* Данные устройств ввода */
  BYTE Keys[256], KeysOld[256]; /* Клавиши клавиатуры */

  INT
    MouseX, MouseY,                /* Позиция курсора мыши */
    MouseDeltaX, MouseDeltaY,      /* Относительный сдвиг курсора мыши */
    MouseWheel,                    /* Относительный сдвиг колеса мыши */
    MouseBut[10], MouseButOld[10]; /* Состояние кнопок мыши */

  BYTE JBut[32], JButOld[32]; /* Кнопки джойстика */
  DBL Jx, Jy, Jz, Jr;         /* Оси джойстика */
  INT JPov;                   /* Point-of-View 0-8 джойстика */

  /* Данные для времени и синхронизации */
  DBL
    Time,            /* время в сукундых со старта программы */
    DeltaTime,       /* время в секундах с прошлого кадра */
    GlobalTime,      /* время в сукундых со старта программы без паузы */
    GlobalDeltaTime, /* время в секундах с прошлого кадра без паузы */
    FPS;             /* количество кадров в секунду */
  BOOL
    IsPause;         /* флаг паузы */

  /* Параметры камеры и проецирования */
  MATR
    MatrWorld,    /* преобразования в мировой СК */
    MatrView,     /* видовые преобразования */
    MatrProj;     /* проекционные преобразования */
  DBL PD, Wp, Hp; /* параметры плоскости проекции */
  VEC
    CameraLoc,	  /* позиция камеры */
    CameraDir,    /* направление камеры вперед */
    CameraUp,     /* направление камеры вверх */
    CameraRight,  /* направление камеры вправо */
    CameraLookAt; /* "точка интереса" камеры */
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

/* Значение сдвига колеса мыши */
extern INT VG4_MouseWheel;

/* Функция инициализации анимации */
BOOL VG4_AnimInit( HWND hWnd );

/* Функция деинициализации анимации */
VOID VG4_AnimClose( VOID );

/* Функция обработки изменение размера кадра */
VOID VG4_AnimResize( INT W, INT H );

/* Функция построения кадра */
VOID VG4_AnimRender( VOID );

/* Функция копирования рабочего кадра в окно */
VOID VG4_AnimCopyFrame( VOID );

/* Функция сроздания нового объекта-единицы анимации */
vg4UNIT * VG4_UnitCreate( INT Size );

/* Функция добавления объекта анимации к системе */
VOID VG4_AnimAdd( vg4UNIT *Unit );

/* Функция преобразования из мировой системы коорлинат в кадр */
POINT VG4_AnimWorldToScreen( VEC P );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
