#ifndef WORLD_H
#define WORLD_H
#include <cstdint>
#include "vector.h"

#define AMOUNT_OF_SENSORS	10	//puse un numero random
typedef enum {NOTHING_HAPPENED, CRASHED, ARRIVED_2_TARGET}RobotState_t;

typedef struct {
	Point_t positionOnRobot;//respecto de la posicion (0,0) del robot
	double angle;//Respecto de la direccion del robot
}Sensor_t;

typedef struct 
{
	double width, depth, height;	//agregar toda la info necesaria de la forma
	double direction;				//Direccion hacia la que apunta la parte frontal del Robot.(es un angulo en radianes donde el 0° es el eje y)
	double velocity;				//[velocity]= cm/ticks (son las unidades de la veocidad).
	Sensor_t sensorArray[AMOUNT_OF_SENSORS]; //Areglo de sensores
	Point_t position;
}Robot_t;

typedef struct {
	Point_t start, end;
	int8_t thickness;
}Wall_t;

typedef struct
{
	uint16_t nWalls;	//cantidad de elementos en el arreglo wall
	Wall_t * walls;	//arreglo de vectores que indican donde esta cada pared
	Point_t target;
}Map_t;

typedef struct
{
	double angle;
	double distance;
}sensData_t;

typedef struct 
{
	double angle;
	Point_t position;
}RobotPosition_t;

int16_t W_Init(Map_t * mapInfo);	//Set map
RobotState_t W_Update(void);				//Avanzar la simulacion. devuelve codigo de error, o de si choco o no, o ambos
void W_setRobotConfiguration(Robot_t * _myRobot);

sensData_t W_getSensorData(uint16_t sensorID);	//despues el sensor se fija que significa esto para este sensor
bool W_configureRobot(uint16_t _direction, uint16_t _velocity);

RobotPosition_t W_getRobotPosition(void);

#endif //WORLD_H