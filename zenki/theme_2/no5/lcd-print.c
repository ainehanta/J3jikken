#include "h8-3052-iodef.h"
#include "lcd.h"

void lcd_print_num(int);
void num_conma(char*, int);

void lcd_print_with_position(char*, int, int);
void nw_check(int*, int*);

void my_itoa(char*, int);
void turn_str(char*);

int main(void) {
	lcd_init();

	lcd_print_num(0);

	return 0;
}

//���l�𐔎��ɕϊ�����
void my_itoa(char* string, int num) {
	int i;
	int sp;
	int max_dig;

	//0�̂Ƃ��̏���
	if (num == 0) {
		string[0] = '0';
		string[1] = '\0';
		return;
	}

	for (i = num, max_dig = 0; i > 0; i /= 10, max_dig++)
		;

	for (sp = 0, i = 0; i < max_dig; i++, sp++) {
		string[sp] = num % 10 + '0';
		num /= 10;
	}
	string[sp++] = '\0';
	//�t���ɑ������Ă���̂ŕ␳����
	turn_str(string);
}

//������̏��Ԃ��t���ɂ���
void turn_str(char* string) {
	int length;
	int sp;
	char tmp;

	for (length = 0; string[length] != '\0'; length++)
		;
	length--;
	for (sp = 0; sp <= length / 2; sp++) {
		tmp = string[sp];
		string[sp] = string[length - sp];
		string[length - sp] = tmp;
	}
}

//�E�l�߂ɐ��l��3�����ƂɃR���}��}�����ĕ\��
void lcd_print_num(int num) {
	char tmp[33];
	int i;

	num_conma(tmp, num);

	for (i = 0; tmp[i] != '\0'; i++)
		;

	//�\�����W���E�[���當�������������Ƃ��납��n�߂�i�E�l�߁j
	lcd_cursor(16 - i, 0);
	lcd_printstr(tmp);
}

//�����œn���Ă������l�𐔎��ɕϊ����A3�����ƂɃR���}��}��
void num_conma(char* string, int num) {
	char tmp[33];

	int i;
	int length;
	int max_length;
	int tmp_i;

	//���l�𐔎���
	my_itoa(tmp, num);

	//�R���}���܂񂾕��������v�Z
	for (length = 0; tmp[length] != '\0'; length++)
		;
	max_length = length;
	max_length += (length / 3);

	if (length % 3 == 0) {
		max_length--;
	}

	//�R���}�Ȃ��̕��������납�猋�ʗp�̕�����ɑ��
	//3���ƂɃR���}��}������
	for (i = max_length, tmp_i = length; tmp_i >= 0; tmp_i--) {
		string[i--] = tmp[tmp_i];
		if ((length - tmp_i) % 3 == 0 && (length - tmp_i) != 0) {
			string[i--] = ',';
		}
	}
	string[max_length] = '\0';
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