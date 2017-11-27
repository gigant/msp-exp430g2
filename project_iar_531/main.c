/*
Пример программы работы с платой rfid-rc522
*/

#include <msp430g2553.h>
#include "pinout.h"
#include "spi.h"
#include "rfid.h"

extern void rfid_init(void);
extern unsigned char readMFRC522( unsigned char addr);
extern unsigned char isCard(void);
extern unsigned char getVersion ();
extern void selftest (void);
static unsigned char rfidStatus1, rfidStatus2;
static unsigned char MFRCver;

int main(void)
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  // Настройка кварца
  BCSCTL1 = CALBC1_16MHZ;                     // Set range
  DCOCTL = CALDCO_16MHZ;                      // Set DCO step + modulation
 
  pinInit();
  spiInit();
  rfid_init();
  MFRCver = getVersion();
  while (1)
  {
    selftest();
    //	light(red,on);
 	rfidStatus1 = readMFRC522( 0x2A );
	//rfidStatus2 = readMFRC522(5);
	__delay_cycles(50000); // finish clearing (minimum (n+1)*16 cycles)
    //	light(red,off);
	if (isCard()) light(red,on);
  }
}
