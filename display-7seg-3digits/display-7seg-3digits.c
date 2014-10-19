#include <msp430.h>

unsigned int contador = 0;

void print_7seg(unsigned char n){
	// 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
	unsigned char digito[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
	P1OUT = digito[n];
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void int_timer_A(void){
	if(++contador > 999) contador = 0;
	TACCTL0 &= ~CCIFG;
}

void main(void){
	  
	WDTCTL = WDTPW + WDTHOLD;
  
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
  
	P1DIR = 0xFF;
	P2DIR = 0x07; // Q1: BIT2, Q2: BIT1, Q3: BIT0
	P1OUT = 0;
	P2OUT &= ~0x07;
		
	TA0CTL = TASSEL_2 + ID_1 + MC_1; // SMCLK + DIV(2) + UP
	TACCTL0	|= CCIE; // int. por comparacao
	TACCR0 = 50000; // * DIV(2) = 100 ms
	
	__enable_interrupt();
		
	while(1){
		int numero = contador; // ex.: 751

		print_7seg(numero / 100); // (751 / 100) = 7
		P2OUT &= ~(BIT0 + BIT1);
		P2OUT |= BIT2; // Q1
		__delay_cycles(1000);

		numero %= 100; // (751 % 100) = 51
	
		print_7seg(numero / 10); // (51 / 10) = 5
		P2OUT &= ~(BIT0 + BIT2);
		P2OUT |= BIT1; // Q2
		__delay_cycles(1000);

		numero %= 10; // (51 % 10) = 1

		print_7seg(numero); // = 1
		P2OUT &= ~(BIT1 + BIT2);
		P2OUT |= BIT0; // Q3
		__delay_cycles(1000);
	}
}