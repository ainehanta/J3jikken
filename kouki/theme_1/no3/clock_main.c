#include "h8-3052-iodef.h"
#include "h8-3052-int.h"
#include "timer.h"
#include "lcd.h"
#include "lcd_util.h"

/* �����ߤμ��� 1000us */
#define INT1MS 1000

typedef struct
{
  unsigned char hour;
  unsigned char minute;
  unsigned char second;
} TIME;

/* �����߽�����ɬ�פ��ѿ�������ѿ��ˤȤ� */
/* volatile �ϥץ�����ή��Ȥ�̵�ط����Ѳ������ѿ���ɬ���Ĥ��� */
volatile unsigned int sys_time;

/* �����ߥϥ�ɥ�Υץ�ȥ�������� */
void int_imia0(void);

void time_init(TIME* time);
void time_sec_to(TIME* time,unsigned int sec);
void time_add_sec(TIME* time,unsigned int sec);
void time_print(TIME time);

void print_zero(unsigned char value);

int main(void)
{
  unsigned int sec_time; /* 1����� +1 ������ѿ������ */
  unsigned int prev_sec_time;

  TIME now;

  ROMEMU(); /* ROM���ߥ�졼������ON (�����߻��ѻ�ɬ��) */

  sys_time = 0; sec_time = 0; /* ���֤��Ǽ�����ѿ��ν���� */
  prev_sec_time = 0;

  timer_init();        /* �����ޤ�������˽�������� */
  timer_set(0,INT1MS); /* ������0��1ms�ֳ֤�ư������ */
  timer_start(0);      /* ������(����ͥ�0)��������  */

  lcd_init();

  time_init(&now);

  now.hour = 23;
  now.minute = 59;
  now.second = 45;

  lcd_cursor(0,0);
  time_print(now);

  ENINT();             /* CPU�����ߵ��� */

  while (1) {
    /* ������ư��ʳ��Ϥ���̵�¥롼�פ�¹Ԥ��Ƥ��� */

    /* ������ sec_time �ι��� ( +1 ����) ��� */
	if(sys_time>=1000)
	{
	  sec_time++;
	  sys_time = 0;
	}

    /* ������ sec_time ���ͤˤ�ä�LED����餻��ץ������ */
    /*   1�ä��Ȥ� �֢��Т��֢��Т��� �ȸ�ߤ����Ǥ�����         */

	if(sec_time!=prev_sec_time)
	{ 
	  lcd_cursor(0,0);
	  time_add_sec(&now,sec_time-prev_sec_time);
	  time_print(now);
	  prev_sec_time = sec_time;
	}

  }
  return 1; /* main���������ͤϥ��顼��٥��ɽ�� 0:���ｪλ */
            /* �ʵץ롼�פγ��ʤΤ�,��ä��鲿����������       */
}

#pragma interrupt
void int_imia0(void)
     /* ������0�γ����ߥϥ�ɥ�                                   */
     /*   ������0����������׵᤬����Ȥ��δؿ����ƤӽФ����     */
     /* �ؿ���̾���ϥ�����0�γ����ߤϤ���̾���ȷ�ޤäƤ���       */
     /* �ؿ���ľ���˳����ߥϥ�ɥ����� #pragma interrupt ��ɬ�� */
{
  /* �����ߥϥ�ɥ�ν������ڤ��ʤ�褦����θ���뤳�� */
  /* ���ǤǤ�������Ϥ����ˤϽ񤫤ʤ� */

  /* sys_time �ι��� ( +1 ����) �򤳤��˽� */
  sys_time++;

  /* �Ƥӥ����޳����ߤ���Ѥ��뤿���ɬ�פ����      */
  /*   ������0�γ����ߥե饰�򥯥ꥢ���ʤ��Ȥ����ʤ� */
  timer_intflag_reset(0);

  ENINT();       /* CPU������ߵ��ľ��֤� */
}

void time_init(TIME* time)
{
  time->hour = 0;
  time->minute = 0;
  time->second = 0;
}

void time_sec_to(TIME* time,unsigned int sec)
{
  if(sec>=86400)
  {
	sec = 0;
  }

  unsigned int tmp;
  tmp = sec%60;
  time->second = tmp;
  sec -= tmp;
  tmp = sec/3600;
  time->hour = tmp;
  sec -= tmp*3600;
  time->minute = sec/60;
}

void time_add_sec(TIME* time,unsigned int sec)
{
  unsigned int tmp = 0;
  tmp = time->hour*3600;
  tmp += time->minute*60;
  tmp += time->second;
  tmp += sec;

  time_sec_to(time,tmp);
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

void print_zero(unsigned char value)
{
  if(value/10==0)
  {
    lcd_printch('0');
  }
}

