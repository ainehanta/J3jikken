#include "h8-3052-iodef.h"

/* ループ定数ms調整済み */
#define _WAIT_LOOP 17400

void wait_ms(int);

int main(void)
{
  P9DDR = 0x30;  /* ポート9の初期化(P94--P95を出力に設定) */
  P9DR = 0x30;

  /* LEDを交互に点滅(10sec) */
  while(1)
  {
    P9DR = 0x20;
    wait_ms(10000);
    P9DR = 0x10;
    wait_ms(10000);
  }
}

/* ms待つ */
void wait_ms(int ms)
{
  int i;
  for(;ms>0;ms--)
  for(i=0;i<_WAIT_LOOP;i++);
}
