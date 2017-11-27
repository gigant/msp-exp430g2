
#include "spi.h"

void spiInit (void)
{
    UCB0CTL1 = UCSWRST;

 // (3) 3-pin, 8-bit SPI master
    UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC; 
    UCB0CTL1 |= UCSSEL_2;   // SMCLK
    UCB0BR0 = 12; // fSCL = SMCLK/12 = ~100kHz
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST; 
};

void spiWrite (unsigned char tx)
{
	while (!(IFG2 & UCB0TXIFG));  	
	UCB0TXBUF = tx; 
	while (UCB0STAT & UCBUSY); 	
};

unsigned char spiRead (void)
{
	while (!(IFG2 & UCB0TXIFG));
	return UCB0RXBUF;
};
