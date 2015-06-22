#include "h8-3052-iodef.h"
#include "h8-3052-int.h"
#include "timer.h"

#define LED_RED   4
#define LED_GREEN 5

#define PWM_T 100
#define _BV(x) (1<<x)
#define _DUTY(x) (unsigned char)(x/100.0*256.0)

volatile unsigned char sys_time;
volatile unsigned char pwm_duty;

void int_imia0(void);

int main(void)
{
  ROMEMU();

  P9DDR = 0xFF;
  P9DR |= _BV(LED_RED)|_BV(LED_GREEN);

  sys_time = 0;
  pwm_duty = 2;

  timer_init();
  timer_set(0,PWM_T);
  timer_start(0);

  ENINT();

  while (1) {
	
  }
  return 1;
}

#pragma interrupt
void int_imia0(void)
{
  if(sys_time>9)
  {
    sys_time = 0;
  }
  if(pwm_duty==0)
  {
	P9DR &= ~_BV(LED_RED);
  }
  else if(pwm_duty==10)
  {
    P9DR |= _BV(LED_RED);
  }
  else if(sys_time>=pwm_duty)
  {
    P9DR &= ~_BV(LED_RED);    
  }
  else
  {
    P9DR |= _BV(LED_RED);
  }

  sys_time++;
  timer_intflag_reset(0);

  ENINT();
}
