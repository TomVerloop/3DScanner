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


#define STEPS_PER_REVOLUTION 2048

int main(void)
{

//uart_init();
	timer_init();
//pin_configure_as_output(0, 7);
//pin_set(0, 7, 0);
//while (1)
//{
//	unsigned int data = -1;
//	while (!uart_rend(0x02))
//	{
//		delay(1000);
//	}
//	if ( uart_receive_blocking() == 0x03)
//	{
//		while (data != 0x06)
//		{
//			data = uart_receive_blocking();
//
//		}
//	}
//}

	stepperInit(64);

	while (1)
	{
		stepperStep(-STEPS_PER_REVOLUTION, &step3);
		stepperStep(STEPS_PER_REVOLUTION, &step3);
	}
	return 0;

	return 0;
}
