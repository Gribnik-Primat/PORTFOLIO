#ifndef __ANIM_H_
#define __ANIM_H_
 
#include <glut.h>
#include <time.h>
namespace bgl
{
  class anim;

  class object
  {
  public:
    virtual void Handle(anim & ANI)
    {
    }
    virtual void Render(anim & ANI)
    {
    }
  };  

  class anim
  {
  private:
    static const int MaxObj = 300;
    object *Objects[MaxObj];
    int NumOfObj;


    static void Display(void);
    static void Reshape(int x, int y);
    static void Keyboard(unsigned char key,int x, int y);
    static void Idle(void);
    void SetProj( void );
    void Timer( void );
    anim (void);

    static anim Instance;
  public:
    int WinW ,WinH;
    int Size,Near,Far;
    double SyncTime, DeltaTime;
    int  TimeShift ;
    bool IsPause;
    
    anim & operator << (object *Obj)
    {
      if(NumOfObj < MaxObj)
        Objects[NumOfObj++] = Obj;
      return *this;
    }

    void Run (void)
    {
      glutMainLoop();
    }
    static anim & GetRef(void)
    {
      return Instance;
    }
  };
}

#endif