#include "h8-3052-iodef.h"
#include "h8-3052-int.h"
#include "lcd.h"
#include "ad.h"

#define  HANTEILEVEL 60 /* この値が大きいと判定が甘くなる  */

int            main(void);
unsigned char  key_read(void);
void           level_ck(unsigned short);
void           ad_data_get_3ch(void);
char           hikaku_3ch(void);

unsigned short touroku_ad3, touroku_ad4, touroku_ad5, hikaku_ad3, hikaku_ad4, hikaku_ad5;

int main(void)
{
  /* キー入力情報を取得するための変数を宣言する */
  unsigned char cf, key_data;

  lcd_init();    /* LCD の初期化            */
  ad_init();     /* A/D 変換器の初期化      */

  /* キー入力取得のためのポートの初期化 */
  P6DDR &= ~0x07;     /* P60,1,2   入力 */
  PADDR |= 0x0f;      /* PA0,1,2,3 出力 */

  PBDDR = 0xff; /* バイオメトリクス用ポート設定 */
  PBDR  = 0x00; /* 全ポートOFF                  */
  P9DDR = 0xff; /* 出力ポートに設定             */

  while (1) {

    P9DR  = 0x30; /* LED OFF */

    /* 登録用の指の入力を促すメッセージを表示する記述を書く   */

    /* バイオメトリクス入力に関するポートの全ポートをONにし， */
    /* 指が置かれるのを検出する記述を書く．                   */
    /* 指が置かれるまではループするようにするが，level_ck()   */
    /* を良く見て適切な記述をすること                         */

    P9DR = 0x10;   /* 緑LED ON */

    /* AD変換器により3ch分のデータを取り込む記述を書く        */
    /* 適切な関数の使用により実現される                       */

    /* 登録用データが得られたことを示すメッセージを表示する記述を書く */
    /* [*]キーの入力を促すメッセージを表示する記述を書く              */
   

    /* キー入力待ち状態を作る(キー入力されるまでループを抜けない)     */
    /* key_read()を用いて記述する                                     */
    
    P9DR  = 0x30; /* LED OFF */

    /* 比較用の指の入力を促すメッセージを表示する記述を書く   */

    /* バイオメトリクス入力に関するポートの全ポートをONにし， */
    /* 指が置かれるのを検出する記述を書く．                   */
    /* 指が置かれるまではループするようにするが，level_ck()   */
    /* を良く見て適切な記述をすること                         */

    P9DR  = 0x20;  /* 赤色LED ON */

    /* AD変換器により3ch分の比較用データを取り込む記述を書く  */
    /* 適切な関数の使用により実現されるが，比較結果が返され   */
    /* る点に注意すること                                     */ 

    /* 返された判定結果に応じて，認証されたか否かを示す       */
    /* メッセージを表示する記述を書く                         */              
    
    /* [*]キーの入力を促すメッセージを表示する記述を書く      */

    /* バイオメトリクス入力に関するポートの全ポートをOFFに    */
    /* する記述を書く                                         */

    /* 再度キー入力待ち状態を作る(キー入力されるまでループを抜けない) */
    /* key_read()を用いて記述する                                     */
 
  }
  return 1;
}

void level_ck(unsigned short level_data)
     /* 指が置かれたかどうかを検出する関数 */
     /* 指が置かれるまでループする         */
     /* 今回は引数として204を与えること    */
{
  unsigned short data;

  data = 0;

  while (data < level_data)
    {
      ADCSR = 0x04;
      ADCSR |= 0x20;
      while ((ADCSR & 0x80) == 0);

      ADCSR &= 0x7f;
      data = ADDRAH;
      data <<= 8;
      data += ADDRAL;
      data >>= 6;
    }
}

void ad_data_get_3ch(void)
     /* バイオメトリクス部分に対応したAD変換器を */
     /* 用いて3チャネル分のデータを取り込む関数  */
     /* 登録用データが保存される                 */
{
  unsigned short data;

      ADCSR = 0x03;
      ADCSR |= 0x20;
      while ((ADCSR & 0x80) == 0);

      ADCSR &= 0x7f;
      data = ADDRDH;
      data <<= 8;
      data += ADDRDL;
      data >>= 6;

      touroku_ad3 = data;

      ADCSR = 0x04;
      ADCSR |= 0x20;
      while ((ADCSR & 0x80) == 0);

      ADCSR &= 0x7f;
      data = ADDRAH;
      data <<= 8;
      data += ADDRAL;
      data >>= 6;

      touroku_ad4 = data;

      ADCSR = 0x05;
      ADCSR |= 0x20;
      while ((ADCSR & 0x80) == 0);

      ADCSR &= 0x7f;
      data = ADDRBH;
      data <<= 8;
      data += ADDRBL;
      data >>= 6;

      touroku_ad5 = data;
}

char hikaku_3ch(void)
     /* バイオメトリクス部分に対応したAD変換器を     */
     /* 用いて3チャネル分のデータを取り込む．        */
     /* 比較用データを保存し，あらかじめ取得していた */
     /* 登録用データとの比較を行い判定結果を返す関数 */
{
   unsigned short data, up_ad3, up_ad4, up_ad5, down_ad3, down_ad4, down_ad5;
   char hantei;

      ADCSR = 0x03;
      ADCSR |= 0x20;
      while ((ADCSR & 0x80) == 0);

      ADCSR &= 0x7f;
      data = ADDRDH;
      data <<= 8;
      data += ADDRDL;
      data >>= 6;

      hikaku_ad3 = data;

      ADCSR = 0x04;
      ADCSR |= 0x20;
      while ((ADCSR & 0x80) == 0);

      ADCSR &= 0x7f;
      data = ADDRAH;
      data <<= 8;
      data += ADDRAL;
      data >>= 6;

      hikaku_ad4 = data;

      ADCSR = 0x05;
      ADCSR |= 0x20;
      while ((ADCSR & 0x80) == 0);

      ADCSR &= 0x7f;
      data = ADDRBH;
      data <<= 8;
      data += ADDRBL;
      data >>= 6;

      hikaku_ad5 = data;

      hantei = 0;

      if (touroku_ad3 + HANTEILEVEL > 0x3ff) { 
	up_ad3 = 0x3ff;
      } else {
	up_ad3 = touroku_ad3 + HANTEILEVEL;
      }

      if (touroku_ad3 - HANTEILEVEL < 0x00) {
	down_ad3 = 0;
      } else {
	down_ad3 = touroku_ad3 - HANTEILEVEL;
      }

      if (touroku_ad4 + HANTEILEVEL > 0x3ff) { 
	up_ad4 = 0x3ff;
      } else {
	up_ad4 = touroku_ad4 + HANTEILEVEL;
      }

      if (touroku_ad4 - HANTEILEVEL < 0x00) {
	down_ad4 = 0;
      } else {
	down_ad4 = touroku_ad4 - HANTEILEVEL;
      }

      if (touroku_ad5 + HANTEILEVEL > 0x3ff) { 
	up_ad5 = 0x3ff;
      } else {
	up_ad5 = touroku_ad5 + HANTEILEVEL;
      }

      if (touroku_ad5 - HANTEILEVEL < 0x00) {
	down_ad5 = 0;
      } else {
	down_ad5 = touroku_ad5 - HANTEILEVEL;
      }

      if (hikaku_ad3 <= up_ad3 && down_ad3 <= hikaku_ad3) {
	 if (hikaku_ad4 <= up_ad4 && down_ad4 <= hikaku_ad4) {
	    if (hikaku_ad5 <= up_ad5 && down_ad5 <= hikaku_ad5) {
	      hantei = 1;
	    }
	 }
      }

      return hantei;
}

unsigned char  key_read(void)
    /* キー入力状態を取得する関数 */
{
  /* キー入力取得のための変数を宣言する */
  unsigned char cf, key_data;

  /* キー入力判定用変数の初期化*/
  cf = 0;
  key_data = 0;

  while (cf == 0 ){  /* キー入力するまでループする */
    PADR = 0x0e;
    cf   = P6DR;
    cf   = ~cf;
    cf  &= 0x07;
    switch(cf) {
      case 1: key_data = '*'; break;
    }
  }

  /* 入力されたキーの情報を返す */
  return key_data;
}
