///--------------------------------------------------------------------------///
///														 					 ///
///	@author: Tom Verloop <Tom_Verloop@live.nl>			 					 ///
///	@Version: 1.0										 					 ///
///														 					 ///
///														 					 ///
///														 					 ///
///--------------------------------------------------------------------------///

#include "LPC11xx.h"
#include "sw_uart.h"
#include "i2c_Stepper_driver.h"
#include "timer.h"
unsigned int x = 1000;
unsigned int y = 1000;
unsigned int z = 1000;

#define STEPS_PER_REVOLUTION 2048

int main(void)
{

uart_init();
	timer_init();
while (1)
{
	unsigned int data = -1;
	while (!uart_rend(0x02))
	{
		delay(1000);
	}
	if ( uart_receive_blocking() == 0x03)
	{
		while (data != 0x06)
		{
			//data = uart_receive_blocking();
			char data = x / 2;
			uart_rend(data);
			if(x % 2 == 1)
			{
				data += 1;
			}
			uart_rend(data);

			data = y / 2;
			uart_rend(data);
			if(y % 2 == 1)
			{
				data += 1;
			}
			uart_rend(data);

			data = z / 2;
			uart_rend(data);
			if(z % 2 == 1)
			{
				data += 1;
			}
			uart_rend(data);
		}
	}
}

	stepperInit(64);

	while (1)
	{
		stepperStep(-STEPS_PER_REVOLUTION, &step1);
		stepperStep(STEPS_PER_REVOLUTION, &step1);
	}
	return 0;

	return 0;
}
