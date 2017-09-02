#include "SensorCalculus.h"
#include "../Calculus/Calculus.h"
#include <cmath>

#define ALCANCE	1000

double getMinDistance(double _angle, uint16_t sensorID, map_t myMap, robot_t myRobot)
{
	dpoint_t firstPoint = getSensorPointOnMap(sensorID, myRobot);
	dpoint_t secondPoint = getPoint(firstPoint, _angle, ALCANCE);
	dvector_t nearestWall = myMap.walls[0];//inicializacion de nearestWall para que funcione el for
	double distance = ALCANCE;
	for (uint16_t i = 0; i < myMap.nWalls; i++)
	{
		dvector_t wall2Check = myMap.walls[i];
		if (doIntersect(firstPoint, secondPoint, wall2Check.start, wall2Check.end))
		{
			if (getDistance(firstPoint, getIntersectionPoint(firstPoint, secondPoint, wall2Check))<= distance)
			{
			nearestWall = wall2Check;
			distance = getDistance(firstPoint, getIntersectionPoint(firstPoint, secondPoint, wall2Check));
			}
		}
	}
	return getDistance(firstPoint, getIntersectionPoint(firstPoint, secondPoint, nearestWall));
}

double getAngle(uint16_t sensorID, robot_t myRobot)
{
	return myRobot.sensorArray[sensorID].angle;
}

dpoint_t getSensorPointOnMap(uint16_t sensorID, robot_t myRobot)
{	
	return absolutePointWRobot(myRobot.sensorArray[sensorID].positionOnRobot, myRobot);
}