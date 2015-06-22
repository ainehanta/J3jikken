#include "h8-3052-iodef.h"

#include "utils.h"
#include "lcd.h"

#define KEYROWNUM 4
#define KEYCOLNUM 3
#define KEYMINNUM  1
#define KEYMAXNUM 12
#define KEYNONE   -1
#define KEYOFF     0
#define KEYON      1
#define KEYTRANS   2

unsigned char keybuf[KEYROWNUM*KEYCOLNUM];

void key_init(void);
void key_sense(void);
int key_check(int keynum);

void key_init(void)
{
  int i;

  PADDR |= 0x0f;
  PADR  |= 0x0f;
  P6DDR &= ~0x07;
  /* キーバッファのクリア */
  for (i = 0; i < (KEYROWNUM*KEYCOLNUM); i++){
      keybuf[i] = 0x00;
  }
}

void key_sense(void)
{
  unsigned char tmp = 0;
  unsigned char row_count = 0;
  unsigned char key_count = 0;

  for(row_count=0;row_count<KEYROWNUM;row_count++)
  {
    PADR &= ~(_BV(3-row_count));
    tmp = 0x07&(~P6DR);

    keybuf[key_count] <<= 1;
    keybuf[key_count+1] <<= 1;
    keybuf[key_count+2] <<= 1;

    keybuf[key_count] |= tmp&0x01;
    keybuf[key_count+1] |= (tmp>>1)&0x01;
    keybuf[key_count+2] |= (tmp>>2)&0x01;

    key_count += 3;

/*
    for(col_count=0;col_count<KEYCOLNUM;col_count++)
    {
      keybuf[row_count+col_count] = keybuf[row_count+col_count] << 1;
      keybuf[row_count+col_count] |= ((tmp>>col_count)&0x01);
    }
*/

    PADR |= _BV(3-row_count);
  }
}

/* keynum : キー番号                */
/*          1 ~ 12指定              */
/*          1 ~ 9までそれぞれ数字に */
/*          10,11,12は*,0,#         */
/* ret    : KEYOFF KEYON KEYTRANS KEYNONE */

int key_check(int keynum)
{
  int ret;
  unsigned char tmp;

  if ((keynum < 1) || (keynum > KEYMAXNUM))
  {
    ret = KEYNONE;
  }
  else
  {
    keynum--;
    tmp = keybuf[keynum];
    if(tmp==0xff)
    {
      ret = KEYON;
    }
    else if(tmp==0x00)
    {
      ret = KEYOFF;
    }
    else
    {
      ret = KEYTRANS;
    }
  }

  return ret;
}
