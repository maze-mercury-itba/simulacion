
//La info de las posiciones y angulo de sensores, va a estar en un .txt (FEDE

#include "Intelligence.h"
#include "IntelligenceResources.h"
#include "../vector.h"
#include "./Autos/Auto1.h"
#include "./Autos/Auto2.h"
#include "../senact/senact.h"
#include <math.h>

//typedef enum { SENSOR_0, SENSOR_1, SENSOR_2, SENSOR_3, SENSOR_4, SENSOR_5, SENSOR_6, SENSOR_7, SENSOR_8, SENSOR_9, SENSOR_10, SENSOR_11 }sensors;

extern void startAuto1(const dpoint_t * allCoordinates);
extern void startAuto2(void);
//extern void startAssisted1(void);
//extern void startAssisted2(void);
//extern void startManual(void);

static void initSensorsData(void);
static dpoint_t getCoordinateFromSensor(int sensorId);
//static void saveReferencedData(dpoint_t* allCoordinates, sensorInfo_s* allInfo);
 


//static sensorInfo_s allInfo[SENSORS_AMMOUNT];		//Tiene que estar creado en algun lugar.
static dpoint_t allCoordinates[SENSORS_AMMOUNT];	//PARA QUE DESPUES USE IAN CON UN FOR RECORRIENDO TODAS LAS COORDENADAS



void I_Update(int16_t _intelligenceType)
{
	initSensorsData();
	//saveReferencedData(allCoordinates, allInfo);
	switch (_intelligenceType) 
	{
	case AUTO1:
		startAuto1(allCoordinates);
		break;
	case AUTO2:
		startAuto2();
		break;
	case ASSISTED1:
		//startAssisted1();
		break;
	case ASSISTED2:
		//startAssisted2();
		break;
	case MANUAL:
		//startManual();
		break;
	default:
		break;
	}
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
	double angle = S_getSensorAngle(sensorId);
	dpoint_t positionOnRobot = S_getSensorPosition(sensorId);
	double distance = S_getStateValue(sensorId);				//VER SI ESTA ES LA FUNCION QUE NECESITAMOS (QUEREMOS LA DISTANCIA)
	dpoint_t answer;
	answer.x = positionOnRobot.x + sin(angle)*distance;
	answer.y = positionOnRobot.y - cos(angle)*distance;
}

/////////////////////////////////////////////////////////////////////////
//static void saveReferencedData(dpoint_t* allCoordinates, sensorInfo_s* allInfo)
//{
//	for (uint16_t i = 0; i < SENSORS_AMMOUNT; i++)
//	{
//		if (allInfo[i].measuredDistance != NULL)		//Si el sensor indicado midio una distancia, guarda la coordenada en allCoordinates
//		{
//			allCoordinates[i].x = allInfo[i].fromX - allInfo[i].measuredDistance * cos(allInfo[i].angle - (3 / 2) * PI);
//			allCoordinates[i].y = allInfo[i].fromY - allInfo[i].measuredDistance * sen(allInfo[i].angle - (3 / 2) * PI);
//		}
//	}
//}
//
