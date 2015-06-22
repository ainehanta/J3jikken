#include "h8-3052-iodef.h"

/* ループ定数 */
#define _WAIT_LOOP 17400

void wait_ms(int);

int main(void)
{
  P9DDR = 0x30;  /* ポート9の初期化(P94--P95を出力に設定) */
  P9DR = 0x30;
  int i;

  while(1)
  {
    /* 加算順でLED点灯（ポート接続逆なので補正） */
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
