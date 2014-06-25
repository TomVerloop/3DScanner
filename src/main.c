///--------------------------------------------------------------------------///
///														 					 ///
///	@author: Rene Keijzer <rene.keijzer@casema.nl		 					 ///
///	@Version: 1.0										 					 ///
///														 					 ///
///														 					 ///
///														 					 ///
///--------------------------------------------------------------------------///

//#include "LPC11xx.h"
#include "sw_uart.h"
#include "i2c_Stepper_driver.h"
#include "adconv.h"
#include "timer.h"
#include "pins.h"
#include <stdlib.h>

int horizontalStepper, verticalStepper;
unsigned int x = 0;

void sendData(int x, int y, int z);
#define STEPS_PER_REVOLUTION 2048

int main(void)
{
	adc_init();
	uart_init();
	timer_init();
	stepperInit(64);
	for (;;)
	{
		unsigned int data = -1;
		while ((data =  uart_receive_non_blocking()) == -1)
		{
			uart_send(0x02);
			delay(1000);
		}

		if (uart_receive_blocking() == 0x03)
		{

				for (verticalStepper = 0; verticalStepper < 256;
						verticalStepper++)
				{
					//Upping the vertical stepper
					for (horizontalStepper = 0; horizontalStepper < 256;
							horizontalStepper++)
					{
						//Scanning horizontal
						if (adc_ready())
						{
							int i;
							for (i = 0; i < 3; i++)
							{
								while (!adc_ready())
									;
								x += adc_getVal();
							}
							x = x / 3;
							sendData(x, horizontalStepper,
									verticalStepper);
						}
						stepperStep(8, &step1);
					}
					//step1.stepperPosition = 0;
					stepperStep(8, &step3);
				}
				//Done!
		}
	}
	return 0;
}

void sendData(int x, int y, int z)
{
	char data = x/8;
	uart_send(data);
}
