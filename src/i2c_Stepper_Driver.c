/*
 * i2c_Stepper_Driver.c
 *
 *  Created on: Jun 19, 2014
 *      Author: tom
 */

#include "MCP23017.h"
#include "i2c_Stepper_Driver.h"
#include <stdlib.h>


typedef struct
{
	int Register;
	int shiftNo;
	int stepperPosition; // The current position (in steps) relative to 'Home'
	int stepperStepNumber; // The current position (in steps) relative to 0°
	int stepperStepDelay; // Delay in CPU ticks between individual steps
	int stepperStepsPerRotation;  // Number of steps in a full 360° rotation
} Stepper;

Stepper step1;
Stepper step2;
Stepper step3;

char RegA;
char RegB;


void stepperSetSpeed(int delay, Stepper* step)
{
	step->stepperStepDelay = delay * 100;
}

int stepperGetPosition(Stepper* step)
{
	return step->stepperPosition;
}

int stepperGetRotation(Stepper* step)
{
	return step->stepperStepNumber;
}

void stepperMoveHome(Stepper* step)
{
	stepperStep(step->stepperPosition * -1, step);
}

void stepperSetHome(Stepper* step)
{
	step->stepperPosition = 0;
}

void stepperMoveZero(Stepper* step)
{
	if (!step->stepperStepNumber)
	{
		stepperStep(step->stepperStepsPerRotation - step->stepperStepNumber,
				step);
	}
}

void stepperSetZero(Stepper* step)
{
	step->stepperStepNumber = 0;
}

void stepMotor(int thisStep, Stepper* step)
{
	char regvalue = 0xF;
	if (step->Register == 0x12)
	{
		RegA &= ~(regvalue << step->shiftNo);
		regvalue = RegA;
	}
	else
	{
		RegB &= ~(regvalue << step->shiftNo);
		regvalue = RegA;
	}
	switch (thisStep)
	{
	case 0: // 1010
		regvalue |= (0x0A << step->shiftNo);
		MCP23017_writereg(step->Register, regvalue);
		break;
	case 1: // 0110

		regvalue |= (0x06 << step->shiftNo);
		MCP23017_writereg(step->Register, regvalue);
		break;
	case 2: // 0101

		regvalue |= (0x05 << step->shiftNo);
		MCP23017_writereg(step->Register, regvalue);
		break;
	case 3: // 1001

		regvalue |= (0x09 << step->shiftNo);
		MCP23017_writereg(step->Register, regvalue);
		break;
	}
	if (step->Register == 0x12)
	{
		RegA = regvalue;
	}
	else
	{
		RegB = regvalue;
	}
}

void stepperStep(int steps, Stepper* step)
{
	int stepsLeft = abs(steps);          // Force number to be positive

	while (stepsLeft > 0)
	{
		// Wait x ticks between individual steps
		delay(step->stepperStepDelay);

		// Increment or decrement step counters (depending on direction)
		if (steps > 0)
		{
			step->stepperPosition++;        // Increment global position counter
			step->stepperStepNumber++;      // Increment single rotation counter
			if (step->stepperStepNumber == step->stepperStepsPerRotation)
			{
				step->stepperStepNumber = 0;
			}
		}
		else
		{
			step->stepperPosition--;        // Decrement global position counter
			if (step->stepperStepNumber == 0)
			{
				step->stepperStepNumber = step->stepperStepsPerRotation;
			}
			step->stepperStepNumber--;      // Decrement single rotation counter
		}

		// Decrement number of remaining steps
		stepsLeft--;

		// Step the motor one step
		stepMotor(step->stepperStepNumber % 4, step);
	}
}

void stepperInit(int steps)
{

	char RegA = 0x00;
	char RegB = 0x00;
	MCP23017_writereg(0x00, 0x00);
	MCP23017_writereg(0x01, 0x00);
	MCP23017_writereg(0x12, RegA);
	MCP23017_writereg(0x13, RegB);
	step1.Register = 0x13;
	step1.shiftNo = 0;
	step1.stepperStepsPerRotation = steps;
	step1.stepperPosition = 0;
	step1.stepperStepNumber = 0;
	step1.stepperStepDelay = 100 * 100;
	step2.Register = 0x13;
	step2.shiftNo = 4;
	step2.stepperStepsPerRotation = steps;
	step2.stepperPosition = 0;
	step2.stepperStepNumber = 0;
	step2.stepperStepDelay = 100 * 100;
	step3.Register = 0x12;
	step3.shiftNo = 0;
	step3.stepperPosition = 0;
	step3.stepperStepNumber = 0;
	step3.stepperStepDelay = 100 * 100;
	step3.stepperStepsPerRotation = steps;
	stepperSetSpeed(100, &step1);
	stepperSetSpeed(100, &step2);
	stepperSetSpeed(100, &step3);
}
