#include "h8-3052-iodef.h"

/* 仮ループ定数 */
#define _WAIT_LOOP 16000

void wait_ms(int);

int main(void)
{
  P9DDR = 0x30;  /* ポート9の初期化(P94--P95を出力に設定) */

  while(1)
  {
    P9DR = 0x10;
    wait_ms(1000);
    P9DR = 0x20;
    wait_ms(1000);
  }
}

/* ms待つ（未調整） */
void wait_ms(int ms)
{
  int i;
  for(;ms>0;ms--)
  for(i=0;i<_WAIT_LOOP;i++);
}
