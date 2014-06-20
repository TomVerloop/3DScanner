/*
 * i2c_Stepper_Driver.h
 *
 *  Created on: Jun 19, 2014
 *      Author: tom
 */


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

void stepperSetSpeed(int delay, Stepper* step);

int stepperGetPosition(Stepper* step);

int stepperGetRotation(Stepper* step);

void stepperMoveHome(Stepper* step);

void stepperSetHome(Stepper* step);

void stepperMoveZero(Stepper* step);

void stepperSetZero(Stepper* step);

void stepMotor(int thisStep, Stepper* step);

void stepperStep(int steps, Stepper* step);

void stepperInit(int steps);
