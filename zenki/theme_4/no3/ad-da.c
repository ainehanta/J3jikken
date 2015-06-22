#include "h8-3052-iodef.h"
/* #include "h8-3052-int.h" */
#include "lcd.h"
#include "ad.h"
#include "da.h"
#include "key.h"

#define BUFSIZE       30 /* �Хåե����礭��(kB) */
#define SAMPLE         0 /* ư������(Ͽ��)       */
#define PLAY           1 /* ư������(����)       */
#define NOSELECT      -1 /* ư��������(������)   */
#define TIME      120000 /* Ͽ��/��������        */

volatile unsigned char databuf[(unsigned long)BUFSIZE * 1024];
volatile long bufptr;
volatile int play_mode;

unsigned char  menu(key_status* key);
void           sample_replay(int mode);

int main(void)
{
	/* �������Ͼ����������뤿����ѿ���������� */
	unsigned char key_data;

	key_status key;

	key_init(&key);
	lcd_init();    /* LCD �ν����            */
	ad_init();     /* A/D �Ѵ���ν����      */
	da_init();     /* D/A �Ѵ���ν����      */

	/* �������˥������ϼ����Τ���Υݡ��Ȥν�����򵭽Ҥ��� */
	/* P60,1,2   ���� */
	/* PA0,1,2,3 ���� */

	while (1) {
		play_mode = NOSELECT;
		key_data = menu(&key);                    /* ��˥塼������   */
		if(key_data == '*')
		{
			play_mode = SAMPLE;
		}
		else if(key_data == '5')
		{
			play_mode = PLAY;
		}

		sample_replay(play_mode);

	}
	return 1;
}

unsigned char  menu(key_status* key)
	/* LCD �˥�˥塼��񤤤�ư������򤹤뤿��δؿ� */
	/* ����ͤ����ϥ�������                           */
{
	unsigned char key_data = '\0';
	/* �������ϼ����Τ�����ѿ���������� */

	lcd_cursor(0,0);                   /* LCD �˥�å�����ɽ�� */
	lcd_printstr("Rec:[*] Play:[5]");
	lcd_cursor(0,1);
	lcd_printstr(" Push * or 5 key");

	while (key_data == '\0')
	{
		key_scan(key);
		key_data = key_press_check(key);
		if(key_data=='*'||key_data=='5')
		{
			break;
		}
		else
		{
			key_data = '\0';
		}
	}

	/* ���Ϥ��줿�����ξ�����֤� */
	return key_data;
}

void sample_replay(int mode)
	/* Ͽ���ޤ��Ϻ�����Ԥ��ؿ� */
	/*   mode: PLAY, SAMPLE     */
{
	bufptr = 0;                             /* �Хåե��ݥ��󥿤�����  */
	if (mode == PLAY){                                /* �����⡼�ɤν��� */
		/* �����˥��ԡ����򥹥ԡ����Ȥ��ƻ��Ѥ���̿�� */
		speaker_switch(SPEAKER);
		lcd_cursor(0,1);
		lcd_printstr(" Now Inverse... ");

		/* �������Ͻ������Ƥ򵭽Ҥ��� */
		bufptr = TIME;
		while(bufptr >= 0){                /*   �ǡ������Ϥ򷫤��֤�    */
			/* ���ġ����˥ǡ��������    */
			/* ���Хåե��ݥ��󥿤� +1   */
			da_out(0,databuf[bufptr--]);
		}

		da_stop(0);
	}
	if (mode == SAMPLE){                              /* Ͽ���⡼�ɤν��� */
		/* �����˥��ԡ�����ޥ����Ȥ��ƻ��Ѥ���̿�� */
		speaker_switch(MIC);
		lcd_cursor(0,1);
		lcd_printstr(" Now Sampling...");

		/* ��������������Ƥ򵭽Ҥ��� */
		while(bufptr <= TIME){                 /*   �ǡ����ɹ��򷫤��֤�     */
			/* ���������Ѵ���������       */
			/* ���������Ѵ���λ�Ԥ� ��5us */
			/* ���Ѵ��ǡ������Ǽ�� ������*/
			/* ���Хåե��ݥ��󥿤� +1    */

			ad_start(0,0);
			while(ADSTATUS()==0);
			databuf[bufptr++] = ADREAD();
		}
	}
	speaker_switch(MIC);      /* ���ԡ���������   */
}
