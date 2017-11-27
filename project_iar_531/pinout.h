#ifndef RFID_h
#define RFID_h

#include <msp430g2553.h>

typedef enum  { on, off }lightStatus;
typedef enum { green, red }led;
typedef enum {click, doubleClick, longPress, release} pressType;

void pinInit (void);
void light (led _led,lightStatus status);
pressType buttonCheck (void);
#endif