#include "h8-3052-iodef.h"
#include "h8-3052-int.h"
#include "lcd.h"
#include "da.h"
#include "timer.h"
#include "key.h"

#define _BV(x) (1<<x)

#define DO_L 261
#define RE 293
#define MI 329
#define FA 349
#define SO 391
#define RA 440
#define SI 493
#define DO_H 523

int            main(void);
unsigned char  menu(void);
void           int_imia0(void);
void           tone_init(void);
void           note_on(int,int);
void           note_off(void);

volatile unsigned long note_t;
volatile unsigned int note_vol;
volatile unsigned char note_flag;
volatile unsigned char note_flag_on;

int main(void)
{
  unsigned char cf,key_data;
  int vol = 127;

  ROMEMU();
  da_init();
  timer_init();

  P6DDR &= ~0x07;  /* P60,1,2   入力 */
  PADDR |= 0x0f;   /* PA0,1,2,3 出力 */

  tone_init();
  
  while (1)
  {
	key_data = 0;

    //key 1,2,3
    PADR = 0x07; // PA3 = L
    cf = P6DR;   // データ入力
    cf = ~cf;    // cfの反転
    cf &= 0x07;  // P60,1,2のみ見る
    switch(cf) {
    case 1 : key_data = '1'; break;
    case 2 : key_data = '2'; break;
    case 4 : key_data = '3'; break;
    }  
      
    //key 4,5,6
    PADR = 0x0b;
    cf = P6DR;
    cf = ~cf;
    cf &= 0x07;
    switch(cf) {
    case 1 : key_data = '4'; break;
    case 2 : key_data = '5'; break;
    case 4 : key_data = '6'; break;
    }  
      
    //key 7,8,9
    PADR = 0x0d; /* This is a mistake code. */
    cf = P6DR;
    cf = ~cf;
    cf &= 0x07;
    switch(cf) {
    case 1 : key_data = '7'; break;
    case 2 : key_data = '8'; break;
    case 4 : key_data = '9'; break;
    }  
      
    //key *,0,#
    PADR = 0x0e;
    cf = P6DR;
    cf = ~cf;
    cf &= 0x07;
    switch(cf) {
    case 1 : key_data = '*'; break;
    case 2 : key_data = '0'; break;
    case 4 : key_data = '#'; break;
    } 

	switch(key_data)
	{
    case '1':
      note_on(DO_L,vol);
      break;
    case '2':
      note_on(RE,vol);
      break;
    case '3':
      note_on(MI,vol);
      break;
    case '4':
      note_on(FA,vol);
      break;
    case '5':
      note_on(SO,vol);
      break;
    case '6':
      note_on(RA,vol);
      break;
    case '7':
      note_on(SI,vol);
      break;
    case '8':
      note_on(DO_H,vol);
      break;
    default:
	  note_off();
	  break;
	}
  }
  return 1;
}

void tone_init(void)
{
  note_t = 0;
  note_vol = 0;
  note_flag = 0;
  note_flag_on = 0;
  da_stop(0);
}

void note_on(int hz,int vol)
{
  unsigned int tmp;
  tmp = 1000*1000 / hz / 2;
  if(tmp==note_t&&note_flag_on==1)
  {
	note_vol = vol;
	return;
  }

  timer_stop(0);

  //各種設定をすること
  note_t = tmp;
  note_flag = 0;
  note_vol = vol;
  timer_set(0,note_t);

  speaker_switch(SPEAKER);
  note_flag_on = 1;
  timer_start(0);
  ENINT();
}

void note_off(void)
{
  timer_stop(0);
  speaker_switch(MIC);
  note_flag_on = 0;
}

#pragma interrupt
void int_imia0(void)
{
  if(note_flag==1)
  {
	da_out(0,note_vol);
	note_flag = 0;
  }
  else
  {
	da_out(0,0);
	note_flag = 1;
  }

  timer_intflag_reset(0);
  ENINT();
}
