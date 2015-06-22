#include "h8-3052-iodef.h"
#include "h8-3052-int.h"
#include "timer.h"
#include "lcd.h"
#include "lcd_util.h"

/* 割り込みの周期 1000us */
#define INT1MS 1000

typedef struct
{
  unsigned char hour;
  unsigned char minute;
  unsigned char second;
} TIME;

/* 割り込み処理に必要な変数は大域変数にとる */
/* volatile はプログラムの流れとは無関係に変化する変数に必ずつける */
volatile unsigned int sys_time;

/* 割り込みハンドラのプロトタイプ宣言 */
void int_imia0(void);

void time_init(TIME* time);
void time_sec_to(TIME* time,unsigned int sec);
void time_add_sec(TIME* time,unsigned int sec);
void time_print(TIME time);

void print_zero(unsigned char value);

int main(void)
{
  unsigned int sec_time; /* 1秒毎に +1 される変数を確保 */
  unsigned int prev_sec_time;

  TIME now;

  ROMEMU(); /* ROMエミュレーションをON (割り込み使用時必須) */

  sys_time = 0; sec_time = 0; /* 時間を格納する変数の初期化 */
  prev_sec_time = 0;

  timer_init();        /* タイマを使用前に初期化する */
  timer_set(0,INT1MS); /* タイマ0を1ms間隔で動作設定 */
  timer_start(0);      /* タイマ(チャネル0)スタート  */

  lcd_init();

  time_init(&now);

  now.hour = 23;
  now.minute = 59;
  now.second = 45;

  lcd_cursor(0,0);
  time_print(now);

  ENINT();             /* CPU割り込み許可 */

  while (1) {
    /* 割り込み動作以外はこの無限ループを実行している */

    /* ここに sec_time の更新 ( +1 する) を書く */
	if(sys_time>=1000)
	{
	  sec_time++;
	  sys_time = 0;
	}

    /* ここに sec_time の値によってLEDを光らせるプログラムを書く */
    /*   1秒ごとに 赤→緑→赤→緑→… と交互に点滅させる         */

	if(sec_time!=prev_sec_time)
	{ 
	  lcd_cursor(0,0);
	  time_add_sec(&now,sec_time-prev_sec_time);
	  time_print(now);
	  prev_sec_time = sec_time;
	}

  }
  return 1; /* mainからの戻り値はエラーレベルを表す 0:正常終了 */
            /* 永久ループの外なので,戻ったら何かおかしい       */
}

#pragma interrupt
void int_imia0(void)
     /* タイマ0の割り込みハンドラ                                   */
     /*   タイマ0から割り込み要求がくるとこの関数が呼び出される     */
     /* 関数の名前はタイマ0の割り込みはこの名前と決まっている       */
     /* 関数の直前に割り込みハンドラ指定の #pragma interrupt が必要 */
{
  /* 割り込みハンドラの処理が軽くなるように配慮すること */
  /* 外でできる処理はここには書かない */

  /* sys_time の更新 ( +1 する) をここに書く */
  sys_time++;

  /* 再びタイマ割り込みを使用するために必要な操作      */
  /*   タイマ0の割り込みフラグをクリアしないといけない */
  timer_intflag_reset(0);

  ENINT();       /* CPUを割り込み許可状態に */
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

