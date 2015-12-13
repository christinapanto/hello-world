#include "display.h"
#include <stdlib.h>


void run()
{
  display *d = newDisplay();
  Button *buttons;
  int i,j,stop=0,leave=0,which_alien;

  drawEnity( d,0, 0, 0);
while (!stop){
i=420,j=230;
which_alien=rand()%4+1;
while((i>230 || j<420) && !stop){
 drawEnity( d,0, 0, 0);
 if (getEvent(d,buttons)==2){
   leave=1;
 }
  if (i>230){
  drawEnity( d,which_alien, 300, i);
    i--;
    leave=0;
  }
  if (leave==0 && i==230) {
    drawEnity( d,which_alien, 300,230);
}
    if (j<420 && leave ==1){
    drawEnity( d,which_alien, 300, j);
    j++;
  }
 drawFrame(d, buttons);
 if(getEvent(d,buttons)==1){
    stop=1;
 }
}
}
  QuitGame(d);


}

int main()
{
  run();
}
