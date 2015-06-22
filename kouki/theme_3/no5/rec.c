#include "h8-3052-iodef.h"
#include "h8-3052-int.h"
#include "lcd.h"
#include "ad.h"
#include "da.h"
#include "timer.h"
#include "key.h"

#define BUFSIZE       30
#define SAMPLINGTIME 100
#define SAMPLE         0
#define PLAY           1
#define INVERSE        2
#define NOSELECT      -1

#define _BV(x) (1<<x)

volatile unsigned char databuf[(unsigned long)BUFSIZE * 1024];
volatile long bufptr;
volatile int play_mode;

int            main(void);
unsigned char  menu(void);
void           sample_replay(int mode);
void           int_imia0(void);

key_status key;

int main(void)
{
  unsigned char key_data;

  ROMEMU();
  lcd_init();
  ad_init();
  da_init();
  timer_init();
  key_init(&key);
  
  timer_set(0,SAMPLINGTIME);

  while (1)
  {
    play_mode = NOSELECT;
    key_data = menu();

	switch(key_data)
	{
	case '*':
	  play_mode = SAMPLE;
	  break;
	case '#':
	  play_mode = PLAY;
	  break;
	case '5':
	  play_mode = INVERSE;
	  break;
	}

	if(play_mode != NOSELECT)
	{
	  sample_replay(play_mode);
	}
  }
  return 1;
}

unsigned char  menu(void)
{
  unsigned char key_data;

  lcd_cursor(0,0);
  lcd_printstr("R:[*]P:[#]I:[5] ");
  lcd_cursor(0,1);
  lcd_printstr(" Push *,# or 5 ");
  key_data = 0;

  key_scan(&key);

  key_data = key_press_check(&key);

  return key_data;
}

void sample_replay(int mode)
{
  if (mode == PLAY)
  {
	speaker_switch(SPEAKER);
    lcd_cursor(0,1);
    lcd_printstr(" Now playing... ");
  }
  if (mode == SAMPLE)
  {
	speaker_switch(MIC);
    lcd_cursor(0,1);
    lcd_printstr(" Now Sampling...");
  }
  bufptr = 0;
  if(mode == INVERSE)
  {
	speaker_switch(SPEAKER);
    lcd_cursor(0,1);
    lcd_printstr(" Now Inverse... ");
	bufptr = BUFSIZE * 1024 - 1;
  }
  
  timer_start(0);
  ENINT();
  if(mode == INVERSE)
  {
	while(bufptr>=0);
  }
  else
  {
	while (bufptr < ((unsigned long)BUFSIZE * 1024));
  }
  speaker_switch(MIC);
  timer_stop(0); 
}

#pragma interrupt
void int_imia0(void)
{
  if (play_mode == SAMPLE)
  {
	ad_start(0,0);
	while(!ADSTATUS());
	databuf[bufptr] = ADREAD();
  }
  if (play_mode == PLAY||play_mode == INVERSE)
  {
	if(bufptr>=0)
	  da_out(0,databuf[bufptr]);
  }

  if(play_mode == INVERSE)
  {
    bufptr--;
  }
  else
  {
	bufptr++;
  }
  timer_intflag_reset(0);
  ENINT();
}
