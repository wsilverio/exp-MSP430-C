
/*
    Display LCD 16x2
    Barramento: P2.7 ... P2.4
    RS: P1.6
    E: P1.7
    R/W: GND
*/

#include <msp430.h>

#define PIN_DATA (BIT7 + BIT6 + BIT5 + BIT4) // D7 ... D4
#define RS BIT6
#define E BIT7

void LCD_inicializa(void);
void LCD_pulso_E(void);
void LCD_escreve_bin(char bin, char rs);
void LCD_escreve_texto(char *caracter);
void LCD_limpa_display(void);
void LCD_pos_cursor(char lin, char col);
void LCD_home(void);
void LCD_cursor_config(char mostra, char blink);
void itoa(long unsigned int inteiro, char* string, int base);

void main(void){
    
    // desabilita wdt
    WDTCTL = WDTPW + WDTHOLD;

    // configura clock 1MHz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    
    // configura portas
    P2DIR |= PIN_DATA;
    P2SEL &= ~PIN_DATA;
    P2SEL2 &= ~PIN_DATA;
    P2OUT &= ~PIN_DATA;
    
    P1DIR |= (RS + E);
    P1OUT &= ~(RS + E);

    // inicializa display
    LCD_inicializa(); 

    // mostra cursor + pisca
    LCD_cursor_config(1, 1);

    // linha 0, coluna 5
    LCD_pos_cursor(0, 5);

    LCD_escreve_texto("TE-124\n      UFPR");
        
    while(1);
}

void LCD_inicializa(void){
    __delay_cycles(15000); // 15ms
    
    P1OUT &= ~RS; // RS = 0: cmd
    
    P2OUT &= ~PIN_DATA; // zera o barramento

    P2OUT |= 0x30;
    LCD_pulso_E(); //Escreve o HSB de 0x30 (necessario)
    __delay_cycles(5000); // 5ms
    
    LCD_pulso_E(); //Escreve o HSB de 0x30 (necessario)
    __delay_cycles(1000); // 1ms
    
    LCD_pulso_E(); //Escreve o HSB de 0x30 (necessario)
    __delay_cycles(1000); // 1ms
    
    // zera o barramento
    P2OUT &= ~PIN_DATA;

    P2OUT |= 0x20;
    LCD_pulso_E(); //Escreve o HSB de 0x20 (necessario)
    __delay_cycles(1000); // 1ms

    LCD_escreve_bin(0x28, 0); // interface 4 bits, 2 linhas, 5x8 pixels
    __delay_cycles(1000); // 1ms    

    LCD_escreve_bin(0x08, 0); // display off
    __delay_cycles(1000); // 1ms

    LCD_escreve_bin(0x01, 0); // display clear
    __delay_cycles(1000); // 1ms

    LCD_escreve_bin(0x06, 0); // incrementa o AC sem deslocar o display
    __delay_cycles(1000); // 1ms

    LCD_escreve_bin(0x0C, 0); // display on
    __delay_cycles(1000); // 1ms 
    
}

void LCD_pulso_E(){
    // pulso enable (1ms)
    P1OUT &= ~E;
    P1OUT |= E;
    P1OUT &= ~E;
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
    LCD_pulso_E();
    
    // zera o barramento
    P2OUT &= ~PIN_DATA;

    // escreve LSB
    P2OUT |= (0x0F & bin) << 4;

    // pulso enable (1ms)
    LCD_pulso_E();
}

void LCD_limpa_display(void){
    LCD_escreve_bin(0x01, 0);
}

void LCD_escreve_texto(char *caracter){
    while(*caracter){
        if(*caracter == '\n'){
            LCD_pos_cursor(1, 0); // pula a linha: linha 1, coluna 0
        }else{
            LCD_escreve_bin(*caracter, 1);
        }
        caracter++;
    }

}

void LCD_pos_cursor(char lin, char col){
    // linha 0: 0x00 ... 0x0F
    // linha 1: 0x40 ... 0x4F

    if(col < 0){
        col = 0;
    }else if(col > 16){
        col = 16;
    }
    
    if (lin < 0){
        lin = 0;
    }else if( lin > 1){
        lin = 1;
    }

    char posicao = (0x40 * lin) + col;

    LCD_escreve_bin(BIT7 + posicao, 0);
}

void LCD_home(void){
    LCD_escreve_bin(0x02, 0); // zera o AC
}

void LCD_cursor_config(char mostra, char blink){
    // mostra = 1: mostra o cursor
    // mostra = 0: esconde o cursor
    // blink = 1: cursor piscando
    // blink = 0: cursor estático
    LCD_escreve_bin((BIT3 + BIT2 + (mostra?BIT1:0) + blink), 0);

}

void itoa(long unsigned int inteiro, char* string, int base){
    // checa se a base é válida
    if (base < 2 || base > 36) { *string = '\0';}
    char* ptr = string, *ptr1 = string, tmp_char;
    int tmp_inteiro;
    do {
        tmp_inteiro = inteiro;
        inteiro /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_inteiro - inteiro * base)];
    } while ( inteiro );
    
    // Aplica sinal negativo
    if (tmp_inteiro < 0) *ptr++ = '-';
    
    *ptr-- = '\0';
    
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
}