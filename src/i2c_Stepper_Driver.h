/*
 * i2c_Stepper_Driver.h
 *
 *  Created on: Jun 19, 2014
 *      Author: tom
 */



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
