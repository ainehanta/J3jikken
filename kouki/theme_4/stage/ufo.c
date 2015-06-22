#include "h8-3052-iodef.h"
#include "h8-3052-int.h"

#include "lcd.h"
#include "random.h"
#include "timer.h"
#include "key.h"
#include "da.h"
#include "sound.h"

#include "utils.h"

#define STAGENUM 5
#define CLEAR 1
#define DEAD  2
#define GAMEOVER 3

#define TRUE  1
#define FALSE 0

#define MAXDIGITNUM 16
#define MAXINVNUM   16

#define INITSPEED   2000
#define SPEEDUP     10

#define TONE_TARGET 83
#define LONG_TARGET 100
#define TONE_MISS   50
#define LONG_MISS   300
#define TONE_HIT    250
#define LONG_HIT    200
#define TONE_DEF    63
#define LONG_DEF    200

#define WAITFEWSEC  5000

volatile unsigned long n_time, speed_count, speed;
volatile int shift_flag;

unsigned char clear_flag;

int main(void);
char *ntos(unsigned int n, char *s);
int game_start(unsigned char tama,unsigned char teki);
void effect(char c);

void print_num(unsigned char num);
void print_status(unsigned char stage,unsigned char life,unsigned char tama);
void print_stage(unsigned char stage);
void print_life(unsigned char life);
void print_tama(unsigned char tama);

void int_imia0(void);
void int_imia1(void);

int main(void)
{
  unsigned char clear_flag;
  unsigned char teki[] = {5,10,15,20};
  unsigned char tama[] = {20,5,2,0};
  unsigned char stage;

  unsigned int score, high_score;
  char score_s[MAXDIGITNUM+1];

  ROMEMU();
  USE_UI();
  timer_pri_set(0,1);
  random_init();
  lcd_init();
  key_init();
  timer_init();
  sound_init();
  timer_set(1,1000);
  timer_start(1);
  ENINT();

  lcd_cursor(0,0);
  lcd_printstr(" 0:Game Start   ");
  lcd_cursor(0,1);
  lcd_printstr(" *:High Score   ");
  high_score = 0;

  lcd_cursor(0,0);

  while(1)
  {
    if (key_check(10) == KEYON){     /* *キー:ハイスコア表示 */
      lcd_cursor(0,0);                   /* LCD にハイスコア表示 */
      lcd_printstr(" High Score is  ");
      lcd_cursor(0,1);
      lcd_printstr("                ");
      lcd_cursor(0,1);
      lcd_printstr(ntos(high_score,score_s));
    }
    if (key_check(10) == KEYOFF){
      lcd_cursor(0,0);
      lcd_printstr(" 0:Game Start   ");
      lcd_cursor(0,1);
      lcd_printstr(" *:High Score   ");
    }
    if (key_check(11) == KEYON){      /* 0キー:ゲームスタート */
      lcd_cursor(0,0);                   /* LCD に操作方法表示 */
      lcd_printstr("*:Sight  0:Trig.");

      score = 0;
      clear_flag = CLEAR;
      stage = 0;
      while(clear_flag!=GAMEOVER)
      {
        score += game_start(teki[stage]+tama[stage],teki[stage]);              /* ゲームスタート */
        if(clear_flag==CLEAR)
        {
          stage++;
        }
        else if(clear_flag==DEAD)
        {
          clear_flag = CLEAR;
        }
        if(stage==STAGENUM)
        {
          clear_flag = GAMEOVER;
        }
        //clear_flag = FALSE; //TODEBUGUSE
      }

      lcd_cursor(0,1);                   /* 得点表示欄のクリア */
      lcd_printstr("                ");
      if (score > high_score){           /* ハイスコアのとき */
        high_score = score;                /* ハイスコア登録 */
        lcd_cursor(0,0);                   /* ハイスコア表示 */
        lcd_printstr(" High Score !!! ");
        lcd_cursor(0,1);
        lcd_printstr(ntos(high_score,score_s));
      } else {                           /* ハイスコアでないとき*/
        lcd_cursor(0,0);                   /* スコアを表示 */
        lcd_printstr(" Your Score ... ");
        lcd_cursor(0,1);
        lcd_printstr(ntos(score,score_s));
      }
      n_time = 0;
      while (n_time < WAITFEWSEC);       /* 得点表示後にちょっと待つ */
      lcd_cursor(0,0);                     /* LCD にメッセージ表示 */
      lcd_printstr(" 0:Game Start   ");
      lcd_cursor(0,1);
      lcd_printstr(" *:High Score   ");
    }
  }
}

char *ntos(unsigned int n, char *s)
{
  int count, i;
  char st[MAXDIGITNUM+1];
  char d;

  count = 0;
  do {
    d = n % 10;
    n = (n - d) / 10;
    st[count] = d + '0';
    count++;
  } while (n > 0);
  for (i = 0; i < count; i++) 
    s[i] = st[count - 1 - i];
  s[count] = '\0';
  return s;
}

int game_start(unsigned char tama,unsigned char teki)
{
  int target, score, gameover, hit, i, j;
  int prev_key10, prev_key11, key10, key11;
  unsigned int ds;
  unsigned char teki_disp_count = teki;
  char t, nc;
  char disp[MAXINVNUM+1]; /* [0: 1234567890n] のような画面イメージ */
  /*  照準,区切り,インベーダとUFO */
  target = 0; t = '0'; score = 0;     /* 照準とスコアの初期化 */
  speed_count = 0; speed = INITSPEED; /* プレイ速度の初期化 */
  shift_flag = FALSE;                 /* 敵の前進フラグの初期化 */
  disp[0] = '0'; disp[1] = ':';       /* 表示フィールドの初期化 */
  for (i = 2; i < MAXINVNUM; i++) disp[i] = ' ';
  disp[MAXINVNUM] = '\0';
  gameover = FALSE;       /* ゲーム終了フラグの初期化 */
  key10 = key_check(10);  /* キーバッファ(照準キー)の初期化 */
  prev_key10 = KEYOFF;
  key11 = key_check(11);  /* キーバッファ(発射キー)の初期化 */
  prev_key11 = KEYOFF;
  lcd_cursor(0,1);        /* 初期画面の表示 */
  lcd_printstr(disp);
  while (gameover == FALSE){ /* ゲームオーバでなければループ */
    /* キーの立上りを検出するための前回チェック時のキー状態を記憶 */
    /* キーバッファ(照準キー)処理 */
    if (key10 != KEYTRANS) prev_key10 = key10; /* 遷移中は前々回の値そのまま */
    key10 = key_check(10);
    /* キーバッファ(発射キー)処理 */
    if (key11 != KEYTRANS) prev_key11 = key11; /* 遷移中は前々回の値そのまま */
    key11 = key_check(11);
    if ((prev_key10 == KEYOFF) && (key10 == KEYON)){
      /* 照準キーが押されたときの処理 */
      /* 照準は 0->1->...->8->9->n->0->... と順に変化する */
      target++;                           /* 照準 +1 */
      if (target > 10) target = 0;        /* UFOの次は0 */
      if (target < 10) t = '0' + target;  /* 照準がUFOでないときのキャラ */
      else t = 'n';                       /* 照準がUFOのときのキャラ */
      disp[0] = t;                        /* 照準値を表示にセット */
      effect('t');                        /* 効果音を鳴らす */
      /* フィールドの表示 */
      lcd_cursor(0,1);
      lcd_printstr(disp);
    }
    if ((prev_key11 == KEYOFF) && (key11 == KEYON) && (tama != 0)){
      /* 発射キーが押されたときの処理 */
      hit = FALSE;  /* ヒット判定フラグの初期化 */
      i = 2;        /* 最も左の敵表示位置 */
      while ((i < MAXINVNUM) && (hit != TRUE)){ /* 列の左から命中を探す */
        if (disp[i] == t){    /* 照準と一致するキャラか? */
          hit = TRUE;           /*   ヒット判定 */
          ds = MAXINVNUM - i;   /*   基本得点の計算(砲台に近い程大) */
          score = score + ds;   /*   得点追加 */
          if (target > 9) score = score + ds * 2;  /* UFOなら3倍の得点 */
          for (j = i; j > 2; j--) disp[j] = disp[j-1];  /* 命中左側を右寄せ */
          disp[2] = ' ';        /* 最も左は常に空白を詰める */
          /* フィールドの表示 */
          lcd_cursor(0,1);
          lcd_printstr(disp);
        }
        i++;  /* 探索位置を +1 */
      } /* ヒット判定があるか右端まで調べたら, 探索終了 */
      if (hit == TRUE)
      {
        effect('s'); /* 命中時の効果音 */
        teki--;
      }
      else
      {
        effect('m');             /* 失敗時の効果音 */
      }
      tama--;
      print_tama(tama);
    }
    /* 敵が前進するタイミングのときの処理 */
    if (shift_flag == TRUE){          /* 前進フラグが立っているなら */

      if(teki == 0)
      {
        gameover = TRUE;
        clear_flag = CLEAR;
        break;
      }

      if(tama == 0)
      {
        gameover = TRUE;
        clear_flag = DEAD;
        break;
      }

      if (disp[2] != ' ') disp[1] = disp[2]; /* 侵略時のみ区切りに侵入 */
      for (i = 2; i < MAXINVNUM; i++)   /* 敵全体を左に1つシフト */
        disp[i] = disp[i + 1];
      if(teki_disp_count!=0)
      {
        if (score % 10 == 1) nc = 'n';    /* スコアの最下位が1ならUFO出現 */
        else nc = (random() % 10) + '0';  /* それ以外はランダムな数字 */
        disp[MAXINVNUM - 1] = nc;         /* 右端に新キャラを入れる */
        teki_disp_count--;
      }
      else
      {
        disp[MAXINVNUM - 1] = ' ';
      }
      shift_flag = FALSE;               /* 前進フラグの消去 */
      lcd_cursor(0,1);                  /* フィールドの表示 */
      lcd_printstr(disp);
    }
    if (disp[1] != ':')
    {
      gameover = TRUE;
      clear_flag = DEAD;
      break;
    }
  }
  return score; /* 得点を返す */
}

void effect(char c)
{
  unsigned int t;
  unsigned long p;

  switch (c) {       /* 種類によって音程と音長を変えられる */
    case 't':            /* 照準を動かしたとき */
      t = TONE_TARGET;
      p = LONG_TARGET;
      break;
    case 'm':            /* ミスしたとき */
      t = TONE_MISS;
      p = LONG_MISS;
      break;
    case 's':            /* ヒットしたとき */
      t = TONE_HIT;
      p = LONG_HIT;
      break;
    default:             /* その他のとき*/
      t = TONE_DEF;
      p = LONG_DEF;
  }
  sound_beep(t,p,32);
}

void print_num(unsigned char num)
{
  if(num/10==0)
  {
    lcd_printch(' ');
  }
  else
  {
    lcd_printch(num/10+'0');
  }
  lcd_printch(num%10+'0');
}

void print_status(unsigned char stage,unsigned char life,unsigned char tama)
{
  lcd_cursor(0,4);
  print_num(stage);
  //lcd_printch('A');

  lcd_cursor(0,10);
  print_num(life);
  //lcd_printch('B');

  lcd_cursor(0,14);
  print_num(tama);
  //lcd_printch('C');
}

void print_stage(unsigned char stage)
{
  lcd_cursor(0,4);
  print_num(stage);
}

void print_life(unsigned char life)
{
  lcd_cursor(0,10);
  print_num(life);
}

void print_tama(unsigned char tama)
{
  lcd_cursor(0,14);
  print_num(tama);
}


#pragma interrupt
void int_imia1(void)
{
  ENINT1();
  key_sense();
  n_time++;
  speed_count++;
  if (speed_count >= speed){
    speed_count = 0;
    speed = speed - SPEEDUP;
    shift_flag = TRUE;
  }

  timer_intflag_reset(1);

  ENINT();
}

