/*
 * adc.c
 *
 *  Created on: June 20, 2014
 *      Author: Rene
 */

#include "LPC11xx.h"

void adc_init()
{
		LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 13);
		LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);

		LPC_IOCON->R_PIO1_0 &= ~(7);
		LPC_IOCON->R_PIO1_0 |= (2);

		LPC_IOCON->R_PIO1_0 &= ~(1 << 7);

		LPC_SYSCON->PDRUNCFG &= ~(1 << 4);

		LPC_ADC->CR &= ~(255);
		LPC_ADC->CR |= 2;

		LPC_ADC->CR &= ~(255 << 8);
		LPC_ADC->CR |= (2 << 8);

		LPC_ADC->CR &= ~(1 << 16);
}

unsigned int adc_getVal()
{
	unsigned int tempvalue = (LPC_ADC->GDR & (1023 << 6));
	return tempvalue >> 6;
}

unsigned char adc_ready()
{
	 LPC_ADC->CR &= ~(7 << 24);
	 LPC_ADC->CR |= (1 << 24);

	 //wait for result
	 while ((LPC_ADC->GDR & (1 << 31)) == 0);
	 return 1;
}
