#ifndef WORLD_H
#define WORLD_H
//#include <stdint.h>
//#include <stdbool.h>

#include "vector.h"

enum update_returns {GAMEOVER, CONTINUE, WIN};


//typedef struct 
//{
//    uipoint_t coord;    //x e y
//	double direction;   //angulo en rad
//}position_t;

typedef struct 
{
	double width, depth, height; //agregar toda la info necesaria de la forma
	position_t position;
}robot_t;

typedef struct
{
    uint16_t nWalls;    //cantidad de elementos en el arreglo wall
    uivector_t * wall;  //arreglo de vectores que indican donde esta cada pared
    
    //alguna indicacion del tamano? o hacerlo predeterminado?
    
    uipoint_t target;      //punto adonde tiene que tratar de ir el robot
}map_t;

int16_t W_Init(map_t * mapInfo);   //set map
int16_t W_Update(void);  //avanzar la simulacion. devuelve codigo de error, o de si choco o no, o ambos
void W_setRobotConfiguration(robot_t * _myRobot);

#ifndef TEST
//Servicios

typedef struct sensData_t
{
    double angle;
    double distance;
};

sensData_t W_getSensorData(uint16_t sensorID);  //despues el sensor se fija que significa esto para este sensor
bool W_configureRobot(uint16_t _direction, uint16_t _velocity);
#else
void funcion_turbia(uint16_t ev);

#endif
position_t W_getRobotPosition(void);


/*
static Robot_s myRobot;
static Map_s myMap;
*/

#endif //WORLD_H