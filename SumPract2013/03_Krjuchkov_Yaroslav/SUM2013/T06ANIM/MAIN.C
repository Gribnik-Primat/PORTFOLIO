/* FILE NAME: MAIN.C
 * PROGRAMMER: IK
 * DATE: 06.06.2013
 * PURPOSE: Main math support implementation module.
 */

#include <glew.h>
#include <windows.h>
#pragma comment(lib, "glew32")
#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>
#include <time.h>
//#include "vec.h"
#include "anim.h"

#define WND_CLASS_NAME "MainWindowClass"
#define ANIMATION_TIMER 1


LRESULT CALLBACK TranslateMessages( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );


INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  INT i;



  for (i = 0; i < 3; i++)
  { 
    IK1_AnimAdd(CowCreate(rand() % 300 + 200, rand() % 300 + 200));

   /* IK1_AnimAdd(CowCreate(/*rand() % 300 + 300,/* rand() % 300 + 300));

    /*IK1_AnimAdd(CowCreate(/*rand() % 300 + 400,/* rand() % 300 + 400));*/
  }


  wc.style = CS_VREDRAW | CS_HREDRAW; 
  wc.cbClsExtra = 0; 
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW; 
  wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.lpszMenuName = NULL; 
  wc.hInstance = hInstance; 
  wc.lpfnWndProc = TranslateMessages;
  wc.lpszClassName = WND_CLASS_NAME; 

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindowA(WND_CLASS_NAME, "T06ANIM",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
    0, 0, 1920, 1080, NULL, NULL, hInstance, NULL);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return msg.wParam;
}




 INT w, h;
LRESULT CALLBACK TranslateMessages( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  
  HDC hDC;
  CREATESTRUCT *cs;
  PAINTSTRUCT ps;

  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    if (!IK1_AnimInit(hWnd))
      return -1;
    SetTimer(hWnd, ANIMATION_TIMER, 5, NULL);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    IK1_AnimResize(w, h);
    return 0;
  case WM_TIMER:
    IK1_AnimRender();
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    IK1_AnimCopyFrame();
    return 0;
  case WM_DESTROY:
    IK1_AnimClose();
    PostQuitMessage(0);
    KillTimer(hWnd, ANIMATION_TIMER);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}