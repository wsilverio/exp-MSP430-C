#### Teste de ***low power mode 4***  
  
  
MCU: MSP430G2553 (MSP-EXP430G2 LaunchPad)  
  
Linux:  
* Compilar:  
```console
$ msp430-gcc -mmcu=msp430g2553 -o lpm4_int.elf lpm4_int.c
```
  
* Gravar:  
```console
    $ mspdebug rf2500  
    (mspdebug) prog lpm4_int.elf  
    (mspdebug) run
```
