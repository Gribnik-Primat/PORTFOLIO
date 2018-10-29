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
 HWND hWnd;

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;

  MSG msg;

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
    10, 10, 1000, 1000, NULL, NULL, hInstance, NULL);

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

HDC hDC;
HPEN hPen, hOldPen;
HBRUSH hBr, hOldBr;
VOID drawdigit(INT x,INT y,INT num)
{
  int n1 = 0,n2 = 0;
  if (num >= 10)
  {
    n1 = num/10;
    n2 = num%10;
  }
  else
    n2 = num;

 

  hDC = GetDC(hWnd);

  switch (n1)
  {
    case 0:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      MoveToEx(hDC, x, y - 3, NULL);
      LineTo(hDC, x, y - 15);
      
      MoveToEx(hDC, x, y - 18, NULL);
      LineTo(hDC, x, y - 33 );
      
      MoveToEx(hDC, x + 3, y - 33, NULL);
      LineTo(hDC,  x + 18, y - 33);
      
      MoveToEx(hDC, x + 18, y - 30 , NULL);
      LineTo(hDC, x + 18, y - 18);
       
      MoveToEx(hDC, x + 18, y - 15, NULL);
      LineTo(hDC,  x + 18, y);
      
      MoveToEx(hDC, x + 15, y , NULL);
      LineTo(hDC, x, y);
      break;

    case 1:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      MoveToEx(hDC, x , y - 30 , NULL);
      LineTo(hDC, x , y - 18);
       
      MoveToEx(hDC, x, y - 15, NULL);
      LineTo(hDC,  x, y);
      
  
      break;

    case 5:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      
      
      MoveToEx(hDC, x, y - 18, NULL);
      LineTo(hDC, x, y - 33 );
      
      MoveToEx(hDC, x + 3, y - 33, NULL);
      LineTo(hDC,  x + 18, y - 33);
       
      MoveToEx(hDC, x + 18, y - 15, NULL);
      LineTo(hDC,  x + 18, y);
      
      MoveToEx(hDC, x + 15, y , NULL);
      LineTo(hDC, x, y);

      MoveToEx(hDC, x , y -16 , NULL);
      LineTo(hDC, x + 18, y -16 );
      break;

    case 3:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      MoveToEx(hDC, x + 3, y - 33, NULL);
      LineTo(hDC,  x + 18, y - 33);
      
      MoveToEx(hDC, x + 18, y - 30 , NULL);
      LineTo(hDC, x + 18, y - 18);
       
      MoveToEx(hDC, x + 18, y - 15, NULL);
      LineTo(hDC,  x + 18, y);
      
      MoveToEx(hDC, x + 15, y , NULL);
      LineTo(hDC, x, y);

      MoveToEx(hDC, x , y -16 , NULL);
      LineTo(hDC, x + 18, y -16 );
      break;
    case 4:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      MoveToEx(hDC, x, y - 15, NULL);
      LineTo(hDC, x, y - 33);
      
      MoveToEx(hDC, x + 18, y - 30 , NULL);
      LineTo(hDC, x + 18, y - 18);
       
      MoveToEx(hDC, x + 18, y - 15, NULL);
      LineTo(hDC,  x + 18, y);

      MoveToEx(hDC, x , y -16 , NULL);
      LineTo(hDC, x + 18, y -16 );
      break;    
    case 2:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      MoveToEx(hDC, x, y - 3, NULL);
      LineTo(hDC, x, y - 15);
      
      MoveToEx(hDC, x + 3, y - 33, NULL);
      LineTo(hDC,  x + 18, y - 33);
      
      MoveToEx(hDC, x + 18, y - 30 , NULL);
      LineTo(hDC, x + 18, y - 18);
      
      MoveToEx(hDC, x + 15, y , NULL);
      LineTo(hDC, x, y);

      MoveToEx(hDC, x , y -16 , NULL);
      LineTo(hDC, x + 18, y -16 );
      break;
    case 6:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      MoveToEx(hDC, x, y - 3, NULL);
      LineTo(hDC, x, y - 15);
      
      MoveToEx(hDC, x, y - 18, NULL);
      LineTo(hDC, x, y - 33 );
      
      MoveToEx(hDC, x + 3, y - 33, NULL);
      LineTo(hDC,  x + 18, y - 33);
      
      MoveToEx(hDC, x + 18, y  , NULL);
      LineTo(hDC, x + 18, y -18  );
      
      MoveToEx(hDC, x + 15, y , NULL);
      LineTo(hDC, x, y);

      MoveToEx(hDC, x , y -16 , NULL);
      LineTo(hDC, x + 18, y -16 );
      break;

      case 7:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      
      MoveToEx(hDC, x + 18, y - 30 , NULL);
      LineTo(hDC, x + 18, y - 18);
       
      MoveToEx(hDC, x + 18, y - 15, NULL);
      LineTo(hDC,  x + 18, y);
      
      MoveToEx(hDC, x , y - 33 , NULL);
      LineTo(hDC, x+15 , y - 33);


   
      break;
    case 8:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      MoveToEx(hDC, x, y - 3, NULL);
      LineTo(hDC, x, y - 15);
      
      MoveToEx(hDC, x, y - 18, NULL);
      LineTo(hDC, x, y - 33 );
      
      MoveToEx(hDC, x + 3, y - 33, NULL);
      LineTo(hDC,  x + 18, y - 33);
      
      MoveToEx(hDC, x + 18, y - 30 , NULL);
      LineTo(hDC, x + 18, y - 18);
       
      MoveToEx(hDC, x + 18, y - 15, NULL);
      LineTo(hDC,  x + 18, y);
      
      MoveToEx(hDC, x + 15, y , NULL);
      LineTo(hDC, x, y);

      MoveToEx(hDC, x , y -16 , NULL);
      LineTo(hDC, x + 18, y -16 );
      break;
      case 9:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      MoveToEx(hDC, x, y - 15, NULL);
      LineTo(hDC, x, y -33);
      
    
      MoveToEx(hDC, x + 3, y - 33, NULL);
      LineTo(hDC,  x + 18, y - 33);
      
      MoveToEx(hDC, x + 18, y - 30 , NULL);
      LineTo(hDC, x + 18, y - 18);
       
      MoveToEx(hDC, x + 18, y - 15, NULL);
      LineTo(hDC,  x + 18, y);
      
      MoveToEx(hDC, x + 15, y , NULL);
      LineTo(hDC, x, y);

      MoveToEx(hDC, x , y -16 , NULL);
      LineTo(hDC, x + 18, y -16 );
      break;
  }
  x += 35;
  switch (n2)
  {
    case 0:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      MoveToEx(hDC, x, y - 3, NULL);
      LineTo(hDC, x, y - 15);
      
      MoveToEx(hDC, x, y - 18, NULL);
      LineTo(hDC, x, y - 33 );
      
      MoveToEx(hDC, x + 3, y - 33, NULL);
      LineTo(hDC,  x + 18, y - 33);
      
      MoveToEx(hDC, x + 18, y - 30 , NULL);
      LineTo(hDC, x + 18, y - 18);
       
      MoveToEx(hDC, x + 18, y - 15, NULL);
      LineTo(hDC,  x + 18, y);
      
      MoveToEx(hDC, x + 15, y , NULL);
      LineTo(hDC, x, y);
      break;

    case 1:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      MoveToEx(hDC, x , y - 30 , NULL);
      LineTo(hDC, x , y - 18);
       
      MoveToEx(hDC, x, y - 15, NULL);
      LineTo(hDC,  x, y);
      
  
      break;

    case 5:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      
      
      MoveToEx(hDC, x, y - 18, NULL);
      LineTo(hDC, x, y - 33 );
      
      MoveToEx(hDC, x + 3, y - 33, NULL);
      LineTo(hDC,  x + 18, y - 33);
       
      MoveToEx(hDC, x + 18, y - 15, NULL);
      LineTo(hDC,  x + 18, y);
      
      MoveToEx(hDC, x + 15, y , NULL);
      LineTo(hDC, x, y);

      MoveToEx(hDC, x , y -16 , NULL);
      LineTo(hDC, x + 18, y -16 );
      break;

    case 3:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      MoveToEx(hDC, x + 3, y - 33, NULL);
      LineTo(hDC,  x + 18, y - 33);
      
      MoveToEx(hDC, x + 18, y - 30 , NULL);
      LineTo(hDC, x + 18, y - 18);
       
      MoveToEx(hDC, x + 18, y - 15, NULL);
      LineTo(hDC,  x + 18, y);
      
      MoveToEx(hDC, x + 15, y , NULL);
      LineTo(hDC, x, y);

      MoveToEx(hDC, x , y -16 , NULL);
      LineTo(hDC, x + 18, y -16 );
      break;
    case 4:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      MoveToEx(hDC, x, y - 15, NULL);
      LineTo(hDC, x, y - 33);
      
      MoveToEx(hDC, x + 18, y - 30 , NULL);
      LineTo(hDC, x + 18, y - 18);
       
      MoveToEx(hDC, x + 18, y - 15, NULL);
      LineTo(hDC,  x + 18, y);

      MoveToEx(hDC, x , y -16 , NULL);
      LineTo(hDC, x + 18, y -16 );
      break;    
    case 2:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      MoveToEx(hDC, x, y - 3, NULL);
      LineTo(hDC, x, y - 15);
      
      MoveToEx(hDC, x + 3, y - 33, NULL);
      LineTo(hDC,  x + 18, y - 33);
      
      MoveToEx(hDC, x + 18, y - 30 , NULL);
      LineTo(hDC, x + 18, y - 18);
      
      MoveToEx(hDC, x + 15, y , NULL);
      LineTo(hDC, x, y);

      MoveToEx(hDC, x , y -16 , NULL);
      LineTo(hDC, x + 18, y -16 );
      break;
    case 6:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      MoveToEx(hDC, x, y - 3, NULL);
      LineTo(hDC, x, y - 15);
      
      MoveToEx(hDC, x, y - 18, NULL);
      LineTo(hDC, x, y - 33 );
      
      MoveToEx(hDC, x + 3, y - 33, NULL);
      LineTo(hDC,  x + 18, y - 33);
      
      MoveToEx(hDC, x + 18, y  , NULL);
      LineTo(hDC, x + 18, y -18  );
      
      MoveToEx(hDC, x + 15, y , NULL);
      LineTo(hDC, x, y);

      MoveToEx(hDC, x , y -16 , NULL);
      LineTo(hDC, x + 18, y -16 );
      break;

      case 7:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      
      MoveToEx(hDC, x + 18, y - 30 , NULL);
      LineTo(hDC, x + 18, y - 18);
       
      MoveToEx(hDC, x + 18, y - 15, NULL);
      LineTo(hDC,  x + 18, y);
      
      MoveToEx(hDC, x , y - 33 , NULL);
      LineTo(hDC, x+15 , y - 33);


   
      break;
    case 8:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      MoveToEx(hDC, x, y - 3, NULL);
      LineTo(hDC, x, y - 15);
      
      MoveToEx(hDC, x, y - 18, NULL);
      LineTo(hDC, x, y - 33 );
      
      MoveToEx(hDC, x + 3, y - 33, NULL);
      LineTo(hDC,  x + 18, y - 33);
      
      MoveToEx(hDC, x + 18, y - 30 , NULL);
      LineTo(hDC, x + 18, y - 18);
       
      MoveToEx(hDC, x + 18, y - 15, NULL);
      LineTo(hDC,  x + 18, y);
      
      MoveToEx(hDC, x + 15, y , NULL);
      LineTo(hDC, x, y);

      MoveToEx(hDC, x , y -16 , NULL);
      LineTo(hDC, x + 18, y -16 );
      break;
      case 9:
      SelectObject(hDC, GetStockObject(DC_PEN));
      SetDCPenColor(hDC, RGB(0, 140, 0));
      MoveToEx(hDC, x, y - 15, NULL);
      LineTo(hDC, x, y -33);
      
    
      MoveToEx(hDC, x + 3, y - 33, NULL);
      LineTo(hDC,  x + 18, y - 33);
      
      MoveToEx(hDC, x + 18, y - 30 , NULL);
      LineTo(hDC, x + 18, y - 18);
       
      MoveToEx(hDC, x + 18, y - 15, NULL);
      LineTo(hDC,  x + 18, y);
      
      MoveToEx(hDC, x + 15, y , NULL);
      LineTo(hDC, x, y);

      MoveToEx(hDC, x , y -16 , NULL);
      LineTo(hDC, x + 18, y -16 );
      break;
  }
  

  ReleaseDC(hWnd, hDC);
}    




LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  double angle1,angle2,angle3,angle4;
  int d[10]={0,1,2,3,4,5,6,7,8,9};
  static CHAR Buf[100];
  CREATESTRUCT *cs;
  SYSTEMTIME st;


  switch (Msg)
  {
  case WM_CREATE:
    cs =(CREATESTRUCT *)lParam;
    SetTimer(hWnd,30,500,NULL);
    return 0;
  case  WM_LBUTTONDOWN:
    SetCapture(hWnd);
    return 0;
  case WM_LBUTTONUP:
    SetCapture(NULL);
    return 0;


  case WM_TIMER:
    GetLocalTime(&st);
  



    hDC = GetDC(hWnd);

    
    hBr = CreateSolidBrush(RGB(255, 255, 255 ));
    hOldBr = SelectObject(hDC, hBr);
    hPen = CreatePen(PS_SOLID, 5, RGB(255, 255,255));
    hOldPen = SelectObject(hDC, hPen);
    Rectangle(hDC,0,0,5000,5000);      

    
    /* hBr = CreateSolidBrush(RGB(255, 255, 0)); */
    
    
    hBr = CreateHatchBrush(HS_DIAGCROSS, RGB(255, 255, 0));
    hOldBr = SelectObject(hDC, hBr);
    SetBkColor(hDC, RGB(0, 0, 255));
    SetBkMode(hDC, TRANSPARENT);
    /* SetBkMode(hDC, OPAQUE); */

    hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
    hOldPen = SelectObject(hDC, hPen);

 
    Ellipse(hDC, 300, 300, 600, 600);
    
     

    angle1 = 2*3.14 / 12 * st.wHour;
    angle2 = 2*3.14 / 60 * st.wMinute;
    angle3 = 2*3.14 / 60 * st.wSecond;



    MoveToEx(hDC, 450, 450, NULL);
    LineTo(hDC,  75*sin(angle1)+450, 450 - cos(angle1)*150);
    
    MoveToEx(hDC, 450, 450, NULL);
    LineTo(hDC,  100*sin(angle2)+450, 450 - cos(angle2)*150);

    MoveToEx(hDC, 450, 450, NULL);
    LineTo(hDC,  125*sin(angle3)+450, 450 - cos(angle3)*150);


    TextOut(hDC,700, 700,Buf,sprintf(Buf,"%02i : %02i : %02i",st.wHour,st.wMinute,st.wSecond));



    drawdigit(270,700,st.wHour);
 
    drawdigit(360,700,st.wMinute);
 
    drawdigit(470,700,st.wSecond);
     

    ReleaseDC(hWnd, hDC);
 

    return 0;
  case WM_DESTROY:
    /* ������� ��������� 'WM_QUIT' � ���������� 'wParam' 0 - ��� ��������,
     * ������ �����: PostMessage(hWnd, WM_QUIT, ���_��������, 0); */
    PostQuitMessage(30);
    KillTimer(hWnd,30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'T00PARAMS.C' FILE */
