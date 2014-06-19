///--------------------------------------------------------------------------///
///														 					 ///
///	@author: Tom Verloop <Tom_Verloop@live.nl>			 					 ///
///	@Version: 1.0										 					 ///
///														 					 ///
///														 					 ///
///														 					 ///
///--------------------------------------------------------------------------///

#include "LPC11xx.h"
#include "pins.h"
#include "sw_uart.h"
#include "MCP23017.h"
//#include <stdlib.h>

#define STEPS_PER_REVOLUTION 2048

typedef struct
{
	int Register;
	int shiftNo;
	int stepperPosition; // The current position (in steps) relative to 'Home'
	int stepperStepNumber; // The current position (in steps) relative to 0°
	int stepperStepDelay; // Delay in CPU ticks between individual steps
	int stepperStepsPerRotation;  // Number of steps in a full 360° rotation
	} Stepper;


	void stepperMoveHome();
	void stepperStep(int steps,Stepper  step);


Stepper step1;
Stepper step2;
Stepper step3;

void stepperSetSpeed(int delay, Stepper  step)
{
	step.stepperStepDelay = delay * 100;
}

int stepperGetPosition(Stepper  step)
{
	return step.stepperPosition;
}

int stepperGetRotation(Stepper  step)
{
	return step.stepperStepNumber;
}

void stepperMoveHome(Stepper  step)
{
	stepperStep(step.stepperPosition * -1,step);
}

void stepperSetHome(Stepper  step)
{
	step.stepperPosition = 0;
}

void stepperMoveZero(Stepper  step)
{
	if (!step.stepperStepNumber)
	{
		stepperStep(step.stepperStepsPerRotation - step.stepperStepNumber,step);
	}
}

void stepperSetZero(Stepper step)
{
	step.stepperStepNumber = 0;
}

void stepMotor(int thisStep, Stepper  step)
{
	switch (thisStep)
	{
	case 0: // 1010
		MCP23017_writereg(step.Register, 0x0A << step.shiftNo);
		break;
	case 1: // 0110
		MCP23017_writereg(step.Register, 0x06 << step.shiftNo);
		break;
	case 2: // 0101
		MCP23017_writereg(step.Register, 0x05 << step.shiftNo);
		break;
	case 3: // 1001
		MCP23017_writereg(step.Register, 0x09 << step.shiftNo);
		break;
	}
}

void stepperStep(int steps, Stepper  step)
{
	int stepsLeft = abs(steps);          // Force number to be positive

	while (stepsLeft > 0)
	{
		// Wait x ticks between individual steps
		delay(step.stepperStepDelay);

		// Increment or decrement step counters (depending on direction)
		if (steps > 0)
		{
			step.stepperPosition++;         // Increment global position counter
			step.stepperStepNumber++;       // Increment single rotation counter
			if (step.stepperStepNumber == step.stepperStepsPerRotation)
			{
				step.stepperStepNumber = 0;
			}
		}
		else
		{
			step.stepperPosition--;         // Decrement global position counter
			if (step.stepperStepNumber == 0)
			{
				step.stepperStepNumber = step.stepperStepsPerRotation;
			}
			step.stepperStepNumber--;       // Decrement single rotation counter
		}

		// Decrement number of remaining steps
		stepsLeft--;

		// Step the motor one step
		stepMotor(step.stepperStepNumber % 4,step);
	}
}

void stepperInit(int steps)
{
	// Setup motor control pins

	MCP23017_writereg(0x00, 0x00);
	MCP23017_writereg(0x01, 0x00);
	MCP23017_writereg(0x12, 0x00);
	MCP23017_writereg(0x13, 0x00);
	// Initialise 16-bit timer 0 which is used for delays
	//timer_init();
	step1.Register = 0x13;
	step1.shiftNo = 0;
	step1.stepperStepsPerRotation = steps;
	step1.stepperPosition = 0; // The current position (in steps) relative to 'Home'
	step1.stepperStepNumber = 0; // The current position (in steps) relative to 0°
	step1.stepperStepDelay = 100 * 1000; // Delay in CPU ticks between individual steps
	step2.Register = 0x13;
	step2.shiftNo = 4;
	step2.stepperStepsPerRotation = steps;
	step2.stepperPosition = 0; // The current position (in steps) relative to 'Home'
		step2.stepperStepNumber = 0; // The current position (in steps) relative to 0°
		step2.stepperStepDelay = 100 * 1000; // Delay in CPU ticks between individual steps
	step3.Register = 0x12;
	step3.shiftNo = 0;
	step3.stepperPosition = 0; // The current position (in steps) relative to 'Home'
		step3.stepperStepNumber = 0; // The current position (in steps) relative to 0°
		step3.stepperStepDelay = 100 * 1000; // Delay in CPU ticks between individual steps
		step3.stepperStepsPerRotation = steps;

	// Set the default speed (2 rotations per second)
	stepperSetSpeed(120, step1);
	stepperSetSpeed(120, step2);
	stepperSetSpeed(120, step3);
}

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
		stepperStep(STEPS_PER_REVOLUTION,step1);
	}
	return 0;

	return 0;
}
