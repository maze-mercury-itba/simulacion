#ifndef SENSORCALCULUS_H
#define SENSORCALCULUS_H
#include "../WorldStructures.h"

double getMinDistance(double _angle, uint16_t sensorID, map_t myMap, robot_t myRobot);
double getAngle(uint16_t sensorID, robot_t myRobot);
dpoint_t getSensorPointOnMap(uint16_t sensorID, robot_t myRobot);


#endif // !SENSORCALCULUS_H
