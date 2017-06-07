#ifndef WORLD_H
#define WORLD_H
#include <cstdint>

#define AMOUNT_OF_SENSORS	10	//puse un numero random

typedef struct
{
	uint16_t x;
	uint16_t y;
}Point_s;

typedef struct {
	Point_s positionOnRobot;//respecto de la posicion (0,0) del robot
	double angle;//Respecto de la direccion del robot
}Sensor_s;

typedef struct 
{
	double width, depth, height;	//agregar toda la info necesaria de la forma
	double direction;				//Direccion hacia la que apunta la parte frontal del Robot.(es un angulo en radianes donde el 0° es el eje y)
	double velocity;				//[velocity]= cm/ticks (son las unidades de la veocidad).
	Sensor_s sensorArray[AMOUNT_OF_SENSORS]; //Areglo de sensores
	Point_s position;
}Robot_s;

typedef struct {
	Point_s start, end;
	double inclination;
	int8_t thickness;
}Wall_s;

typedef struct
{
	uint16_t nWalls;	//cantidad de elementos en el arreglo wall
	Wall_s * walls;	//arreglo de vectores que indican donde esta cada pared
}Map_s;

typedef struct
{
	double angle;
	double distance;
}sensData_t;

int16_t W_Init(Map_s * mapInfo);	//Set map
int16_t W_Update(void);				//Avanzar la simulacion. devuelve codigo de error, o de si choco o no, o ambos
void W_setRobotConfiguration(Robot_s * _myRobot);

sensData_t W_getSensorData(uint16_t sensorID);	//despues el sensor se fija que significa esto para este sensor
bool W_configureRobot(uint16_t _direction, uint16_t _velocity);

Point_s W_getRobotPosition(void);

#endif //WORLD_H