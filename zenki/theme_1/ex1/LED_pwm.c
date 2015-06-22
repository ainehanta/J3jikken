#include "h8-3052-iodef.h"

#define _WAIT_LOOP_MS 17400
#define _WAIT_LOOP 174

void wait_ms(unsigned int);
void wait(unsigned int);

int main(void)
{
  P9DDR = 0x30;  /* $B%]!<%H(B9$B$N=i4|2=(B(P94--P95$B$r=PNO$K@_Dj(B) */

  int _duty = 128; /* $B%G%e!<%F%#!<Hf$N@_Dj(B 0($B%]!<%H>o;~(BHIGH),255($B%]!<%H(BLOW)*/

  P9DR = 0x00;

  /* PWM$B%k!<%W(B */
  while(1)
  {
    P9DR |= 0x20;
    wait(_duty);
    P9DR &= ~0x20;
    wait(255-_duty);
  }
}

/* us$B$N(B1/10$BICBT$D(B */
void wait(unsigned int s)
{
  int i;
  for(;s>0;s--)
    for(i=0;i<_WAIT_LOOP;i++);
}

/* ms$BBT$D(B */
void wait_ms(unsigned int ms)
{
  int i;
  for(;ms>0;ms--)
    for(i=0;i<_WAIT_LOOP_MS;i++);
}
