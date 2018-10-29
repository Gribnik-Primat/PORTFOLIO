#include "COMPL.h"

#define Weight 1920
#define Height 1080


#define x0 -1.0
#define x1 1.0
#define y0 -1.0
#define y1 1.0


struct Frame
{
  typedef unsigned char byte;
  int H, W;
  static byte Image [Height][Weight][3];
  
  void frame (double t)
  {
    COMPLE z1,z2;
    double c;
    H = Height;
    W = Weight;
    
      for (int ys=0;ys<H;ys++)
      {
        double b=y1-ys*(y1-y0/**sin(t)*0.3*/)/H /*- 0.5*/;
        for(int xs=0;xs<W;xs++)
        {
          double a = x0+xs*(x1-x0/*sin(t)*0.3*/)/W /*- 0.5*/;
		  c = 0.39 + sin(t) * 0.02;
          z1 = COMPLE(a,b);
          z2 = COMPLE(c,c);
          
          int n = z1.Julia(z1);
          Image[ys][xs][1]=n*n*sin(t)*0.01;
          Image[ys][xs][2]=n*n*n*cos(t)*0.01;
          Image[ys][xs][3]=n*n*cos(t)*0.01;
        }
      }
  }
  void Draw(void)
  {
    glDrawPixels(W,H,GL_BGR_EXT,GL_UNSIGNED_BYTE,Image);
  }
};

Frame::byte Frame::Image[Height][Weight][3];

void Display (void)
{
  Frame F;  
  glClearColor(0.3,0.5,0.7,1);
  glClear(GL_COLOR_BUFFER_BIT);
  
  
  glRasterPos2d(-1,1);
  glPixelZoom(1,-1);
  
  F.frame(clock()/(double)CLOCKS_PER_SEC);
  F.Draw();

  glFinish();

  glutSwapBuffers();

  glutPostRedisplay();
}


void keyboard(unsigned char Key,int x,int y)
{
  
  if (Key == 'f')
    glutFullScreen();
}


void main(int argc, char *argv[])
{
  
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);


  glutInitWindowPosition(0,0);
  glutInitWindowSize(Weight,Height);
  glutCreateWindow("11-3");

  glutDisplayFunc(Display);
  glutKeyboardFunc(keyboard);
  
  glutMainLoop();
}
