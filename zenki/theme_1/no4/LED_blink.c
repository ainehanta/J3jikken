#include "h8-3052-iodef.h"

/* $B%k!<%WDj?t(B */
#define _WAIT_LOOP 17400

void wait_ms(int);

int main(void)
{
  P9DDR = 0x30;  /* $B%]!<%H(B9$B$N=i4|2=(B(P94--P95$B$r=PNO$K@_Dj(B) */
  P9DR = 0x30;
  int i;

  while(1)
  {
    /* $B2C;;=g$G(BLED$BE@Et!J%]!<%H@\B35U$J$N$GJd@5!K(B */
    for(i=0x40;i>0x00;i-=0x10)
    {
      switch(i)
      {
      case 0x30 :
	P9DR = 0x10;
	break;
      case 0x20 :
	P9DR = 0x20;
	break;
      case 0x10 :
	P9DR = 0x00;
	break;
      case 0x40 :
	P9DR = 0x30;
	break;
      }
      wait_ms(1000);
    }
  }
}

void wait_ms(int ms)
{
  int i;
  for(;ms>0;ms--)
  for(i=0;i<_WAIT_LOOP;i++);
}
