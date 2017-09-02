#include "./IntelligenceCalculus.h"
#include "../../senact/senact.h"
#define MOTOR_L		0
#define MOTOR_R		1


void actOverInclination(double inc)
{
	if (inc >= 0)
	{
		if (inc > 1 && inc < 3)
		{
			S_setActuatorMov(MOTOR_L, 20);
			S_setActuatorMov(MOTOR_R, 100);
		}
		else if (inc >= 3)
		{
			S_setActuatorMov(MOTOR_L, 0);
			S_setActuatorMov(MOTOR_R, 100);
			
		}
		else
		{
			S_setActuatorMov(MOTOR_L, 50);
			S_setActuatorMov(MOTOR_R, 100);
		}
	}
	if (inc < 0)
	{
		if (inc < -1 && inc > -3)
		{
			S_setActuatorMov(MOTOR_L, 100);
			S_setActuatorMov(MOTOR_R, -20);
			
		}
		else if (inc <= -3)
		{
			S_setActuatorMov(MOTOR_L, 100);
			S_setActuatorMov(MOTOR_R, -50);
		}
		else
		{
			S_setActuatorMov(MOTOR_L, 100);
			S_setActuatorMov(MOTOR_R, 50);
		}
	}
	return;
}

void goBack(void)
{
	S_setActuatorMov(MOTOR_L, -100);
	S_setActuatorMov(MOTOR_R, -100);
	return;
}

void turnRight(void)
{
	S_setActuatorMov(MOTOR_L, 100);
	S_setActuatorMov(MOTOR_R, -100);
	return;
}

void turnLeft(void)
{
	S_setActuatorMov(MOTOR_L, 30);
	S_setActuatorMov(MOTOR_R, 100);
}

void goSlightlyRight(void)
{
	S_setActuatorMov(MOTOR_L, 100);
	S_setActuatorMov(MOTOR_R, -10);
	return;
}

void moveFoward(void)
{
	S_setActuatorMov(MOTOR_L, 100);
	S_setActuatorMov(MOTOR_R, 100);
}