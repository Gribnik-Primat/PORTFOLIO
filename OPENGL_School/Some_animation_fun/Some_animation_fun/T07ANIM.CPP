#include "ANIM.h"
#include "bball.h"
#include "flyers.h"
void main (void)
{
  bgl::anim &My = bgl :: anim :: GetRef();
  My<< new camera()<<new cubic()<<new elki();
  for(int i = 0;i<20;i++)
    My<<new elki()<<new snowman();
 // for(int i = 0;i<100;i++)
    // My<<new bird();

  My.Run();
}

