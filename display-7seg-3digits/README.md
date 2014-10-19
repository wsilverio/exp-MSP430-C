## Contador com display 7 segmentos - 3 digitos 
  
![Esquematico](https://raw.githubusercontent.com/wsilverio/MSP430-C/master/display-7seg-3digits/msp7seg_esquematico.png)  
  
MCU: MSP430G2553 (MSP-EXP430G2 LaunchPad)  

Linux: 
  
* Compilar:  
    ```console
    $ msp430-gcc -mmcu=msp430g2553 -o display-7seg-3digits.elf display-7seg-3digits.c  
    ```
  
* Gravar:  
    ```console
    $ mspdebug rf2500
    (mspdebug) prog display-7seg-3digits.elf
    (mspdebug) run
    ```
