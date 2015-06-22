#include "h8-3052-iodef.h"
/* #include "h8-3052-int.h" */
#include "lcd.h"
#include "ad.h"
#include "da.h"
#include "key.h"

#define BUFSIZE       30 /* バッファの大きさ(kB) */
#define SAMPLE         0 /* 動作選択(録音)       */
#define PLAY           1 /* 動作選択(再生)       */
#define INVERSE		   2
#define NOSELECT      -1 /* 動作選択値(末選択)   */
#define TIME      120000 /* 録音/再生時間        */

volatile unsigned char databuf[(unsigned long)BUFSIZE * 1024];
volatile long bufptr;
volatile int play_mode;

unsigned char  menu(key_status* key);
void           sample_replay(int mode);

int main(void)
{
	/* キー入力情報を取得するための変数を宣言する */
	unsigned char key_data;

	key_status key;

	key_init(&key);
	lcd_init();    /* LCD の初期化            */
	ad_init();     /* A/D 変換器の初期化      */
	da_init();     /* D/A 変換器の初期化      */

	/* ◎ここにキー入力取得のためのポートの初期化を記述する */
	/* P60,1,2   入力 */
	/* PA0,1,2,3 出力 */

	while (1) {
		play_mode = NOSELECT;
		key_data = menu(&key);                    /* メニューを選ぶ   */
		if(key_data == '*')
		{
			play_mode = SAMPLE;
		}
		else if(key_data == '#')
		{
			play_mode = PLAY;
		}
		else if(key_data == '5')
		{
			play_mode = INVERSE;
		}

		sample_replay(play_mode);

	}
	return 1;
}

unsigned char  menu(key_status* key)
	/* LCD にメニューを書いて動作を選択するための関数 */
	/* 戻り値は入力キー情報                           */
{
	unsigned char key_data = '\0';
	/* キー入力取得のための変数を宣言する */

	lcd_cursor(0,0);                   /* LCD にメッセージ表示 */
	lcd_printstr("R:[*]P:[#]I:[5]");
	lcd_cursor(0,1);
	lcd_printstr("Push * , # or 5");

	while (key_data == '\0')
	{
		key_scan(key);
		key_data = key_press_check(key);
		if(key_data=='*'||key_data=='5'||key_data=='#')
		{
			break;
		}
		else
		{
			key_data = '\0';
		}
	}

	/* 入力されたキーの情報を返す */
	return key_data;
}

void sample_replay(int mode)
	/* 録音または再生を行う関数 */
	/*   mode: PLAY, SAMPLE     */
{
	bufptr = 0;                             /* バッファポインタを初期化  */
	if (mode == INVERSE){                                /* 再生モードの処理 */
		/* ここにスピーカをスピーカとして使用する命令 */
		speaker_switch(SPEAKER);
		lcd_cursor(0,1);
		lcd_printstr(" Now Inverse... ");

		/* 音声出力処理内容を記述する */
		bufptr = TIME;
		while(bufptr >= 0){                /*   データ出力を繰り返す    */
			/* ◎Ｄ／Ａにデータを出力    */
			/* ◎バッファポインタを +1   */
			da_out(0,databuf[bufptr--]);
		}

		da_stop(0);
	}
	if(mode == PLAY)
	{
		speaker_switch(SPEAKER);
		lcd_cursor(0,1);
		lcd_printstr(" Now Playing... ");

		while(bufptr <= TIME)
		{
			da_out(0,databuf[bufptr++]);
		}
	}
	if (mode == SAMPLE){                              /* 録音モードの処理 */
		/* ここにスピーカをマイクとして使用する命令 */
		speaker_switch(MIC);
		lcd_cursor(0,1);
		lcd_printstr(" Now Sampling...");

		/* 音声取込処理内容を記述する */
		while(bufptr <= TIME){                 /*   データ読込を繰り返す     */
			/* ◎Ａ／Ｄ変換スタート       */
			/* ◎Ａ／Ｄ変換終了待ち 約5us */
			/* ◎変換データを格納　 　　　*/
			/* ◎バッファポインタを +1    */

			ad_start(0,0);
			while(ADSTATUS()==0);
			databuf[bufptr++] = ADREAD();
		}
	}
	speaker_switch(MIC);      /* スピーカーオフ   */
}

