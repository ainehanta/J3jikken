#include "h8-3052-iodef.h"
#include "lcd.h"

//Kisarazu•\Ž¦
int main(void) {
	lcd_init();

	lcd_cursor(0, 0);

	lcd_printch('K');
	lcd_printch('i');
	lcd_printch('s');
	lcd_printch('a');
	lcd_printch('r');
	lcd_printch('a');
	lcd_printch('z');
	lcd_printch('u');

	return 0;
}
