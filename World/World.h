#ifndef WORLD_H
#define WORLD_H
#include <stdint.h>
#include <stdbool.h>

typedef struct 
{
	double width, depth, height;	//agregar toda la info necesaria de la forma
	double direction;				//Direccion hacia la que apunta la parte frontal del Robot.(es un angulo en radianes donde el 0� es el eje y)
	double velocity;				//[velocity]= cm/ticks (son las unidades de la veocidad).
	Point_s position;
}Robot_s;

typedef struct
{
	uint16_t x;
	uint16_t y;
}Point_s;

typedef struct {
	Point_s start, end;
	double inclination;
	int8_t thickness;
	Wall_s * previousWall;	//Wall que conecta con el punto start de esta Wall, en caso de q no conecte con ninguna Wall se deja esta variable en NULL
	Wall_s * nextWall;		//Analogo a previousWall.
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

void * W_getRobotPosition(void);//PUSE QUE AHORA DEVUELVE UN VOID * YA QUE LA DIRECCION DEL ROBOT NO SE ENCUENTRA MAS EN POINT_S, SINO
								//EN ROBOT_S, ENTONCES ESTA FUNCION DEVUELVE UN ARREGLO DE PUNTEROS CON UN POINT_S Y UN DOUBLE.

#endif //WORLD_H