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
#define NOSELECT      -1

#define _BV(x) (1<<x)

volatile unsigned char databuf[(unsigned long)BUFSIZE * 1024];
volatile unsigned long bufptr;
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
	case '1':
	  play_mode = SAMPLE;
	  break;
	case '2':
	  play_mode = PLAY;
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
  lcd_printstr("Rec:[1] Play:[2]");
  lcd_cursor(0,1);
  lcd_printstr(" Push 1 or 2 key");
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
  timer_start(0);
  ENINT();
  while (bufptr < ((unsigned long)BUFSIZE * 1024));
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
  if (play_mode == PLAY)
  {
	da_out(0,databuf[bufptr]);
  }
  bufptr++;
  timer_intflag_reset(0);
  ENINT();
}
