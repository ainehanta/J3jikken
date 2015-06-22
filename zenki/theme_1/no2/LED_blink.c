#include "h8-3052-iodef.h"

/* $B2>%k!<%WDj?t(B */
#define _WAIT_LOOP 16000

void wait_ms(int);

int main(void)
{
  P9DDR = 0x30;  /* $B%]!<%H(B9$B$N=i4|2=(B(P94--P95$B$r=PNO$K@_Dj(B) */

  while(1)
  {
    P9DR = 0x10;
    wait_ms(1000);
    P9DR = 0x20;
    wait_ms(1000);
  }
}

/* ms$BBT$D!JL$D4@0!K(B */
void wait_ms(int ms)
{
  int i;
  for(;ms>0;ms--)
  for(i=0;i<_WAIT_LOOP;i++);
}
