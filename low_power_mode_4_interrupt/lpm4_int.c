#include <msp430.h>

#define LEDS 0x41 // leds conectados a P1.0 e P1.6
#define botao 0x08 // botao conectado a P1.3

#pragma vector = PORT1_VECTOR
__interrupt void int_P1(void){
  
    P1OUT ^= LEDS; // inverte os LEDs
  
    // debouncing
    __delay_cycles(1000);
    while(!(P1IN & botao));
    __delay_cycles(1000);

    P1IFG &= ~botao; // limpa int. P1
}

void main(void){
      
    WDTCTL = WDTPW + WDTHOLD; // desabilita wdt
    
    P1DIR = LEDS; // output
    P1REN = botao; // habilita resistor pullup no botao
    P1OUT = (botao + BIT0); // seta o pullup do botao e acende led vermelho
    
    P1IE = P1IES = botao; // habilita int. somente no botao + borda de descida
    P1IFG = 0; // limpa int. P1
  
    __enable_interrupt();
        
    __bis_SR_register(LPM4_bits); // low power mode 4
}