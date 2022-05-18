/*
A simple example of using the gfx library.
CSE 20211
9/7/2011
by Prof. Thain
*/

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "gfx.h"

const char *pDevice = "/dev/input/mice";

int main()
{
	int ysize = 300;
	int xsize = 300;
  int i = 0;
	char event;
  int fd;
  int bytes;
  unsigned char data[3];
  int left, middle, right;
  int toggle = 0;

  fd = open(pDevice, O_RDONLY | O_NONBLOCK);
  if (fd == -1) {
    printf("ERROR Opening %s\n", pDevice);
    return -1;
  }
  

	// Open a new window for drawing.
	gfx_open(xsize,ysize,"SAND II: GRITTIER");

	// Set the current drawing color to white.
  gfx_color(0xFF,0xFF,0xFF);
	while(1) {
    //grab key events 
		if(gfx_event_waiting())
    {
      event = gfx_wait();
      if(event =='q') break;
    }
    
    //grab raw mouse data
    bytes = read(fd, data, sizeof(data));
    if(bytes > 0)
    {
      //left click down
      if(data[0] == 0x9)
        toggle = 1;
      //left click up
      if(data[0] == 0x8)
        toggle = 0;
    }
 
    if(toggle)
    {
      gfx_point(gfx_mousex()+0, gfx_mousey()+0);
      gfx_point(gfx_mousex()+0, gfx_mousey()+1);
      gfx_point(gfx_mousex()+0, gfx_mousey()+2);

      gfx_point(gfx_mousex()+1, gfx_mousey()+0);
      gfx_point(gfx_mousex()+1, gfx_mousey()+1);
      gfx_point(gfx_mousex()+1, gfx_mousey()+2);
      
      gfx_point(gfx_mousex()+2, gfx_mousey()+0);
      gfx_point(gfx_mousex()+2, gfx_mousey()+1);
      gfx_point(gfx_mousex()+2, gfx_mousey()+2);
    }
    gfx_flush();
	}

	return 0;
}
