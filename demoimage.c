#include "types.h"
#include "stat.h"
#include "user.h"

void draw(int startx, int starty, int c) {
  int b = 63; //BG color
  int w = 63; //White
  int l = 59; //light blue

  int img[] = {
    b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,
    b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,
    b,b,b,b,b,b,0,0,0,0,b,b,b,b,b,b,
    b,b,b,b,b,0,c,c,c,c,0,b,b,b,b,b,
    b,b,b,b,0,c,c,c,0,0,0,0,b,b,b,b,
    b,b,b,0,0,c,c,0,l,l,w,w,0,b,b,b,
    b,b,0,c,0,c,c,0,l,l,l,l,0,b,b,b,
    b,b,0,c,0,c,c,c,0,0,0,0,b,b,b,b,
    b,b,0,c,0,c,c,c,c,c,c,0,b,b,b,b,
    b,b,0,0,0,c,c,c,c,c,c,0,b,b,b,b,
    b,b,b,b,0,c,c,c,c,c,c,0,b,b,b,b,
    b,b,b,b,0,c,c,c,c,c,c,0,b,b,b,b,
    b,b,b,b,0,c,c,0,0,0,c,0,b,b,b,b,
    b,b,b,b,0,c,c,0,b,0,c,0,b,b,b,b,
    b,b,b,b,0,0,0,0,b,0,0,0,b,b,b,b,
    b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b
  };

  for (int i = startx; i < startx + 16; i++) {
    for (int j = starty; j < starty + 16; j++) {
      plotpixel(i, j, img[(j - starty)*16 + (i - startx)]);
    }
  }
}

int
main(void)
{
  modeswitch(1);

  for (int x = 0; x < 20; x++) {
    for (int y = 0; y < 12; y++) {
      draw(x*16, y*16, uptime()%64);
    }
  }

  for (int x = 0; x < 320; x++) {
    for (int y = 192; y < 200; y++) {
      plotpixel(x, y, 0);
    }
  }
  exit();
}
