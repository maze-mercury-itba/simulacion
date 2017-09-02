#ifndef MOVEMENT_H
#define MOVEMENT_H
#include "../WorldStructures.h"

robot_t getNextHipoteticRobotPosition(robot_t);//BIEN
dpoint_t calculateNextPoint(dpoint_t point, robot_t newRobot);//BIEN
bool arrived2Target();//FALTA HACERLAAA
uint16_t hasCrashedWithWall(dpoint_t previousPoint, robot_t newRobot, map_t myMap, robot_t myRobot);//BIEN
robot_t W_rotateRobot(robot_t robot);

#endif // !MOVEMENT_H

