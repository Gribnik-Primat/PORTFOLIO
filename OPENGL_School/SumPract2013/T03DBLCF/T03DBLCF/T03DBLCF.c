/* FILE NAME: T03DBLBF.C
 * PROGRAMMER: VG4
 * DATE: 03.06.2013
 * PURPOSE: WinAPI double buffered output.
 */

#include <windows.h>

#include <time.h>

#include <math.h>

#define WND_CLASS_NAME "My window class"

/* ������ ������ */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

/* ������� ������� ���������.
 * ���������:
 *   - ���������� ���������� ����������:
 *       HINSTANCE hInstance;	
 *   - ���������� ����������� ���������� ����������
 *     (�� ������������ � ������ ���� NULL):
 *       HINSTANCE hPrevInstance;
 *   - ��������� ������:
 *       CHAR *CmdLine;
 *   - ���� ������ ����:
 *       INT ShowCmd;
 * ������������ ��������:
 *   (INT) ��� �������� � ������������ �������.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  MSG msg;
  HWND hWnd;

  
  
  wc.style = CS_VREDRAW | CS_HREDRAW; /* ����� ����: ��������� ��������������
                                       * ��� ��������� ������������� ���
                                       * ��������������� �������� */
  wc.cbClsExtra = 0; /* �������������� ���������� ���� ��� ������ */
  wc.cbWndExtra = 0; /* �������������� ���������� ���� ��� ���� */
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW; /* ������� ���� - ��������� � ������� */
  wc.hCursor = LoadCursor(NULL, IDC_ARROW); /* �������� ������� (����������) */
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); /* �������� ����������� (���������) */
  wc.lpszMenuName = NULL; /* ��� ������� ���� */
  wc.hInstance = hInstance; /* ���������� ����������, ��������������� ����� */
  wc.lpfnWndProc = MyWindowFunc; /* ��������� �� ������� ��������� */
  wc.lpszClassName = WND_CLASS_NAME; /* ��� ������ */

  /* ����������� ������ � ������� */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindowA(WND_CLASS_NAME, "> 30 <",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
    10, 10, 1024, 1024, NULL, NULL, hInstance, NULL);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return msg.wParam;
} /* End of 'WinMain' function */

/* ������� ��������� ��������� ����.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 *   - ����� ���������:
 *       UINT Msg;
 *   - �������� ��������� ('word parameter'):
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter'):
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
 */

VOID DrawArrow (HDC hDC,INT Xc,INT Yc,INT Angle,DOUBLE ScaleX, DOUBLE ScaleY)
{
  INT x,y,i;
  POINT pts[] =
  {
    {0,0},{-2,1},{0,6},{2,1}
  };
  DOUBLE a = Angle * 3.14  / 180, si = sin(a), co = cos(a);
  
  for (i = 0; i < sizeof(pts) / sizeof(pts[0]); i++)
  {
    x = (ScaleX * pts[i].x * co - ScaleY * pts[i].y * si) + Xc;
    y = -(ScaleX * pts[i].x * si + ScaleY * pts[i].y * co) + Yc;

    pts[i].x = x;
    pts[i].y = y;

  }
  Polygon(hDC,pts,sizeof(pts)/sizeof(pts[0]));
}


LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  double angle1,angle2,angle3;
  INT x, y,i;
  HDC hDC;
  HPEN hPen, hOldPen;
  HBRUSH hBr, hOldBr;
  CREATESTRUCT *cs;
  PAINTSTRUCT ps;
  POINT pt;
  BITMAP Bm;
  static INT WinW, WinH;
  static HDC hMemDCFrame, hMemDC;
  static HBITMAP hBmFrame, hBmBack, hBmAnd, hBmXor;

  
  SYSTEMTIME st;

  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 30, 5, NULL);

    hBmBack = LoadImage(NULL, "cf.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    /*hBmAnd = LoadImage(NULL, "and1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmXor = LoadImage(NULL, "xor1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);    */

    /*** ������� �������� � ������ ***/
    /* ���������� ������� ���� */
    hDC = GetDC(hWnd);
    /* ������� �������� � ������, ����������� � ���������� ���� */
    hMemDCFrame = CreateCompatibleDC(hDC);
    hMemDC = CreateCompatibleDC(hDC);
    SetPixelV(hMemDC, 100, 100, RGB(0, 0, 0));
    /* ������� �������� ���� */
    ReleaseDC(hWnd, hDC);

    return 0;
  case WM_SIZE:
    WinW = LOWORD(lParam);
    WinH = HIWORD(lParam);

    /*** �������� �������� (��������� ������) ***/
    if (hBmFrame != NULL)
      DeleteObject(hBmFrame);
    /* ������� � ������ �����������, ����������� � ��������� ���� */
    hDC = GetDC(hWnd);
    hBmFrame = CreateCompatibleBitmap(hDC, WinW, WinH);
    ReleaseDC(hWnd, hDC);
    /* �������� �� � �������� ������ */
    SelectObject(hMemDCFrame, hBmFrame);

    /* ��������� ��������� � ����� �������� */
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_TIMER:
    /* ���������� ����� ����� */
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    GetLocalTime(&st);

    SelectObject(hMemDCFrame, GetStockObject(DC_BRUSH));
    SelectObject(hMemDCFrame, GetStockObject(NULL_PEN));

    SetDCBrushColor(hMemDCFrame, RGB(50, 150, 200));
    Rectangle(hMemDCFrame, 0, 0, WinW + 1, WinH + 1);
    SelectObject(hMemDCFrame, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDCFrame, RGB(255, 250, 250));

    GetObject(hBmBack, sizeof(Bm), &Bm);
    SelectObject(hMemDC, hBmBack);
    StretchBlt(hMemDCFrame, 0, 0, WinW, WinH,
      hMemDC, 0, 0, Bm.bmWidth, Bm.bmHeight, SRCCOPY);
    
    angle1 = 2*3.14 / 12 * st.wHour;
    angle2 = 2*3.14 / 60 * st.wMinute;
    angle3 = 2*3.14 / 60 * st.wSecond;


    //hBr = CreateSolidBrush(RGB(0, 0, 0));
    //hOldBr = SelectObject(hMemDCFrame, hBr);

    SetBkColor(hMemDCFrame, RGB(255, 0, 0));
    SetBkMode(hMemDCFrame, TRANSPARENT);
    /* SetBkMode(hDC, OPAQUE); */


    /*hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
    hOldPen = SelectObject(hMemDCFrame, hPen);

    MoveToEx(hMemDCFrame, WinW/2, WinH/2, NULL);
    LineTo(hMemDCFrame,  WinW/7*sin(angle1)+WinW/2, WinH/2 - cos(angle1)* WinH/7);
    
    MoveToEx(hMemDCFrame, WinW/2, WinH/2, NULL);
    LineTo(hMemDCFrame,  WinW/4*sin(angle2)+WinW/2, WinH/2 - cos(angle2)* WinH/4);

    MoveToEx(hMemDCFrame, WinW/2, WinH/2, NULL);
    LineTo(hMemDCFrame,  WinW/3*sin(angle3)+WinW/2, WinH/2 - cos(angle3)* WinH/3);  */

    /*SetROP2(hMemDCFrame, R2_XORPEN);
    MoveToEx(hMemDCFrame, 0, 0, NULL);
    LineTo(hMemDCFrame, WinW, WinH);
    MoveToEx(hMemDCFrame, WinW - 1, 0, NULL);
    LineTo(hMemDCFrame, 0, WinH);
    SetROP2(hMemDCFrame, R2_COPYPEN); */


    SelectObject(hMemDCFrame, GetStockObject(DC_BRUSH));
    SelectObject(hMemDCFrame, GetStockObject(DC_PEN));

    SetDCBrushColor(hMemDCFrame, RGB(0, 0,  0));
    SetDCPenColor(hMemDCFrame, RGB(255, 50, 50));

    DrawArrow(hMemDCFrame,WinW/2,WinH/2,-st.wHour*30,15,70);
    DrawArrow(hMemDCFrame,WinW/2,WinH/2,-st.wMinute*6,15,70);
    DrawArrow(hMemDCFrame,WinW/2,WinH/2,-st.wSecond*6,15,70);
    
        

    
    /* ����� ������������� �������� */
   /* SelectObject(hMemDC, hBmAnd);
    GetObject(hBmAnd, sizeof(Bm), &Bm);
    x = pt.x - Bm.bmWidth / 2;
    y = pt.y - Bm.bmHeight / 2;
    BitBlt(hMemDCFrame, x, y, Bm.bmWidth, Bm.bmHeight,
      hMemDC, 0, 0, SRCAND);
    SelectObject(hMemDC, hBmXor);
    GetObject(hBmXor, sizeof(Bm), &Bm);
    BitBlt(hMemDCFrame, x, y, Bm.bmWidth, Bm.bmHeight,
      hMemDC, 0, 0, SRCINVERT);      */
    
    /* ����� ����������� */
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_PAINT:

    /* ��������� */
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, WinW, WinH, hMemDCFrame, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    /* ���������� �������� � ������ � �������� */
    if (hBmFrame != NULL)
      DeleteObject(hBmFrame);
    if (hMemDCFrame != NULL)
      DeleteDC(hMemDCFrame);

    /* ������� ��������� 'WM_QUIT' � ���������� 'wParam' 0 - ��� ��������,
     * ������ �����: PostMessage(hWnd, WM_QUIT, ���_��������, 0); */
    PostQuitMessage(30);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */


/* END OF 'T03DBLBF.C' FILE */
