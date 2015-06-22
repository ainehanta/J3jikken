#include "h8-3052-iodef.h"
/* #include "h8-3052-int.h" */
#include "da.h"

#define TCONST40us 100*6
#define TCONST1ms 2500*10
#define TCONST1_10ms 2500
#define TCONST1_100ms 250

#define RA	66

void wait1_10ms(int ms);
void wait1_100ms(int ms);
void sound(int vol,int div);

int main(void)
{
	da_init();

	sound(10,RA);

	return 0;
}

void sound(int vol,int div)
{
	speaker_switch(SPEAKER);

	for(;;)
	{
		da_out(0,vol);
		wait1_100ms(div);
		da_out(0,0);
		wait1_100ms(div);
	}
}

void wait1_10ms(int ms)
     /* ms = 1 �ΤȤ�, ��1ms��������ʥ롼�פ��ԤĴؿ�            */
     /* LCD��ư����ݾڤ��뤿���ɬ�פʻ��֤���ݤ���Τ���Ū     */
     /* ���δ�, CPU�����ꤹ��Τ���դ�, ¾�����Ӥ˻��Ѥ��ʤ����� */
{
  int i,j;

  for (i = 0; i < ms; i++){
    for (j = 0; j < TCONST1_10ms; j++);
  }
}

void wait1_100ms(int ms)
     /* ms = 1 �ΤȤ�, ��1ms��������ʥ롼�פ��ԤĴؿ�            */
     /* LCD��ư����ݾڤ��뤿���ɬ�פʻ��֤���ݤ���Τ���Ū     */
     /* ���δ�, CPU�����ꤹ��Τ���դ�, ¾�����Ӥ˻��Ѥ��ʤ����� */
{
  int i,j;

  for (i = 0; i < ms; i++){
    for (j = 0; j < TCONST1_100ms; j++);
  }
}
