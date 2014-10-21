/*
    Display LCD 16x2
    Barramento: P2.7 - P2.4
    RS: P1.6
    E: P1.7
*/

#include <msp430.h>
// #include <string.h>

#define PIN_DATA 0xF0 // BIT7 + BIT6 + BIT5 + BIT4
#define RS 0x40 // BIT6
#define E  0x80 // BIT7

void LCD_inicializa(void);
void LCD_escreve_bin(char bin, char rs);
void LCD_escreve_texto(char texto[]);

void main(void){
    
    // desabilita wdt
    WDTCTL = WDTPW + WDTHOLD;

    // configura clock 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    
    // configura portas
    P2DIR = PIN_DATA;
    P2OUT &= ~PIN_DATA;
    P1DIR = (RS + E);
    P1OUT &= ~(RS + E);

    // inicializa display
    LCD_inicializa();   
}
void LCD_inicializa(void){
    __delay_cycles(15000); // 15ms

    LCD_escreve_bin(0x30, 0); // (necessario)
    __delay_cycles(5000); // 5ms

    LCD_escreve_bin(0x30, 0); // (necessario)
    __delay_cycles(1000); // 1ms

    LCD_escreve_bin(0x30, 0); // (necessario)
    __delay_cycles(1000); // 1ms

    LCD_escreve_bin(0x20, 0); // (necessario)
    __delay_cycles(1000); // 1ms

    LCD_escreve_bin(0x28, 0); // interface 4 bits, 2 linhas, 5x8 pixels
    __delay_cycles(1000); // 1ms    

    LCD_escreve_bin(0x08, 0); // display off
    __delay_cycles(1000); // 1ms

    LCD_escreve_bin(0x01, 0); // display clear
    __delay_cycles(1000); // 1ms

    LCD_escreve_bin(0x06, 0); // incrementa o AC sem deslocar o display
    __delay_cycles(1000); // 1ms

    LCD_escreve_bin(0x0F, 0); // display on, mostra cursor, blink
    __delay_cycles(1000); // 1ms 
}

void LCD_escreve_bin(char bin, char rs){
    __delay_cycles(1000); // 1ms

    // RS = 0: cmd
    // RS = 1: dados
    P1OUT = (rs)?(P1OUT | RS):(P1OUT & ~RS);

    // zera o barramento
    P2OUT &= ~PIN_DATA;
    
    // escreve HSB
    P2OUT |= (0xF0 & bin);

    // pulso enable (1ms)
    P1OUT |= E;
    P1OUT &= ~E;

    // escreve LSB
    P2OUT |= (0x0F & bin) << 4;

    // pulso enable (1ms)
    P1OUT |= E;
    P1OUT &= ~E;
}

void LCD_escreve_texto(char texto[]){
    
}