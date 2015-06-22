#include "h8-3052-iodef.h"
#include "lcd.h"

#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3

void calc(int, int, int);
void string_add(char*, char*);

void lcd_print_num(int);
void num_conma(char*, int);

void lcd_print_with_position(char*, int, int);
void nw_check(int*, int*);

void my_itoa(char*, int);
void turn_str(char*);

int main(void) {
	lcd_init();

	calc(MUL, 100, 10);

	return 0;
}

//計算を行い表示する
void calc(int op, int a, int b) {
	int answer = 0;
	char opch[2];		 //演算子バッファ
	char result[33]; //結果用バッファ
	char buffer[33]; //作業用バッファ

	opch[1] = '\0';
	switch (op) {
	case ADD:
		answer = a + b;
		opch[0] = '+';
		break;
	case SUB:
		answer = a - b;
		opch[0] = '-';
		break;
	case MUL:
		answer = a * b;
		opch[0] = '*';
		break;
	case DIV:
		answer = a / b;
		opch[0] = '/';
		break;
	}

	//結果および演算子を結合し結果バッファに格納
	num_conma(result, a);
	string_add(result, opch);
	num_conma(buffer, b);
	string_add(result, buffer);
	string_add(result, "=");
	num_conma(buffer, answer);
	string_add(result, buffer);

	//結果バッファの内容を表示
	lcd_print_with_position(result, 0, 0);
}

//文字列を結合する
void string_add(char* a, char* b) {
	int a_length;
	int i;

	for (a_length = 0; a[a_length] != '\0'; a_length++)
		;

	for (i = 0; b[i] != '\0'; i++) {
		a[a_length++] = b[i];
	}

	a[a_length++] = '\0';
}

//数値を数字に変換する
void my_itoa(char* string, int num) {
	int i;
	int sp;
	int max_dig;

	//0のときの処理
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
	//逆順に代入されているので補正する
	turn_str(string);
}

//文字列の順番を逆順にする
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

//右詰めに数値を3桁ごとにコンマを挿入して表示
void lcd_print_num(int num) {
	char tmp[33];
	int i;

	num_conma(tmp, num);

	for (i = 0; tmp[i] != '\0'; i++)
		;

	//表示座標を右端から文字数分引いたところから始める（右詰め）
	lcd_cursor(16 - i, 0);
	lcd_printstr(tmp);
}

//引数で渡ってきた数値を数字に変換し、3桁ごとにコンマを挿入
void num_conma(char* string, int num) {
	char tmp[33];

	int i;
	int length;
	int max_length;
	int tmp_i;

	//数値を数字へ
	my_itoa(tmp, num);

	//コンマを含んだ文字数を計算
	for (length = 0; tmp[length] != '\0'; length++)
		;
	max_length = length;
	max_length += (length / 3);

	if (length % 3 == 0) {
		max_length--;
	}

	//コンマなしの文字列を後ろから結果用の文字列に代入
	//3つごとにコンマを挿入する
	for (i = max_length, tmp_i = length; tmp_i >= 0; tmp_i--) {
		string[i--] = tmp[tmp_i];
		if ((length - tmp_i) % 3 == 0 && (length - tmp_i) != 0) {
			string[i--] = ',';
		}
	}
	string[max_length] = '\0';
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
