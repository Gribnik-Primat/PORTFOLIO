#include <math.h>
#include <glut.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <stdio.h>
#include <time.h>

struct COMPLE
{
  double Re,Im;
  COMPLE (void)
  {
  }
  COMPLE (double a ,double b): Re(a), Im(b)
  {
  }
  COMPLE  operator+ (const COMPLE &z)const
  {
    return COMPLE(Re + z.Re,Im + z.Im);
  }
  COMPLE  operator* (const COMPLE &z)const
  {
    return COMPLE(Re * z.Re - Im * z.Im,Re * z.Im + Im * z.Re);
  }
  int Julia(const COMPLE &z2)
  { 
    COMPLE z1(Re,Im);
    int n = 0;
    while(n<255 && (z1.Re*z1.Re + z1.Im*z2.Im)<4)
    {
      z1 = ((z1 * z1) + z2);
      n++;
    }
    return n; 
  }
};


