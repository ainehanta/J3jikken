#include "h8-3052-iodef.h"
#include "key.h"
#include "lcd.h"

//$B%A%c%?%j%s%0<h$j=|$-(Bms
#define KEY_WAIT 20

void key_init(key_status*);
void key_scan(key_status*);
void key_scan_once(key_status*);
char key_press_check(key_status*); //$BF1;~2!$7HsBP1~(B

char key_press_check(key_status *status)
{
  int i;
  char ret = '\0';

  for(i=0;i<12;i++)
  {
    if((status->now>>i&0x01)!=0x00)
    {
      if((((status->now>>i)&0x01)==((status->prev>>i)&0x01)) &&
	 ((status->mask>>i)&0x01)==0x00)
      {
	switch(i)
	{
	case 0:
	  ret = '*';
	  break;
	case 1:
	  ret = '0';
	  break;
	case 2:
	  ret = '#';
	  break;
	case 3:
	  ret = '7';
	  break;
	case 4:
	  ret = '8';
	  break;
	case 5:
	  ret = '9';
	  break;
	case 6:
	  ret = '4';
	  break;
	case 7:
	  ret = '5';
	  break;
	case 8:
	  ret = '6';
	  break;
	case 9:
	  ret = '1';
	  break;
	case 10:
	  ret = '2';
	  break;
	case 11:
	  ret = '3';
	  break;
	default:
	  ret = '\0';
	  break;
	}

	status->mask |= 0x01<<i;
	break;
      }
    }
    else
    {
      status->mask &= ~(0x01<<i);
    }
  }

  return ret;
}

void key_scan(key_status *status)
{
  key_scan_once(status);
  wait1ms(KEY_WAIT);
  key_scan_once(status);
}

void key_scan_once(key_status *status)
{
  int cf;

  status->prev = status->now;
  status->now = 0;
  
  //key 1,2,3
  PADR = 0x07; // PA3 = L
  cf = P6DR;   // $B%G!<%?F~NO(B
  cf = ~cf;    // cf$B$NH?E>(B
  cf &= 0x07;  // P60,1,2$B$N$_8+$k(B
  status->now |= cf<<9;
  
  //key 4,5,6
  PADR = 0x0b;
  cf = P6DR;
  cf = ~cf;
  cf &= 0x07;
  status->now |= cf<<6;
  
  //key 7,8,9
  PADR = 0x0d;
  cf = P6DR;
  cf = ~cf;
  cf &= 0x07;
  status->now |= cf<<3;
      
  //key *,0,#
  PADR = 0x0e;
  cf = P6DR;
  cf = ~cf;
  cf &= 0x07;
  status->now |= cf;
}

void key_init(key_status *status)
{
  P6DDR &= ~0x07;  /* P60,1,2   $BF~NO(B */
  PADDR |= 0x0f;   /* PA0,1,2,3 $B=PNO(B */

  status->now = 0;
  status->prev = 0;
  status->mask = 0;
}
