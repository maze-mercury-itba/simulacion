#include "Intelligence.h"
#include "IntelligenceResources.h"
#include "../vector.h"
#include "Autos/Auto1.h"
#include "Autos/Auto2.h"
#include "Manual.h"
#include "../senact/senact.h"
#include "../FileHandler/FileHandler.h"
#include <cmath>
#define ESCALA	(50/.15)

extern void startAuto1(const dpoint_t * allCoordinates);
static dpoint_t getSensorPosition(int sensorId);
static void initSensorsData(void);
static dpoint_t getCoordinateFromSensor(int sensorId);
static dpoint_t allCoordinates[SENSORS_AMMOUNT];

uint16_t intelMode;

// hacer variable global MODO
void I_Init(uint16_t mode)
{ //PONER LA VARIABLE GLOBAL MODO=modo;
	intelMode = mode;
	if (mode == MANUAL) {
		I_Drive(0, 0, 0);
	}
}

void I_Update(void) //NO RECIBIR NADA
{
	//HACER UN SWITCH CON MODO 
	initSensorsData();
	switch (intelMode) 
	{
	case MANUAL:
		startManual();
		break;
	default:
		startAuto1(allCoordinates);
	}
	return;
}

static void initSensorsData(void)
{
	int i;
	for (i = 0; i < SENSORS_AMMOUNT; i++)
		allCoordinates[i] = getCoordinateFromSensor(i);
	return;
}

static dpoint_t getCoordinateFromSensor(int sensorId)
{
	double angle = F_getSensorAngle(sensorId);
	dpoint_t positionOnRobot = getSensorPosition(sensorId);
	double distance = S_getStateValue(sensorId)*ESCALA;
	dpoint_t answer;
	answer.x = positionOnRobot.x + sin(angle)*distance;
	answer.y = positionOnRobot.y - cos(angle)*distance;
	return answer;
}

static dpoint_t getSensorPosition(int sensorId)
{
	dpoint_t temp;
	temp.x = F_getSensorXPos(sensorId);
	temp.y = F_getSensorYPos(sensorId);
	return temp;
}