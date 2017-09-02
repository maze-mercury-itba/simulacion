#ifndef WORLD_H
#define WORLD_H
#include <cstdint>
#include "WorldStructures.h"

int16_t W_Init(map_t * mapInfo);	//Set map
robotState_t W_Update(void);				//Avanzar la simulacion. devuelve codigo de error, o de si choco o no, o ambos
void W_setRobotConfiguration(robot_t * _myRobot);

sensData_t W_getSensorData(uint16_t sensorID);	//despues el sensor se fija que significa esto para este sensor
bool W_configureRobot(/*double dAngle,*/ double dVelocity, /*dpoint_t rPoint, */double rVelocity); //FUNCION NUEVA!!!!!!!!!!!!!

position_t W_getRobotPosition(void);
dpoint_t W_absolutePoint(dpoint_t point);

#endif //WORLD_H