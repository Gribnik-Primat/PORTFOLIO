#include <stdlib.h>
#include "bball.h"

bgl::anim bgl::anim::Instance;


void bgl :: anim::Timer( void )
{
  static long starttime = -1, oldtime, pausetime = 0;
  long t;
  t = clock();
 
  if (starttime == -1)
    starttime = oldtime = t;

  if (IsPause)
    DeltaTime = 0, pausetime += t - oldtime; 
  else
    DeltaTime = (t - oldtime) / (double)CLOCKS_PER_SEC;

  SyncTime = (t - starttime - pausetime - TimeShift) / (double)CLOCKS_PER_SEC;

  oldtime = t;
}  

bgl::anim::anim(void): Near(1),Far(1000)
{
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(1920, 1080);
  glutCreateWindow("11-3");

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.3, 0.5, 0.7, 1); 

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);

  glutIdleFunc(Idle);
  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);
  glutReshapeFunc(Reshape);
 
}

void bgl :: anim::SetProj( void )
{
  double ratio_x = 1, ratio_y = 1;

  if (Instance.WinW >= Instance.WinH)
     ratio_x *= (double)Instance.WinW / Instance.WinH;
  else
     ratio_y *= (double)Instance.WinH / Instance.WinW;

  glMatrixMode(GL_PROJECTION);  
  glLoadIdentity();
  glFrustum(-ratio_x, ratio_x, -ratio_y, ratio_y, Instance.Near, Instance.Far);
  glMatrixMode(GL_MODELVIEW);  
}


void bgl :: anim::Display(void)
{
   Instance.Timer(); 
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();

   gluLookAt(10,15,15,0,0,0,0,1,0);

   for(int i =0;i<Instance.NumOfObj;i++)
     Instance.Objects[i]->Handle(Instance);
   for(int i =0;i<Instance.NumOfObj;i++)
   { 
     glPushMatrix();
     glPushAttrib(GL_ALL_ATTRIB_BITS);
     Instance.Objects[i]->Render(Instance);
     glPopAttrib();
     glPopMatrix();
   }                         

   glFinish();
   glutSwapBuffers();
}
void bgl :: anim::Keyboard(unsigned char key, int x, int y)
{
  if(key == 27)
    exit(0);
  else if (key == 'f')
     glutFullScreen();
  else if (key == 'p')
    Instance.IsPause = !Instance.IsPause;
  else if (key == 'a')
    Instance.TimeShift += CLOCKS_PER_SEC / 30;
  else if (key == 'z')
    Instance.TimeShift -= CLOCKS_PER_SEC / 30;
  
}
void bgl :: anim::Reshape(int x, int y)
{
  Instance.WinW = x;
  Instance.WinH = y;
  glViewport(0, 0, Instance.WinW, Instance.WinH);
  Instance.SetProj();
}
void bgl :: anim::Idle(void)
{
  glutPostRedisplay();
}





