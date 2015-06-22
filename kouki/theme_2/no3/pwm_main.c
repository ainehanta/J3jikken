#include "h8-3052-iodef.h"
#include "h8-3052-int.h"

#define LED_RED   4
#define LED_GREEN 5

#define _BV(x) (1<<x)

int main(void)
{
  ROMEMU();
  
  ENINT();
  
  TSTR &= ~_BV(3);

  GRA3L = 0xFF;
  GRA3H = 0x7F;
  GRB3L = 0xFF;
  GRB3H = 0xFF;
  
  TMDR = _BV(3);
  TCR3 = _BV(6) | _BV(1) | _BV(0);
  TIOR3 = _BV(4) | _BV(1); 

  TSTR = _BV(3);//タイマースタート

  while (1)
  {
  }
  
  return 1;
}
