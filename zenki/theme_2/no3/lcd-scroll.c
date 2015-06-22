#include "h8-3052-iodef.h"
#include "lcd.h"

void lcd_scroll(char*);

int main(void) {
	lcd_init();

	lcd_scroll("Kisarazu-NCT");

	return 0;
}

//�������0�s�ڂŉE�����ɃX�N���[��
void lcd_scroll(char* string) {
	int i;
	int x;

	for (;;)
		for (x = 0; x <= 16; x++) {
			//1��O�̃��[�v�̂��݂��폜
			lcd_cursor(x - 1, 0);
			lcd_printch(' ');
			//�[�����ɍs���܂ŕ�����\��
			for (i = 0; string[i] != '\0' && i + x < 16; i++)
				lcd_printch(string[i]);
			wait1ms(250);
		}
}
