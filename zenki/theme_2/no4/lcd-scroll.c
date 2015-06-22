#include "h8-3052-iodef.h"
#include "lcd.h"
#include <string.h>

#define LEFT  -1
#define RIGHT 1

//スクロール設定用の構造体
typedef struct {
	char string[33];
	int speed;
	　 //スピード
	int x;
	　　　 //開始座標
	int y;			//開始座標
	int direction;	//方向
} SCROLL_FACTORY;

void lcd_scroll(SCROLL_FACTORY);
void lcd_print_with_position(char*, int, int);
void nw_check(int*, int*);

int main(void) {
	//スクロールの初期設定
	SCROLL_FACTORY settings;
	strcpy(settings.string, "Kisarazu-KNCT");
	settings.speed = 250;
	settings.x = 0;
	settings.y = 1;
	settings.direction = RIGHT;

	lcd_init();

	lcd_scroll(settings);

	return 0;
}

//文字列をスクロール表示、構造体で方向および速度を設定する
void lcd_scroll(SCROLL_FACTORY settings) {
	int i;

	for (;;) {
		//画面を塗りつぶす（残像防止）
		lcd_cursor(0, 0);
		for (i = 0; i < 32; i++) {
			lcd_printch(' ');
			if (i == 15) {
				lcd_cursor(0, 1);
			}
		}

		lcd_print_with_position(settings.string, settings.x, settings.y);

		//方向設定に従って座標を更新
		settings.x += settings.direction;

		//改行の判断、座標の変更
		if (settings.x >= 16 || settings.x < 0) {
			if (settings.y == 0) {
				settings.y = 1;
			} else {
				settings.y = 0;
			}

			if (settings.direction == LEFT) {
				settings.x = 15;
			} else {
				settings.x = 0;
			}
		}
		wait1ms(settings.speed);
	}
}

//LCDの任意座標に文字列を表示、自動改行
void lcd_print_with_position(char* string, int x, int y) {
	int i;

	//一文字ずつLCDに表示
	for (i = 0; string[i] != '\0' && i < 32; i++) {
		lcd_cursor(x, y);
		lcd_printch(string[i]);
		x++;

		//改行が必要かチェック
		nw_check(&x, &y);
	}
}

//改行か必要かチェックし、必要な場合改行した座標値を代入
void nw_check(int *x, int *y) {
	if (*x > 15) {
		*x = 0;
		if (*y == 0) {
			*y = 1;
		} else {
			*y = 0;
		}
	} else if (*x < 0) {
		*x = 15;
		if (*y == 0) {
			*y = 1;
		} else {
			*y = 0;
		}
	}
}