#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "gfx.h"

const char *pDevice = "/dev/input/mice";

void swapGrain(int uA[100][100], int uB[100][100], int xA, int yA, int xB, int yB)
{
  int tmpA;
  int tmpB;
  tmpA = uA[xA][yA];
  tmpB = uA[xB][yB];
  uB[xA][yA] = tmpB;
  uB[xB][yB] = tmpA;
}

int main()
{
  int ysize = 300;
  int xsize = 300;
  int i,j;
  char event;
  int fd;
  int bytes;
  unsigned char data[3];
  int toggleLeft = 0;
  int toggleRight = 0;
  int brush = 1;


  int universe[100][100];
  int universePage[100][100];
  //initialize universe
  for(i = 0; i < 100; i++)
  {
    for(j = 0; j < 100; j++)
    {
      universe[i][j] = 0;
      universePage[i][j] = 0;
    }
  }
  
  //grab mouse
  fd = open(pDevice, O_RDONLY | O_NONBLOCK);
  if (fd == -1) {
    printf("ERROR Opening %s\n", pDevice);
    return -1;
  }
  

	// Open a new window for drawing.
	gfx_open(xsize,ysize,"SAND II: GRITTIER");

  while(1) {
    
    //grab key events 
		if(gfx_event_waiting())
    {
      event = gfx_wait();
      if(event =='+') brushSize++;
      if(event =='-') brushSize--;
      if(event =='q') break;
      if(event =='r')
      {
        for(i = 0; i < 100; i++)
        {
          for(j = 0; j < 100; j++)
          {
            universe[i][j] = 0;
            universePage[i][j] = 0;
          }
        }
      }
      if(event =='1')
        brush = 1;
      if(event =='2')
        brush = 2;
    }
    
    
    //grab raw mouse data
    bytes = read(fd, data, sizeof(data));
    if(bytes > 0)
    {
      //left click down
      if(data[0] == 0x9)
        toggleLeft = 1;
      //left click up
      if(data[0] == 0x8)
        toggleLeft = 0;
      //right click down
      if(data[0] == 0xa)
        toggleRight = 1;
      //right click up
      if(data[0] == 0x8)
        toggleRight = 0;
    }

    
    //interaction with universe
    if(toggleLeft)
    {
      universe[gfx_mousex()/3][gfx_mousey()/3] = brush;
    }
    else if (toggleRight)
    {
      universe[gfx_mousex()/3][gfx_mousey()/3] = 0;
    }
    

    //update universe
    for(i = 0; i < 100; i++)
    {
      for(j = 0; j < 100; j++)
      {
        //bottom row is a sink
        if(j == 99 || i == 0 || i == 99)
          universePage[i][j] = 2;
        else if (universe[i][j] == 1 && universe[i][j+1] == 0)
        {
          swapGrain(universe, universePage, i, j, i, j+1);
        }
        else if (universe[i][j] == 1 && universe[i+1][j+1] == 0)
        {
          swapGrain(universe, universePage, i, j, i+1, j+1);
        }
        else if (universe[i][j] == 1 && universe[i-1][j+1] == 0)
        {
          swapGrain(universe, universePage, i, j, i-1, j+1);
        }
        else if (universe[i][j] == 2)
        {
          universePage[i][j] = 2;
        }
      }
    }
    for(i = 0; i < 100; i++)
    {
      for(j = 0; j < 100; j++)
      {
        universe[i][j] = universePage[i][j];
      }
    }
    

    //draw universe
    for(i = 0; i < 100; i++)
    {
      for(j = 0; j < 100; j++)
      {
        if(universe[i][j] == 0)
          gfx_color(0x00,0x00,0x00);
        if(universe[i][j] == 1)
          gfx_color(0xff,0xff,0xff);
        if(universe[i][j] == 2)
          gfx_color(0xff,0xb0,0xb0);
        
        gfx_point(i*3+0, j*3+0);
        gfx_point(i*3+0, j*3+1);
        gfx_point(i*3+0, j*3+2);

        gfx_point(i*3+1, j*3+0);
        gfx_point(i*3+1, j*3+1);
        gfx_point(i*3+1, j*3+2);
      
        gfx_point(i*3+2, j*3+0);
        gfx_point(i*3+2, j*3+1);
        gfx_point(i*3+2, j*3+2);
      }
    }
    gfx_flush();
    usleep(50000);
	}

	return 0;
}
