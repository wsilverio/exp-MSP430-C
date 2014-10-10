Experimento de "debouncing" com a Launchpad MSP430 Rev 1.5 da Texas Instruments  
  
MCU: MSP430G2553  
Compilado e gravado com 'msp430-gcc' e 'mspdebug'  
  
Linux:  
$ msp430-gcc -mmcu=msp430g2553 -o debouncing.elf debouncing.c  
$ mspdebug rf2500  
(mspdebug) prog debouncing.elf  
(mspdebug) run