#include "h8-3052-iodef.h"
#include "h8-3052-int.h"
#include "timer.h"
#include "lcd.h"
#include "lcd_util.h"

/* �����ߤμ��� 1000us */
#define INT1MS 1000

/* �����߽�����ɬ�פ��ѿ�������ѿ��ˤȤ� */
/* volatile �ϥץ�������ή��Ȥ�̵�ط����Ѳ������ѿ���ɬ���Ĥ��� */
volatile unsigned int sys_time;

/* �����ߥϥ�ɥ�Υץ��ȥ�������� */
void int_imia0(void);

int main(void)
{
  unsigned int sec_time; /* 1����� +1 ������ѿ������ */
  unsigned int prev_sec_time;

  char buffer[17];

  ROMEMU(); /* ROM���ߥ�졼������ON (�����߻��ѻ�ɬ��) */

  sys_time = 0; sec_time = 0; /* ���֤��Ǽ�����ѿ��ν���� */
  prev_sec_time = 0;

  timer_init();        /* �����ޤ�������˽�������� */
  timer_set(0,INT1MS); /* ������0��1ms�ֳ֤�ư������ */
  timer_start(0);      /* ������(����ͥ�0)��������  */

  lcd_init();
  lcd_printstr("SEC:0");

  ENINT();             /* CPU�����ߵ��� */

  while (1) {
    /* ������ư��ʳ��Ϥ���̵�¥롼�פ�¹Ԥ��Ƥ��� */

    /* ������ sec_time �ι��� ( +1 ����) ��� */
	if(sys_time>=1000)
	{
	  sec_time++;
	  sys_time = 0;
	}

    /* ������ sec_time ���ͤˤ�ä�LED����餻��ץ�������� */
    /*   1�ä��Ȥ� �֢��Т��֢��Т��� �ȸ�ߤ����Ǥ�����         */

	if(sec_time!=prev_sec_time)
	{
	  prev_sec_time = sec_time;
	  my_itoa(buffer,sec_time);
	  lcd_cursor(0,0);
	  lcd_printstr("SEC:");
	  lcd_printstr(buffer);
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