#include "h8-3052-iodef.h"
#include "lcd.h"
#include <string.h>

#define LEFT  -1
#define RIGHT 1

//�X�N���[���ݒ�p�̍\����
typedef struct {
	char string[33];
	int speed;
	�@ //�X�s�[�h
	int x;
	�@�@�@ //�J�n���W
	int y;			//�J�n���W
	int direction;	//����
} SCROLL_FACTORY;

void lcd_scroll(SCROLL_FACTORY);
void lcd_print_with_position(char*, int, int);
void nw_check(int*, int*);

int main(void) {
	//�X�N���[���̏����ݒ�
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

//��������X�N���[���\���A�\���̂ŕ�������ё��x��ݒ肷��
void lcd_scroll(SCROLL_FACTORY settings) {
	int i;

	for (;;) {
		//��ʂ�h��Ԃ��i�c���h�~�j
		lcd_cursor(0, 0);
		for (i = 0; i < 32; i++) {
			lcd_printch(' ');
			if (i == 15) {
				lcd_cursor(0, 1);
			}
		}

		lcd_print_with_position(settings.string, settings.x, settings.y);

		//�����ݒ�ɏ]���č��W���X�V
		settings.x += settings.direction;

		//���s�̔��f�A���W�̕ύX
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

//LCD�̔C�Ӎ��W�ɕ������\���A�������s
void lcd_print_with_position(char* string, int x, int y) {
	int i;

	//�ꕶ������LCD�ɕ\��
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