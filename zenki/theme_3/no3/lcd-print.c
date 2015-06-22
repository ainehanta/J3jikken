#include "h8-3052-iodef.h"
#include "lcd.h"

#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3

void num_conv_string(char*,int);
void lcd_print_with_position(char*,int,int);
void my_itoa(char*,int);
void turn_str(char*);
void string_add(char*,char*);


int main(void)
{
  char buffer[33];

  lcd_init();

  calc(MUL,25,10);

  return 0;
}

void calc(int op,int a,int b)
{
  int answer;
  char result[33];
  char buffer[33];

  switch(op)
  {
  case ADD:
    answer = a + b;
    my_itoa(result,a);
    lcd_printstr(result);
    lcd_printch('+');
    my_itoa(result,b);
    lcd_printstr(result);
    lcd_printch('=');
    num_conv_string(result,a+b);
    lcd_printstr(result);
    break;
  case SUB:
    answer = a - b;
    my_itoa(result,a);
    lcd_printstr(result);
    lcd_printch('-');
    my_itoa(result,b);
    lcd_printstr(result);
    lcd_printch('=');
    num_conv_string(result,a-b);
    lcd_printstr(result);
    break;
  case MUL:
    answer = a * b;
    my_itoa(result,a);
    lcd_printstr(result);
    lcd_printch('*');
    my_itoa(result,b);
    lcd_printstr(result);
    lcd_printch('=');
    num_conv_string(result,a*b);
    lcd_printstr(result);
    break;
  case DIV:
    answer = a / b;
    my_itoa(result,a);
    lcd_printstr(result);
    lcd_printch('/');
    my_itoa(result,b);
    lcd_printstr(result);
    lcd_printch('=');
    num_conv_string(result,a/b);
    lcd_printstr(result);
    break;
  }
}

void string_add(char* a,char* b)
{
  int a_length;
  int i;

  for(a_length=0;a[a_length]!='\0';a_length++);
  a_length--;

  for(i=0;b[i]!='\0';i++)
  {
    a[a_length++] = b[i];
  }

  a[a_length++] = '\0';
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

void num_conv_string(char* string,int num)
{
  char tmp[33];

  int i;
  int length;
  int max_length;
  int start_count;
  int sp;

  my_itoa(tmp,num);

  for(length=0;tmp[length]!='\0';length++);
  max_length = length;
  if(length<=3)
  {
    for(i=0;tmp[i]!='\0';i++)
    {
      string[i] = tmp[i];
    }
    string[i] = '\0';
    return;
  }

  start_count = length%3-1;
  length+=length/3+1;

  for(sp=0,i=0;tmp[i]!='\0';i++)
  {
    string[sp++] = tmp[i];
    if((i==start_count||(i-start_count)%3==0)&&i!=max_length-1)
    {
      string[sp++] = ',';
    }
  }
  string[sp++] = '\0';
}

void lcd_print_with_position(char* string,int x,int y)
{
  int i;
  lcd_cursor(x,y);

  for(i=0;string[i]!='\0'&&i<32;i++)
  {
    lcd_printch(string[i]);
    x++;

    if(x>=16)
    {
      if(y==0)
      {
	x = 0;
	y = 1;
	lcd_cursor(x,y);
      }
      else
      {
	x = 0;
	y = 0;
	lcd_cursor(x,y);
      }
    }
  }
}
