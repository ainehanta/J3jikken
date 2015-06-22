#include "h8-3052-iodef.h"
#include "lcd.h"

void lcd_print_with_position(char*, int, int);
void nw_check(int*, int*);

int main(void) {
	lcd_init();

	lcd_print_with_position("abcdefghijklmnopqrstuvwxyz0123456789", 11, 1);

	return 0;
}

//任意座標に文字列を表示、自動改行
void lcd_print_with_position(char* string, int x, int y) {
	int i;

	//一文字ずつ表示
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