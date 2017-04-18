������:


LARGE_INTEGER li;

QueryPerfomanceCounter(&li);  - �������� ���. ����� � ������� ������ ���������

QueryPerfomanceFrequency (&li);  - �������� ��.��������� � �������.

� ���:
���������:

INT64
  TimeFreq,  /* ������ ��������� ������� � ������� */
  TimeStart, /* ����� ������ �������� */
  TimeOld,   /* ����� ����������� ����� */
  TimePause, /* ����� ������� � ����� */
  TimeShift, /* ����� ������ */
  TimeFPS;   /* ����� ��� ������ FPS - frame-per-second */
LONG FrameCounter;

� ANIM ���������:
DBL
  Time,            /* ����� � �������� �� ������ ��������� */
  DeltaTime,       /* ����� � �������� � �������� ����� */
  GlobalTime,      /* ����� � �������� �� ������ ��������� ��� ����� */
  GlobalDeltaTime, /* ����� � �������� � �������� ����� ��� ����� */
  FPS;             /* ���������� ������ � ������� */
BOOL
  IsPause;         /* ���� ����� */

 
AnimInit:

  /* ������������� ������� */
  LARGE_INTEGER li;
  
  QueryPerfomanceFrequency (&li); 
  TimeFreq  = li.QuadPart;
  QueryPerfomanceCounter(&li); 
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = TimeShift = 0;
  FrameCount = 0;

Render:
  /* ��������� ����������: */
  LARGE_INTEGER li;
  
  QueryPerfomanceCounter(&li);

  /* ���������� ����� */
  Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* ��������� ����� */
  if (Anim.IsPause)
    Anim.DeltaTime = 0, TimePause += li.QuadPart - TimeOld;
  else
    Anim.DeltaTime = Anim.GlobalDeltaTime;

  Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause - TimeShift) /
    TimeFreq;

  /* ��������� ������������������ */
  if (TimeFPS > TimeFreq)
  {
    Anim.FPS = FrameCount / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
  }
  /* ��������� ����� "�������" ����� */
  TimeOld = li.QuadPart;

