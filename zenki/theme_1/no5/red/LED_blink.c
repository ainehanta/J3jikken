#include "h8-3052-iodef.h"

/* $B%k!<%WDj?t(B */
#define _WAIT_LOOP 17400

void wait_ms(int);

int main(void)
{
  P9DDR = 0x30;  /* $B%]!<%H(B9$B$N=i4|2=(B(P94--P95$B$r=PNO$K@_Dj(B) */
  int i = 15; /* $BE@LG<~4|$N@_Dj(B */

  while(1)
  {
    P9DR = 0x20;
    wait_ms(i);
    P9DR = 0x30;
    wait_ms(i);
  }
}

/* ms$BBT$D(B */
void wait_ms(int ms)
{
  int i;
  for(;ms>0;ms--)
  for(i=0;i<_WAIT_LOOP;i++);
}
