#ifndef WORLD_H
#define WORLD_H
#include <cstdint>
#include "../vector.h"

#define AMOUNT_OF_SENSORS	10	//puse un numero random
typedef enum {NOTHING_HAPPENED, CRASHED, ARRIVED_2_TARGET}robotState_t;

typedef struct {
	dpoint_t positionOnRobot;//respecto de la posicion (0,0) del robot
	double angle;//Respecto de la direccion del robot
}sensor_t;

typedef struct 
{
	uint16_t amountOfPoints;
	dpoint_t * robotPoints;
	double direction;				//Direccion hacia la que apunta la parte frontal del Robot.(es un angulo en radianes donde el 0° es el eje y)
	double rotation;
	double velocity;				//[velocity]= cm/ticks (son las unidades de la veocidad).
	sensor_t sensorArray[AMOUNT_OF_SENSORS]; //Areglo de sensores
	dpoint_t position;
}robot_t;

typedef struct
{
	uint16_t nWalls;	//cantidad de elementos en el arreglo wall
	dvector_t * walls;	//arreglo de vectores que indican donde esta cada pared
	dpoint_t target;
}map_t;

typedef struct
{
	double angle;
	double distance;
}sensData_t;

typedef struct 
{
	double angle;
	dpoint_t position;
}position_t;

int16_t W_Init(map_t * mapInfo);	//Set map
robotState_t W_Update(void);				//Avanzar la simulacion. devuelve codigo de error, o de si choco o no, o ambos
void W_setRobotConfiguration(robot_t * _myRobot);

sensData_t W_getSensorData(uint16_t sensorID);	//despues el sensor se fija que significa esto para este sensor
bool W_configureRobot(double _direction, double _velocity,double _rotation);
void W_rotateRobot(double angle, dpoint_t pointInRobot);

position_t W_getRobotPosition(void);
dpoint_t absolutePoint(dpoint_t point);

#endif //WORLD_H