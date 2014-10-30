#include <msp430.h>

#define CLK 8000000 // ex.: 8M
#define BPS 9600 // ex.: 9600 bps

#define RX_pin BIT1 // P1.1
#define TX_pin BIT2 // P1.2

int roundX(float x);

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

    float n = ((float)CLK)/BPS;
    int BRx = (int) n;  
    // int BRSx = roundX((n - BRx)*8) << 1;

    UCA0BR1 = (BRx & 0xFF00) >> 8;
    UCA0BR0 = (BRx & 0x00FF);
    UCA0MCTL |= roundX((n - BRx)*8) << 1; // |= BRSx;

    // USCI reset: liberado para operacao
    UCA0CTL1 &= ~UCSWRST; // &= ~0x01

    // Habilita int. de recepcao
    IE2 |= UCA0RXIE;
}

int roundX(float x){
        return (x > 0)?((int)(x + 0.5)):((int)(x - 0.5));
}