#include <msp430.h>

// Botao conectado a P1.3
#define botao 0x08

// Bobinas: A: P2.1, B:P2.2, A':P2.3, B':P2.4
#define BOBINAS (BIT1 + BIT2 + BIT3 + BIT4)

// Sequencia dos passos
unsigned char passo[4] = {0x02, 0x04, 0x08, 0x10};
// Seleção da bobina
char bobina = 0;
// Sentido de rotação
unsigned char sentido = 1;

__attribute__((interrupt(PORT1_VECTOR)))
void int_P1(void){
    
    // Inverte o sentido da rotação
    sentido = -sentido;
    
    // Debouncing
    __delay_cycles(1000);
    while(!(P1IN & botao));
    __delay_cycles(1000);

    // limpa flag int. P1
    P1IFG &= ~botao;
}

__attribute__((interrupt(TIMER0_A0_VECTOR)))
void int_timer_A(void){
    
    // Próximo passo
    if((bobina += sentido) > 3){
        bobina = 0;
    }else if(bobina < 0){
        bobina = 3;
    }

    P2OUT &= ~BOBINAS;
    P2OUT |= passo[bobina];

    TACCTL0 &= ~CCIFG; // limpa flag int. timer A0
}

void main(void){
      
    // Desabilita watchdog
    WDTCTL = WDTPW + WDTHOLD;
  
    // Clock 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
  
    // Configuração de I/O
    P2DIR |= BOBINAS;
    P2OUT &= ~BOBINAS;

    P1REN = P1OUT = botao; // habilita e ativa o resistor pullup do botao
    
    P1IE = P1IES = botao; // habilita int. somente no botao e config. como borda de descida
    P1IFG = 0; // limpa int. P1
    
    TA0CTL = TASSEL_2 + ID_1 + MC_1; // SMCLK + DIV(1) + UP CCR0
    TACCTL0 |= CCIE; // habilita int. por comparacao
    TACCR0 = 5000; // 5ms
    
    __enable_interrupt(); // habilita GIE
        
    while(1); // loop infinito      
}