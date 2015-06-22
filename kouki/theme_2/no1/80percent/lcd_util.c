#include "lcd_util.h"

//���ͤ�������Ѵ�����
void my_itoa(char* string, int num) {
	int i;
	int sp;
	int max_dig;

	//0�ΤȤ��ν���
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
	//�ս����������Ƥ���Τ���������
	turn_str(string);
}

//ʸ����ν��֤�ս�ˤ���
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
