##### UART - teste serial

Testado com:  
MCU= **MSP430G2553** @8MHz  
Serial: modo assíncrono, @9600bps, 8bits, sem paridade  


Compilar e gravar:
```console
$ msp430-gcc -mmcu=msp430g2553 -o uart_teste.elf UART_teste.c  
$ mspdebug rf2500  
(mspdebug) prog uart_teste.elf  
(mspdebug) <Ctrl+D>
$ ls /dev/ttyACM*
$ [cutecom](http://cutecom.sourceforge.net/)
```
Arquivo ***UART_auto_config_BRx_BRSx.c***:
 > Substituir as constantes CLK e BPS para os valores desejados
