#include "h8-3052-iodef.h"

#define _WAIT_LOOP_MS 17400
#define _WAIT_LOOP 174

void wait_ms(unsigned int);
void wait(unsigned int);

int main(void)
{
  P9DDR = 0x30;  /* ポート9の初期化(P94--P95を出力に設定) */

  int _duty = 128; /* デューティー比の設定 0(ポート常時HIGH),255(ポートLOW)*/

  P9DR = 0x00;

  /* PWMループ */
  while(1)
  {
    P9DR |= 0x20;
    wait(_duty);
    P9DR &= ~0x20;
    wait(255-_duty);
  }
}

/* usの1/10秒待つ */
void wait(unsigned int s)
{
  int i;
  for(;s>0;s--)
    for(i=0;i<_WAIT_LOOP;i++);
}

/* ms待つ */
void wait_ms(unsigned int ms)
{
  int i;
  for(;ms>0;ms--)
    for(i=0;i<_WAIT_LOOP_MS;i++);
}
