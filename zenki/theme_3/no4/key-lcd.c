#include "h8-3052-iodef.h"
#include "lcd.h"

//チャタリング取り除きms
#define KEY_WAIT 20

typedef struct
{
  unsigned int prev;
  unsigned int now;
  unsigned int mask;
} key_status;

void nw_check(int*,int*);
void key_init(key_status*);
void key_scan(key_status*);
void key_scan_once(key_status*);
char key_press_check(key_status*); //同時押し非対応

void lcd_putch(unsigned char ch, unsigned char rs);

void my_itoa(char*,int);
void turn_str(char*);

int main(void)
{
  char key_data = '\0';
  char print_data = '\0';

  int x = 0;
  int y = 0;

  key_status key;

  lcd_init();
  lcd_putch(0x0d,0);
  key_init(&key);

  print_data = 'a';
  while(1)
  {
    key_scan(&key);
    key_data = key_press_check(&key);
    lcd_cursor(x,y);
    lcd_printch(print_data);
    if(key_data!='\0')
    {
      switch(key_data)
      {
      case '7':
	print_data--;
	break;
      case '9':
	print_data++;
	break;
      case '0':
	lcd_cursor(x,y);
	lcd_printch(' ');
	x++;
	nw_check(&x,&y);
	break;
      case '*':
	lcd_cursor(x,y);
	lcd_printch(' ');
	x--;
	nw_check(&x,&y);
	break;
      case '#':
	x++;
	nw_check(&x,&y);
	break;
      }
    }

    if(print_data>'z')
    {
      print_data = 'a';
    }
    else if(print_data<'a')
    {
      print_data = 'z';
    }

    lcd_cursor(x,y);
  }
}

void my_itoa(char* string,int num)
{
  int i;
  int sp;
  int max_dig;

  for(i=num,max_dig=0;i>0;i/=10,max_dig++);

  for(sp=0,i=0;i<max_dig;i++,sp++)
  {
    string[sp] = num%10+'0';
    num /= 10;
  }
  string[sp++] = '\0';
  turn_str(string);
}

void turn_str(char* string)
{
  int length;
  int sp;
  char tmp;

  for(length=0;string[length]!='\0';length++);
  length--;
  for(sp=0;sp<=length/2;sp++)
  {
    tmp = string[sp];
    string[sp] = string[length-sp];
    string[length-sp] = tmp;
  }
}

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
  cf = P6DR;   // データ入力
  cf = ~cf;    // cfの反転
  cf &= 0x07;  // P60,1,2のみ見る
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
  P6DDR &= ~0x07;  /* P60,1,2   入力 */
  PADDR |= 0x0f;   /* PA0,1,2,3 出力 */

  status->now = 0;
  status->prev = 0;
  status->mask = 0;
}

void nw_check(int *x,int *y)
{
  if(*x>15)
  {
    *x = 0;
    if(*y==0)
    {
      *y = 1;
    }
    else
    {
      *y = 0;
    }
  }
  else if(*x<0)
  {
    *x = 15;
    if(*y==0)
    {
      *y = 1;
    }
    else
    {
      *y = 0;
    }
  }
}
