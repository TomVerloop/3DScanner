///--------------------------------------------------------------------------///
///														 					 ///
///	@author: Tom Verloop <Tom_Verloop@live.nl>			 					 ///
///	@Version: 1.0										 					 ///
///														 					 ///
///														 					 ///
///														 					 ///
///--------------------------------------------------------------------------///


#include "LPC11xx.h"

int main(void) {
	unsigned char x = 0x01;
    //SET UP UART (sec. 13.2 in datasheet "BASIC CONFIGURATION")
    LPC_IOCON->PIO1_7         |= 0x01;       //configure UART TXD pin (sec. 7.4.41)
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);    //enable clock to UART (sec. 3.5.14)
    LPC_SYSCON->UARTCLKDIV    |= x;       //0x9B will give approx. 19.2K baud signal (sec. 3.5.16)
    LPC_UART->FCR             |= 0x01;       //enable UART FIFOs (necessary for operation) (sec. 13.5.6)
    LPC_UART->LCR             |= 0x03;       //set for 8 bit data width (sec. 13.5.7)
    LPC_UART->TER             |= 0x80;       //transmit enable (sec. 13.5.16)

    //unsigned int i = 0;

    while(1){                    //infinite loop

         LPC_UART->THR |= x;              //transmit data (sec. 13.5.2)
         x++;

         LPC_SYSCON->UARTCLKDIV    &= ~0xFF;
         LPC_SYSCON->UARTCLKDIV    |= x;       //0x9B will give approx. 19.2K baud signal (sec. 3.5.16)
    }
    return 0 ;
}
