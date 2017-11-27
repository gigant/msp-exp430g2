#include "pinout.h"

void pinInit (void)
{
//������������ �����
  
  // ������� ��������� ��������������� ��� ����� ����������� ��� SPI
  //P1DIR|=BIT6;
  //P1OUT&=~BIT6;
  // ������ ���������
  P1DIR|=BIT0;
  P1OUT&=~BIT0;
  // ������ P1.3
  P1REN = BIT3;  // ���������� ��������
  P1OUT = BIT3;
  
  // ������������ SPI
  P1SEL |= BIT5 + BIT6 + BIT7;// �������� ������������� �������� ����
  P1SEL2 |= BIT5 + BIT6 + BIT7;// �������� ������������� �������� ����
  
  P2DIR |= BIT5; // SPI �����
  P2OUT &=~BIT5;
  
  P2DIR |=BIT4; // SPI slave select
  P2OUT |=BIT4;
}

pressType  buttonCheck(void)
{ // �������� ��������� ������ ��� ���� ��������
  if ( (P1IN & BIT3)!= BIT3) {
    	 __delay_cycles(220000);
    	return click;
  }
    return release;
};

void light (led _led, lightStatus status)
{
  if (_led == green)
    if (status == on) P1OUT |= BIT6;
  	else if (status == off) P1OUT &=~ BIT6;
	
  if (_led == red)
    if (status == on) P1OUT |= BIT0;
  	else if (status == off) P1OUT &=~ BIT0;
}