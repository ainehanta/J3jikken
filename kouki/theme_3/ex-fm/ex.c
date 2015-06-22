#include "h8-3052-iodef.h"
#include "h8-3052-int.h"
#include "lcd.h"
#include "da.h"
#include "timer.h"
#include "key.h"

#define _BV(x) (1<<x)
#define _IS(x,pos) ((x>>pos)&0x01)

#define DO_L 261
#define RE 293
#define MI 329
#define FA 349
#define SO 391
#define RA 440
#define SI 493
#define DO_H 523

#define MODE         0
#define NOTE_STATUS  1
#define OUT_STATUS   2

#define MODE_FM     1
#define MODE_SQ     0
#define NOTE_ON     1
#define NOTE_OFF    0
#define OUT_HIGH    1
#define OUT_LOW     0

int            main(void);
unsigned char  menu(void);
void           int_imia0(void);
void           tone_init(void);
void           note_on(int,int);
void           note_on_fm(int);
void           note_off(void);

volatile unsigned long note_t;
volatile unsigned int note_vol;
volatile unsigned char note_flag;
volatile unsigned int sin_pos;

volatile unsigned char sin_data[360] = {127,129,131,133,135,138,140,142,144,146,149,151,153,155,157,159,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,196,198,199,201,203,205,206,208,210,211,213,215,216,218,219,221,222,224,225,227,228,229,231,232,233,234,235,236,238,239,240,241,242,243,243,244,245,246,247,247,248,249,249,250,250,251,251,252,252,252,253,253,253,253,253,253,253,254,253,253,253,253,253,253,253,252,252,252,251,251,250,250,249,249,248,247,247,246,245,244,243,243,242,241,240,239,238,236,235,234,233,232,231,229,228,227,225,224,222,221,219,218,216,215,213,211,210,208,206,205,203,201,199,198,196,194,192,190,188,186,184,182,180,178,176,174,172,170,168,166,164,162,159,157,155,153,151,149,146,144,142,140,138,135,133,131,129,127,125,123,121,119,116,114,112,110,108,105,103,101,99,97,95,92,90,88,86,84,82,80,78,76,74,72,70,68,66,64,62,60,58,56,55,53,51,49,48,46,44,43,41,39,38,36,35,33,32,30,29,27,26,25,23,22,21,20,19,18,16,15,14,13,12,11,11,10,9,8,7,7,6,5,5,4,4,3,3,2,2,2,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,2,2,2,3,3,4,4,5,5,6,7,7,8,9,10,11,11,12,13,14,15,16,18,19,20,21,22,23,25,26,27,29,30,32,33,35,36,38,39,41,43,44,46,48,49,51,53,55,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,95,97,99,101,103,105,108,110,112,114,116,119,121,123,125};

int main(void)
{
  unsigned char cf,key_data;
  int vol = 127;
  int hz = 0;

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
	  hz = DO_L;
      break;
    case '2':
	  hz = RE;
      break;
    case '3':
	  hz = MI;
      break;
    case '4':
	  hz = FA;
      break;
    case '5':
	  hz = SO;
      break;
    case '6':
	  hz = RA;
      break;
    case '7':
      hz = SI;
      break;
    case '8':
      hz = DO_H;
      break;
    default:
	  hz = 0;
	  break;
	}

	if(hz!=0)
	{
	  //note_on(hz,vol);
	  note_on_fm(hz);
    }
	else
	{
	  note_off();
	}
  }
  return 1;
}

void tone_init(void)
{
  note_t = 0;
  note_vol = 0;
  note_flag = 0;
  sin_pos = 0;
  da_stop(0);
}

void note_on_fm(int hz)
{
  unsigned int tmp;
  tmp = 1000*1000/hz/360;
  if(tmp==note_t&&_IS(note_flag,MODE)==MODE_FM&&_IS(note_flag,NOTE_STATUS)==NOTE_ON)
  {
	return;
  }

  timer_stop(0);

  note_t = tmp;
  timer_set(0,note_t);
  speaker_switch(SPEAKER);
  note_flag = 0;
  note_flag |= _BV(MODE);
  note_flag |= _BV(NOTE_STATUS);
  timer_start(0);
  ENINT();
}

void note_on(int hz,int vol)
{
  unsigned int tmp;
  tmp = 1000*1000 / hz / 2;
  if(tmp==note_t&&
	 _IS(note_flag,MODE)==MODE_SQ&&
	 _IS(note_flag,NOTE_STATUS)==NOTE_ON)
  {
	note_vol = vol;
	return;
  }

  timer_stop(0);

  //各種設定をすること
  note_t = tmp;
  note_flag = 0;
  note_flag &= ~(_BV(MODE));
  note_vol = vol;
  timer_set(0,note_t);
  sin_pos = 0;
  speaker_switch(SPEAKER);
  note_flag |= _BV(NOTE_STATUS);
  timer_start(0);
  ENINT();
}

void note_off(void)
{
  timer_stop(0);
  speaker_switch(MIC);
  note_flag = 0;
  note_t = 0;
}

#pragma interrupt
void int_imia0(void)
{
  if(_IS(note_flag,MODE)==MODE_SQ)
  {
	if(_IS(note_flag,OUT_STATUS)==OUT_LOW)
    {
  	  da_out(0,note_vol);
	  note_flag |= _BV(OUT_STATUS);
    }
    else
    {
 	  da_out(0,0);
	  note_flag &= ~(_BV(OUT_STATUS));
    }
  }
  else
  {
	if(sin_pos>=360)
	{
	  sin_pos = 0;
	}
	da_out(0,sin_data[sin_pos]);
    sin_pos++;
  }

  timer_intflag_reset(0);
  ENINT();
}
