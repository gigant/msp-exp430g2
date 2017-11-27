#ifndef SPI_h
#define SPI_h

#include <msp430g2553.h>

void spiInit(void);
void spiWrite(unsigned char);
unsigned char spiRead (void);
#endif