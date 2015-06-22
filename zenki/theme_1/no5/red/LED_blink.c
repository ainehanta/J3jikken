#include "h8-3052-iodef.h"

/* ループ定数 */
#define _WAIT_LOOP 17400

void wait_ms(int);

int main(void)
{
  P9DDR = 0x30;  /* ポート9の初期化(P94--P95を出力に設定) */
  int i = 15; /* 点滅周期の設定 */

  while(1)
  {
    P9DR = 0x20;
    wait_ms(i);
    P9DR = 0x30;
    wait_ms(i);
  }
}

/* ms待つ */
void wait_ms(int ms)
{
  int i;
  for(;ms>0;ms--)
  for(i=0;i<_WAIT_LOOP;i++);
}
