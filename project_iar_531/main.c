
#include "io430.h"

#define GRN_LED BIT6
#define RED_LED BIT0

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  P1DIR |= GRN_LED;
  P1OUT |= GRN_LED;
  
  while(1) {
/* all work is done in the ISR */
}

  return 0;
}
