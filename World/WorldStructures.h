#ifndef WORLDSTRUCTURES_H
#define WORLDSTRUCTURES_H

#include "../vector.h"

#define AMOUNT_OF_SENSORS	4	//puse un numero random
typedef enum { NOTHING_HAPPENED, CRASHED, ARRIVED_2_TARGET }robotState_t;

typedef struct {
	dpoint_t positionOnRobot;//respecto de la posicion (0,0) del robot
	double angle;//Respecto de la direccion del robot
}sensor_t;

typedef struct
{
	dpoint_t position;
	uint16_t amountOfPoints;
	dpoint_t * robotPoints;
	double D_angle;		// Direccion hacia la que apunta la parte frontal del robot (es un angulo en radianes donde el 0° es el eje y)
	double D_velocity;	// [velocity]= cm/tick (son las unidades de la veocidad).
	dpoint_t R_point;	// Punto del robot respecto al cual gira
	double R_velocity;	// Velocidad angular [] = (angulo en rad/tick
	sensor_t sensorArray[AMOUNT_OF_SENSORS]; //Arreglo de sensores
}robot_t;

typedef struct 
{
	double amountOfSides;
	dvector_t * side;
}area_t;

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

#endif // !WORLDSTRUCTURES_H
