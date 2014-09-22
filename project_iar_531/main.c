
#include "msp430g2553.h"

	#define GRN_LED BIT6
	#define RED_LED BIT0

	void msp_init (void);

int main( void )
{
  msp_init();
  
  P1DIR |= GRN_LED;
  P1OUT |= GRN_LED;
  
  while(1) {
/* all work is done in the ISR */
}

  return 0;
}

void msp_init(void)
{
  // отключение watchgog таймера
  WDTCTL = WDTPW + WDTHOLD;
  
  _BIS_SR (OSCOFF);
  //SR |= OSCOFF;
  BCSCTL1 |=XTS;
  BCSCTL3 |= LFXT1S0;
  IFG1 &= ~OFIFG;
  unsigned int i;
  for (i = 0; i < 10000; i++);
  if (IFG1&&OFIFG)   for (i = 0; i < 10000; i++);
  BCSCTL2 |=SELM0;//+SELM1;
  for (i = 0; i < 10000; i++);
}

/*
BIC.W #OSCOFF,SR ; Turn on osc.
BIS.B #XTS,&BCSCTL1 ; HF mode
MOV.B #LFXT1S0,&BCSCTL3 ; 1-3MHz Crystal
L1 BIC.B #OFIFG,&IFG1 ; Clear OFIFG
MOV.W #0FFh,R15 ; Delay
L2 DEC.W R15 ;
JNZ L2 ;
BIT.B #OFIFG,&IFG1 ; Re-test OFIFG
JNZ L1 ; Repeat test if needed
BIS.B #SELM1+SELM0,&BCSCTL2 ; Select LFXT1CLK*/
