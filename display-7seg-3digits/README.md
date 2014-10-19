## Contador com display 7 segmentos - 3 digitos ##  
  
![Esquematico](raw.githubusercontent.com/wsilverio/MSP430-C/master/display-7seg-3digits/msp7seg_esquematico.png)  
  
MCU: MSP430G2553 (MSP-EXP430G2 LaunchPad)  
  
* Compilar:  
<$ msp430-gcc -mmcu=msp430g2553 -o display-7seg-3digits.elf display-7seg-3digits.c>  
  
* Gravar:  
<$ mspdebug rf2500  
(mspdebug) prog display-7seg-3digits.elf  
(mspdebug) run>  
