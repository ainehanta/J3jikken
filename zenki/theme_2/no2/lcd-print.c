#include "h8-3052-iodef.h"
#include "lcd.h"

void lcd_print_with_position(char*, int, int);
void nw_check(int*, int*);

int main(void) {
	lcd_init();

	lcd_print_with_position("abcdefghijklmnopqrstuvwxyz0123456789", 11, 1);

	return 0;
}

//�C�Ӎ��W�ɕ������\���A�������s
void lcd_print_with_position(char* string, int x, int y) {
	int i;

	//�ꕶ�����\��
	for (i = 0; string[i] != '\0' && i < 32; i++) {
		lcd_cursor(x, y);
		lcd_printch(string[i]);
		x++;

		//���s���K�v���`�F�b�N
		nw_check(&x, &y);
	}
}

//���s���K�v���`�F�b�N���A�K�v�ȏꍇ���s�������W�l����
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