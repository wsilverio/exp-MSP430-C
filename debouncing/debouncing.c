#include <msp430.h>

// Mascaras dos perifericos
#define LEDS 0x41 // leds conectados a P1.0 e P1.6
#define	botao 0x08 // Botao conectado a P1.3

unsigned char pisca = 0; // global char 8 bits

#pragma vector = PORT1_VECTOR
__interrupt void int_P1(void){
	if(pisca ^= 1){
		P1OUT 	|= 0x01; // acende vermelho
		TA0CTL 	|= MC_1; // timer em modo UP
	}else{
		P1OUT 	&= ~LEDS; // apaga leds
		TA0CTL 	&= ~MC_1; // timer em modo STOP
	}
	
	// Debouncing
	__delay_cycles(10);
	while(!(P1IN & botao));
	__delay_cycles(10);

	P1IFG &= ~botao; // limpa flag int. P1
}
#pragma vector = TIMER0_A0_VECTOR
__interrupt void int_timer_A(void){
	P1OUT ^= LEDS; // inverte leds
	TACCTL0 &= ~CCIFG;	// limpa flag int. timer A0
}

void main(void){
	  
	WDTCTL = WDTPW + WDTHOLD; // Desabilita watchdog
  
	// Clock 1MHz
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
  
	P1DIR = LEDS; // ledss como saida
	P1REN = P1OUT = botao; // habilita e ativa o resistor pullup do botao e apaga os leds
	
	P1IE = P1IES = botao; // habilita int. somente no botao e config. como borda de descida
	P1IFG = 0; // limpa int. P1
	
	TA0CTL = TASSEL_2 + ID_2 + MC_0; // SMCLK + DIV(2) + STOP
	TACCTL0	|= CCIE; // habilita int. por comparacao
	TACCR0 = 62500; // * div(2) = 125000us : periodo = 250ms
	
	__enable_interrupt(); // habilita GIE
		
	while(1); // loop infinito		
}
