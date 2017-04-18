/* FILE NAME: MAIN.C
 * PROGRAMMER: IB1
 * DATE: 06.06.2013
 * PURPOSE: Main math support implementation module.
 */

#pragma comment(lib, "glew32")
#include <glew.h>

#include <windows.h>

#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>
#include <time.h>
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
  INT i, j, k;

  //LoadRawFile( "Region0000.raw", MAP_SIZE * MAP_SIZE );
  //IB1_AnimAdd(TestUnitCreate(0, 0, 0, "cube.obj"));
  IB1_AnimAdd(TestUnitCreate(0, 1000, 0, "cessna172.object"));
  for (i = 0; i < 1; i++)
    IB1_AnimAdd(TestUnitCreate(5000 - rand() % 10000, 0, 5000 - rand() % 10000, "house.obj"));
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

  hWnd = CreateWindowA(WND_CLASS_NAME, "T07ANIM",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN,
    0, 0, 1920, 1080, NULL, NULL, hInstance, NULL);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return msg.wParam;
}






INT w, h;
void LoadCow( void );
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
    LoadCow();
    if (!IB1_AnimInit(hWnd))
      return -1;
    SetTimer(hWnd, ANIMATION_TIMER, 1, NULL);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    IB1_AnimResize(w, h);
    return 0;
  case WM_TIMER:
    IB1_AnimRender();
    IB1_AnimCopyFrame();   
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    IB1_AnimCopyFrame();
    return 0;
  case WM_DESTROY:
    IB1_AnimClose();
    PostQuitMessage(0);
    KillTimer(hWnd, ANIMATION_TIMER);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}