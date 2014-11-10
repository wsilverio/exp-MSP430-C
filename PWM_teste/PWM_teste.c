#include <msp430.h>

#define LED BIT6 // led verde

unsigned int valor_ADC = 0;

void main(void){

	// desabilita wdt
    WDTCTL = WDTPW + WDTHOLD;

    // configura clock 8MHz
    BCSCTL1 = CALBC1_8MHZ;
    DCOCTL = CALDCO_8MHZ;

    // configura saída PWM
    P1DIR |= LED;
    P1SEL |= LED;

    // configuração do timer A0
    TA0CTL = TASSEL_2 + ID_1 + MC_1; // SMCLK + DIV(2) + UP CCR0
	TACCTL0	|= CCIE; // int. por comparação
	TACCTL1 |= OUTMOD_6; // PWM toggle/set
	TACCR0 = 1023; // período: valor máx ADC
	TACCR1 = 0; // reset

	// configuração do ADC10

	// SREF_0: referência: VCC e GND
    // ADC10SHT_2: amostra em 16 ciclos
    // ADC10ON: liga o ADC
    // ADC10IE: habilita interrupção do ADC
    ADC10CTL0 |= SREF_0 + ADC10SHT_2 + ADC10ON + ADC10IE;

    // desativa amostragem multipla
    ADC10CTL0 &= ~MSC;

    // canal de entrada no canal 5 (P1.5)
    ADC10CTL1 |= INCH_5;

    // habilita ADC10 no canal 5 (P1.5)
    ADC10AE0 |= BIT5;

    __enable_interrupt();

    // ENC: habilita conversão
    // ADC10SC: inicia a conversão
    ADC10CTL0 |= ENC + ADC10SC;
	
	while(1); // loop
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void int_timer_A(void){

	// atualiza o tempo em nivel alto
	TACCR1 = valor_ADC;
	
	// limpa a flag do int. timer
	TACCTL0 &= ~CCIFG;
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