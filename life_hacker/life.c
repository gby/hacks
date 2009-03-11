#include <stdio.h>
#include <stdlib.h>
#include "console.h"



unsigned char display[HMAX][VMAX];
unsigned char bitmap[2][HMAX][VMAX];

static const int hsize=HMAX, vsize=VMAX;

static int cbn, generation;

void pixel(int c, int i, int j)
{
  if (c<2 || c>3)
    {
      bitmap[1-cbn][i][j] = 0;
      display[i][j] = 0;
      return;
    }
  if (c==3)
    {
      bitmap[1-cbn][i][j] = 1;
      display[i][j] = 1;
      return;
    }
  bitmap[1-cbn][i][j] = bitmap[cbn][i][j];
}

void print_display(char * msg, int size) {

	  int i,j, len;

	  len = snprintf(msg, size, "Generation: %d\n", generation);
	  msg += len;
	  size -= len;
	  if(size<=0) goto the_end;
	  
	  for (i=1; i<=HMAX; i++) {
		  for(j=1; j<=VMAX; j++) {
			  len = snprintf(msg, size, "%c", (display[i][j] ? '*' : ' '));
			  msg += len;
			  size -= len;
			  if(size<=0) break;
	    }
		len = snprintf(msg, size, "|\n");
		msg += len;
		size -= len;
		if(size<=0) break;
	  }
	
the_end:
	   return;
}

void bitmap_init(void)
{
  int i,j;
  
  for (i=1; i<=hsize; i++)
    for(j=1; j<=vsize; j++)
      if((bitmap[cbn][i][j] = rand() %2))
    	  display[i][j] = 1;
      else
    	  display[i][j] = 0;

  for (i=0; i<=hsize+1; i++)
    {
      bitmap[cbn][i][0] = 0;
      bitmap[1-cbn][i][0] = 0;
      bitmap[cbn][i][vsize+1] = 0;
      bitmap[1-cbn][i][vsize+1] = 0;
    }
  for (j=0; j<=vsize+1; j++)
    {
      bitmap[cbn][0][j] = 0;
      bitmap[1-cbn][0][j] = 0;
      bitmap[cbn][hsize+1][j] = 0;
      bitmap[1-cbn][hsize+1][j] = 0;
    }
}


void bitmap_next(void)
{
  int i,j;
  int c;
  
  generation++;

  for (i=1; i<=hsize; i++)
    for(j=1; j<=vsize; j++)
      {
	c = bitmap[cbn][i-1][j-1] + bitmap[cbn][i-1][j] 
	  + bitmap[cbn][i-1][j+1] + bitmap[cbn][i][j-1] 
	  + bitmap[cbn][i][j+1]   + bitmap[cbn][i+1][j-1] 
	  + bitmap[cbn][i+1][j]   + bitmap[cbn][i+1][j+1];
	pixel(c,i,j);
      }
  cbn = 1-cbn;
}


void start_life(void)
{
	cbn = 0;
	generation = 0;
	bitmap_init();
  
  return;
}






