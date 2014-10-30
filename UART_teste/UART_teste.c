#include <msp430.h>

#define RX_pin BIT1 // P1.1
#define TX_pin BIT2 // P1.2

void Serial_config(void);
void Serial_escreve_dado(char dado);
void Serial_escreve_texto(char *caracter);

char inicio = 1;

#pragma vector = USCIAB0RX_VECTOR
__interrupt void Serial_receive(void){
    if (inicio){
        Serial_escreve_texto("Interface Serial\n> ");
        inicio = 0;
        IFG2 &= ~UCA0RXIFG; // limpa flag int. RX
    }else{
        Serial_escreve_dado(UCA0RXBUF); // devolve byte recebido
        Serial_escreve_texto("\n> "); // pula linha + "> "
    }
}

void main(void){
    
    // desabilita wdt
    WDTCTL = WDTPW + WDTHOLD;

    // configura clock 8MHz
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;

    Serial_config();

    __enable_interrupt(); // habilita GIE
        
    while(1);
}

void Serial_config(void){
    // configura pinos serial
    P1SEL |= (RX_pin + TX_pin);
    P1SEL2 |= (RX_pin + TX_pin);

    // USCI reset: desabilitado para operacao
    // UCSWRST (BIT1) = 1
    UCA0CTL1 |= UCSWRST; // |= 0x01

    // modo UART:
        // UCMODE1 (BIT2) = 0
        // UCMODE0 (BIT1) = 0
    // modo assincrono:
        // UCSYNC (BIT0) = 0
    UCA0CTL0 &= ~(UCMODE1 + UCMODE0 + UCSYNC); // &= ~0x07

    // USCI clock: modo 2 (SMCLK):
    // UCSSEL (BIT7 e BIT6):
            // (BIT7) = 1
            // (BIT6) = 0
    UCA0CTL1 |= UCSSEL_2; // |= 0x80

    // Oversampling desabilitado
        //  UCOS16 (BIT1) = 0
    UCA0MCTL &= ~UCOS16;

    // 9600 bps: 8M / 9600 = 833,333
    // int(8M / 9600) = 833 = 0x[03][41]
    // round((833,333 - 833)*8) = [3]
    UCA0BR1 = 0x03;
    UCA0BR0 = 0x41;
    UCA0MCTL |= 0x06; // |= (0x[3]) << 1;

    // USCI reset: liberado para operacao
    UCA0CTL1 &= ~UCSWRST; // &= ~0x01

    // Habilita int. de recepcao
    IE2 |= UCA0RXIE;
}

void Serial_escreve_dado(char dado){
    while(!(IFG2 & UCA0TXIFG)); // aguarda buffer vazio
    UCA0TXBUF = dado;
}

void Serial_escreve_texto(char *caracter){
    while(*caracter){
        Serial_escreve_dado(*caracter);
        caracter++;
    }
}