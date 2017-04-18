Таймер:


LARGE_INTEGER li;

QueryPerfomanceCounter(&li);  - получает тек. время с момента старта программы

QueryPerfomanceFrequency (&li);  - получает ед.измерения в секунду.

У нас:
глобально:

INT64
  TimeFreq,  /* единиц измерения времени в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время предыдущего кадра */
  TimePause, /* время простоя в паузу */
  TimeShift, /* время сдвига */
  TimeFPS;   /* время для замера FPS - frame-per-second */
LONG FrameCounter;

В ANIM добавляем:
DBL
  Time,            /* время в сукундых со старта программы */
  DeltaTime,       /* время в секундах с прошлого кадра */
  GlobalTime,      /* время в сукундых со старта программы без паузы */
  GlobalDeltaTime, /* время в секундах с прошлого кадра без паузы */
  FPS;             /* количество кадров в секунду */
BOOL
  IsPause;         /* флаг паузы */

 
AnimInit:

  /* инициализация таймера */
  LARGE_INTEGER li;
  
  QueryPerfomanceFrequency (&li); 
  TimeFreq  = li.QuadPart;
  QueryPerfomanceCounter(&li); 
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = TimeShift = 0;
  FrameCount = 0;

Render:
  /* обновляем информацию: */
  LARGE_INTEGER li;
  
  QueryPerfomanceCounter(&li);

  /* глобальное время */
  Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* локальное время */
  if (Anim.IsPause)
    Anim.DeltaTime = 0, TimePause += li.QuadPart - TimeOld;
  else
    Anim.DeltaTime = Anim.GlobalDeltaTime;

  Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause - TimeShift) /
    TimeFreq;

  /* вычисляем производительность */
  if (TimeFPS > TimeFreq)
  {
    Anim.FPS = FrameCount / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
  }
  /* обновляем время "старого" кадра */
  TimeOld = li.QuadPart;

