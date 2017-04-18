#ifndef __BBALL_H_
#define __BBALL_H_
 

#include "ANIM.h"
#include "vec.h"
#include <math.h>
#include <stdlib.h>

static float radius = 0.1;
static float a = 6.0;
static float border = (a - radius-0.1)/2;
static int g = 0;
static double X,Y,Z;

class ball : public bgl::object
{
protected:
 Vec POS;
 Vec VEL;

public: 
  ball (void)
  {
    POS.X=(3.0*rand()/RAND_MAX-3);
    POS.Y=(3.0*rand()/RAND_MAX-3);
    POS.Z=(3.0*rand()/RAND_MAX-3);
 
 
   
  
    VEL.X=(0.001*rand()/RAND_MAX-0.005);
    VEL.Y=(0.001*rand()/RAND_MAX-0.005);
    VEL.Z=(0.001*rand()/RAND_MAX-0.005);
  }
  
  virtual void Handle(bgl:: anim & ANI)
  {
    POS = POS + VEL;
 
    if(POS.X>=border)
      VEL.X = -VEL.X;
    if(POS.Y>=border)
      VEL.Y = -VEL.Y;
    if(POS.Z>=border)
      VEL.Z = -VEL.Z;
    if(POS.X<=-border)
      VEL.X = -VEL.X;
    if(POS.Y<=-border)
      VEL.Y = -VEL.Y;
    if(POS.Z<=-border)
      VEL.Z = -VEL.Z;
    }
  virtual void Render(bgl:: anim & ANI)
  {
    glPushMatrix();
    glRotated(ANI.SyncTime*30,0,1,0);
    glColor3d(fabs(POS.Y)-0.4,fabs(POS.X)-0.4,fabs(POS.Z)-0.4);
    glTranslated(POS.X,POS.Y,POS.Z);
    glutSolidSphere(radius,30,30);
    glPopMatrix();
  }
};
class cubic :public ball
{
  virtual void Render (bgl:: anim & ANI)
  {
    glPushMatrix();
    glRotated(ANI.SyncTime*30,0,1,0);
    glColor3d(fabs(POS.X)-0.4,fabs(POS.Z)-0.4,fabs(POS.Y)-0.4);
    glTranslated(0,0,0);
    glutWireCube(a);
    glPopMatrix();
  }
};

class cubic2 :public ball
{
  virtual void Render (bgl:: anim & ANI)
  {
    glPushMatrix();
    glRotated(ANI.SyncTime*30,0,1,0);
    glColor3d(fabs(POS.Z)-0.4,fabs(POS.X)-0.4,fabs(POS.Y)-0.4);
    glTranslated(POS.X,POS.Y,POS.Z);
    glutSolidCube(0.2);
    glPopMatrix();
  }
};

class camera :public ball
{
  virtual void Handle (bgl:: anim & ANI)
  { 
    ball::Handle(ANI);
    gluLookAt(5,5,5, POS.X ,POS.Y ,POS.Z, 0, 1, 0);
    ball::Render(ANI);
  }

};

#endif