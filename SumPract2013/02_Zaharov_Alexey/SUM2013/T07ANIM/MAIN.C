/* Zakharov Alexey, 10-1, Summer Practise 2013 */

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
  INT i, j;

  for (i = 0; i < 1; i++)
   // for (j = 0; j < 5; j++)
    {
      AZ1_AnimAdd(TestUnitCreate(i, 0, 1, "cessna172.object"));
    }

  //AZ1_AnimAdd(TestUnitCreate(1, 1, 1, "Models\\Houses\\house1.object"));

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
    10, 10, 480, 480, NULL, NULL, hInstance, NULL);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

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
    if (!AZ1_AnimInit(hWnd))
      return -1;
    SetTimer(hWnd, ANIMATION_TIMER, 5, NULL);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    AZ1_AnimResize(w, h);
    return 0;
  case WM_TIMER:
    AZ1_AnimRender();
    AZ1_AnimCopyFrame();   
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    AZ1_AnimCopyFrame();
    return 0;
  case WM_DESTROY:
    AZ1_AnimClose();
    PostQuitMessage(0);
    KillTimer(hWnd, ANIMATION_TIMER);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}