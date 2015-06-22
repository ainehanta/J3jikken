#include "h8-3052-iodef.h"

/* $B%k!<%WDj?t(Bms$BD4@0:Q$_(B */
#define _WAIT_LOOP 17400

void wait_ms(int);

int main(void)
{
  P9DDR = 0x30;  /* $B%]!<%H(B9$B$N=i4|2=(B(P94--P95$B$r=PNO$K@_Dj(B) */
  P9DR = 0x30;

  /* LED$B$r8r8_$KE@LG(B(10sec) */
  while(1)
  {
    P9DR = 0x20;
    wait_ms(10000);
    P9DR = 0x10;
    wait_ms(10000);
  }
}

/* ms$BBT$D(B */
void wait_ms(int ms)
{
  int i;
  for(;ms>0;ms--)
  for(i=0;i<_WAIT_LOOP;i++);
}
