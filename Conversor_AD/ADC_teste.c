#include <msp430.h>

#define LED0 BIT0 // led vermelho
#define LED1 BIT6 // led verde

unsigned int valor_ADC = 0;

void main(void){

	// desabilita wdt
    WDTCTL = WDTPW + WDTHOLD;

    // configura clock 8MHz
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;

    P1DIR |= LED0 + LED1;

    // SREF_0: referência: VCC e GND
    // ADC10SHT_2: amostra em 16 ciclos
    // ADC10ON: liga o ADC
    // ADC10IE: habilita interrupção do ADC
    ADC10CTL0 |= SREF_0 + ADC10SHT_2 + ADC10ON + ADC10IE;

    // Desativa amostragem multipla
    ADC10CTL0 &= ~(MSC);

    // Canal de entrada no canal 5 (P1.5)
    ADC10CTL1 |= INCH_5;

    // Habilita ADC10 no canal 5 (P1.5)
    ADC10AE0 |= BIT5;

    __enable_interrupt();

    // ENC: habilita conversão
    // ADC10SC: inicia a conversão
    ADC10CTL0 |= ENC + ADC10SC;

    while(1){
    	if(valor_ADC < 0xFF){ // < 255
    		// Apaga os leds
    		P1OUT &= ~(LED1 + LED0);
    	}else if(valor_ADC < 0x1FF){ // < 511
    		// Acende led verde
    		P1OUT &= ~LED0;
    		P1OUT |= LED1;
    	}else if(valor_ADC < 0x2FF){ // < 767
    		// Acende led vermelho
    		P1OUT &= ~LED1;
    		P1OUT |= LED0;
    	}else{
    		// Acende os 2 leds
    		P1OUT |= LED1 + LED0;
    	}

    }
}

#pragma vector = ADC10_VECTOR
__interrupt void int_ADC10(void){

	// Lê o valor do ADC
	valor_ADC = ADC10MEM;

	// Limpa flag interrupção do ADC
	ADC10CTL0 &= ~ADC10IFG;

	// ADC10SC: inicia nova conversão
	ADC10CTL0 |= ADC10SC;

}