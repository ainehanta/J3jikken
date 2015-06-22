#include "h8-3052-iodef.h"
#include "lcd.h"

void lcd_scroll(char*);

int main(void) {
	lcd_init();

	lcd_scroll("Kisarazu-NCT");

	return 0;
}

//文字列を0行目で右方向にスクロール
void lcd_scroll(char* string) {
	int i;
	int x;

	for (;;)
		for (x = 0; x <= 16; x++) {
			//1回前のループのごみを削除
			lcd_cursor(x - 1, 0);
			lcd_printch(' ');
			//端っこに行くまで文字を表示
			for (i = 0; string[i] != '\0' && i + x < 16; i++)
				lcd_printch(string[i]);
			wait1ms(250);
		}
}
