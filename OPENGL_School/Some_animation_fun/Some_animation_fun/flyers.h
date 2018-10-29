#ifndef __FLYERS_H_
#define __FLYERS_H_

#include <stdlib.h>
#include "anim.h"

class elki : public bgl::object
{
protected:
    Vec POST;
    double c;
public:
  elki (void):c(60.0*rand()/RAND_MAX-60)
  {
    POST.X=(30.0*rand()/RAND_MAX-15);
    POST.Y=(30.0*rand()/RAND_MAX-15);
    POST.Z=(30.0*rand()/RAND_MAX-15);

  }                                          
  void Render (bgl:: anim & ANI)
  {
    glPushMatrix();
    glTranslated(POST.X,0,POST.Z);
    
    glRotated(-90,1,0,0);
    glColor3d(0.5,0.3,0.1);
    glutSolidCone(0.2,1.5,30,30);
    glTranslated(0,0,0.5);
    for(int i = 0;i < 4;i++)
    {
      glColor3d(0,0.5,0);
      glutSolidCone(1,1.5,30,30);
      glTranslated(0,0,0.5);
      glRotated(sin(ANI.SyncTime * 4+c)*30 ,1,1,1);
    }
    glPopMatrix();
  }

};
class snowman : public bgl::object
{
protected:
    Vec POSS;
public:
  snowman (void)
  {
    POSS.X=(30.0*rand()/RAND_MAX-15);
    POSS.Y=(30.0*rand()/RAND_MAX-15);
    POSS.Z=(30.0*rand()/RAND_MAX-15);

  }                                          
  void Render (bgl:: anim & ANI)
  {
    glPushMatrix();
    glTranslated(POSS.X,0,POSS.Z);
    glColor3d(0,0,0);
    glutSolidSphere(1.2,30,30);
    
    glTranslated(2,0,2*sin(ANI.SyncTime*3));
    glutSolidSphere(1.2,30,30);
    glTranslated(-1,0.5,1*-sin(ANI.SyncTime*3));
    glColor3d(1,1,1);
    glutSolidSphere(2,30,30);
    glTranslated(0,1.8,0);
    glutSolidSphere(1.5,30,30);
    glTranslated(0,1.8,0);
    glutSolidSphere(1,30,30);
    glColor3d(0.9,0.3,0.1);
    glutSolidCone(0.5,3,30,30);
    glColor3d(0,0,0);
    glTranslated(0.5,0,0);
    glutSolidSphere(0.75,30,30);
    glTranslated(-1,0,0);
    glutSolidSphere(0.75,30,30);
    glPopMatrix();
  }

};

class snow : public bgl::object
{
protected:
    Vec SNOW;
    int v;
public:
  snow (void):v(0.5)
  {
    SNOW.X=(30.0*rand()/RAND_MAX-15);
    SNOW.Y=(30.0*rand()/RAND_MAX);
    SNOW.Z=(30.0*rand()/RAND_MAX-15);

  }                                          
  void Render (bgl:: anim & ANI)
  { if(SNOW.Y > 0)
      SNOW.Y += (rand()/RAND_MAX-0.5);
    glPushMatrix();
      glColor3d(1,1,1);
      glTranslated(SNOW.X,SNOW.Y,SNOW.Z);
      glutSolidSphere(0.05,30,30);
  
    glPopMatrix();
  }

};
class bird : public bgl::object
{
protected:
    Vec POS;
public:
  bird (void)
  {
    POS.X=(30.0*rand()/RAND_MAX-15);
    POS.Y=(30.0*rand()/RAND_MAX);
    POS.Z=(30.0*rand()/RAND_MAX-15);

  }                                          
  void Render (bgl:: anim & ANI)
  {
    POS.X += sin(ANI.SyncTime)*3;
    POS.Z += sin(ANI.SyncTime)*3;
    glPushMatrix();
    glTranslated(0,0,0);
    glBegin(GL_TRIANGLES);
      glColor3d(0.9,0.5,0.1);
      glVertex3d(0,0,0);
      glVertex3d(6,0,0);
      glVertex3d(4.5,3*fabs(sin(ANI.SyncTime)),6-fabs(sin(ANI.SyncTime)));

      glVertex3d(0,0,0);
      glVertex3d(6,0,0);
      glVertex3d(4.5,3*fabs(sin(ANI.SyncTime)),-6-fabs(sin(ANI.SyncTime)));

      glVertex3d(4.5,3*fabs(sin(ANI.SyncTime)),-6-fabs(sin(ANI.SyncTime)));
      glVertex3d(1.5,3*fabs(sin(ANI.SyncTime)),-6-fabs(sin(ANI.SyncTime)));
      glVertex3d(0,0,0);

      glVertex3d(4.5,3*fabs(sin(ANI.SyncTime)),6-fabs(sin(ANI.SyncTime)));
      glVertex3d(1.5,3*fabs(sin(ANI.SyncTime)),6-fabs(sin(ANI.SyncTime)));
      glVertex3d(0,0,0);

      glVertex3d(4.5,3*fabs(sin(ANI.SyncTime)),6-fabs(sin(ANI.SyncTime)));
      glVertex3d(1.5,3*fabs(sin(ANI.SyncTime)),6-fabs(sin(ANI.SyncTime)));
      glVertex3d(3,3*fabs(sin(ANI.SyncTime-45)),9-fabs(sin(ANI.SyncTime)));

      glVertex3d(4.5,3*fabs(sin(ANI.SyncTime)),-6-fabs(sin(ANI.SyncTime)));
      glVertex3d(1.5,3*fabs(sin(ANI.SyncTime)),-6-fabs(sin(ANI.SyncTime)));
      glVertex3d(3,3*fabs(sin(ANI.SyncTime-45)),-9-fabs(sin(ANI.SyncTime)));
    glEnd();
  
    glPopMatrix();
  }

};

#endif __FLYERS_H_