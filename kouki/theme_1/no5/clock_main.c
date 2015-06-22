#include "h8-3052-iodef.h"
#include "h8-3052-int.h"
#include "timer.h"
#include "lcd.h"
#include "lcd_util.h"
#include "key.h"

#define INT1MS 1000

typedef struct
{
  char hour;
  char minute;
  char second;
  int msec;
} TIME;

volatile unsigned int sys_time;
volatile unsigned long total_sys_time;

void int_imia0(void);

void time_init(TIME* time);
void time_print(TIME time);

void time_msec_to(TIME* time,unsigned int msec);
unsigned long time_to_msec(TIME time);
void stop_watch_print(TIME time);

void print_zero(unsigned int value);

int main(void)
{
  unsigned int sec_time;
  unsigned long prev_total_sys_time;
  unsigned long count_sys_time;

  TIME now;
  key_status key;

  char flag = 0;

  ROMEMU();

  sys_time = 0;
  total_sys_time = 0;
  prev_total_sys_time = 0;
  count_sys_time = 0;
  sec_time = 0;

  timer_init();
  timer_set(0,INT1MS);
  timer_start(0);

  lcd_init();

  time_init(&now);

  key_init(&key);

  lcd_cursor(0,0);
  time_print(now);

  ENINT();

  while (1) {

	if(sys_time>=1000)
	{
	  sec_time++;
	  sys_time = 0;
	}

	key_scan(&key);

	switch(key_press_check(&key))
	{
	case '0':
	  if(flag==1)
	  {
	    count_sys_time = time_to_msec(now);
		prev_total_sys_time = total_sys_time;
		flag = 0;
	  }
	  else
	  {
	    flag = 1;
	  }
	  break;
	case '6':
	  if(flag==1)
	  {
		if(now.second<60)
		  now.second += 1;
		count_sys_time=time_to_msec(now);
	  }
	  break;
	case '9':
	  if(flag==1)
	  {
		if(now.second>0)
		  now.second -= 1;
		count_sys_time=time_to_msec(now);
	  }
	  break;
	case '5':
	  if(flag==1)
	  {
		if(now.minute<60)
		  now.minute += 1;
		count_sys_time=time_to_msec(now);
	  }
	  break;
	case '8':
	  if(flag==1)
	  {
		if(now.minute>0)
		  now.minute -= 1;
		count_sys_time=time_to_msec(now);
	  }
	  break;
	case '4':
	  if(flag==1)
	  {
		if(now.hour<23)
		now.hour += 1;
		count_sys_time=time_to_msec(now);
	  }
	  break;
	case '7':
	  if(flag==1)
	  {
		if(now.hour>0)
		  now.hour -= 1;
		count_sys_time=time_to_msec(now);
	  }
	  break;
	}

	if(flag==0)
	{
	  count_sys_time += total_sys_time - prev_total_sys_time;
	  prev_total_sys_time = total_sys_time;
	}

	lcd_cursor(0,0);
	time_msec_to(&now,count_sys_time);
	time_print(now);
  }
  return 1;
}

#pragma interrupt
void int_imia0(void)
{
  sys_time++;
  total_sys_time++;

  timer_intflag_reset(0);

  ENINT();
}

void time_init(TIME* time)
{
  time->hour = 0;
  time->minute = 0;
  time->second = 0;
  time->msec = 0;
}

void time_print(TIME time)
{
  char buf[17];

  my_itoa(buf,time.hour);
  print_zero(time.hour);
  lcd_printstr(buf);
  lcd_printch(':');

  my_itoa(buf,time.minute);
  print_zero(time.minute);
  lcd_printstr(buf);
  lcd_printch(':');

  my_itoa(buf,time.second);
  print_zero(time.second);
  lcd_printstr(buf);
}

void stop_watch_print(TIME time)
{
  char buf[17];

  my_itoa(buf,time.hour);
  print_zero(time.hour);
  lcd_printstr(buf);
  lcd_printch(':');

  my_itoa(buf,time.minute);
  print_zero(time.minute);
  lcd_printstr(buf);
  lcd_printch(':');

  my_itoa(buf,time.second);
  print_zero(time.second);
  lcd_printstr(buf);

  my_itoa(buf,time.msec/10);
  lcd_printch('.');
  print_zero(time.msec/10);
  lcd_printstr(buf);
}

void time_msec_to(TIME* time,unsigned int msec)
{
  if(msec>=86400000)
  {
	msec = 0;
  }

  unsigned int tmp;
  tmp = msec%1000;
  time->msec = tmp;
  msec /= 1000;
  tmp = msec%60;
  time->second = tmp;
  msec -= tmp;
  tmp = msec/3600;
  time->hour = tmp;
  msec -= tmp*3600;
  time->minute = msec/60;
}

unsigned long time_to_msec(TIME time)
{
  unsigned long msec = 0;
  msec = time.msec;
  msec += time.second*1000;
  msec += time.minute*1000*60;
  msec += time.hour*1000*3600;

  return msec;
}

void print_zero(unsigned int value)
{
  if(value/10==0)
  {
    lcd_printch('0');
  }
}
