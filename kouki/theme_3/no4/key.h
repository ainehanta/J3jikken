#ifndef _KEY_H
#define _KEY_H

#include "h8-3052-iodef.h"

//チャタリング取り除きms
#define KEY_WAIT 20

typedef struct
{
  unsigned int prev;
  unsigned int now;
  unsigned int mask;
} key_status;

void key_init(key_status*);
void key_scan(key_status*);
void key_scan_once(key_status*);
char key_press_check(key_status*); //同時押し非対応

#endif
