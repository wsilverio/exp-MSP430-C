#### ADC10 teste ####

CPU: MSP430G2553 [(MSP-EXP430G2 LaunchPad)](http://www.ti.com/ww/en/launchpad/launchpads-msp430-msp-exp430g2.html)


**Potenciômetro:**
  * Pin 1: VCC
  * Pin 2: P1.5
  * Pin 3: GND

**ADC10MEM:** GND = 0 --- VCC = 1023

**Funcionamento:**
* ADC10MEM < 255:
  * apaga os dois LEDs
* 255 < ADC10MEM < 511:
  * acende somente o LED verde (P1.6)
* 511 < ADC10MEM < 767:
  * acende somente o LED vermelho (P1.0)
* 767 < ADC10MEM:
 * acende os dois LEDs
