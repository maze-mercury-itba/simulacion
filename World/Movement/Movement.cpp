#include "./Movement.h"
#include "../Calculus/Calculus.h"
#include <cmath>
#define W_PI		3.14159265358979323846
#define MAX_HIPO	1000

typedef unsigned int uint;

uint16_t hasCrashedWithWall(dpoint_t previousPoint, robot_t newRobot, map_t myMap, robot_t myRobot)
{
	uint16_t nearWall = 0;
	double distance = MAX_HIPO;
	dvector_t wall2Check;
	for (uint i = 0; i < myMap.nWalls; i++)
	{
		wall2Check = myMap.walls[i];
		for (uint u = 0; u < myRobot.amountOfPoints; u++)
		{
			dpoint_t tempPoint = calculateNextPoint(myRobot.robotPoints[u], newRobot);
			dpoint_t prevPoint = absolutePointWRobot(myRobot.robotPoints[u],myRobot);
			if (doIntersect(prevPoint, tempPoint, wall2Check.start, wall2Check.end))
			{
				dpoint_t newPoint;
				if (ifVertical(prevPoint, tempPoint) && ifVertical(wall2Check.start, wall2Check.end))
					newPoint = tempPoint;
				else
					newPoint = getIntersectionPoint(prevPoint, tempPoint, myMap.walls[i]);
				if (getDistance(newPoint, prevPoint) < distance)
				{
					nearWall = i + 1;
					distance = getDistance(newPoint, prevPoint);
				}
			}
		}
	}
	if (distance == MAX_HIPO)
		nearWall = 0;
	return nearWall;
}

robot_t getNextHipoteticRobotPosition(robot_t robot)				//terminar de ver esta parte para que se haga update de desplazamiento y rotacion
{
	if (robot.D_velocity == 0 && robot.R_velocity == 0)
		return robot;
	double _direction = (W_PI / 2) - robot.D_angle;						// Se pasa el angulo que esta con respecto del eje y 
	dpoint_t _nextPoint;													// hacia angulos con respecto al eje x para simplificar las cuentas.
	_nextPoint.y = robot.position.y - sin(_direction) * robot.D_velocity;
	_nextPoint.x = robot.position.x + cos(_direction) * robot.D_velocity;
	robot.position = _nextPoint;
	robot = W_rotateRobot(robot);
	return robot;
}

dpoint_t calculateNextPoint(dpoint_t point, robot_t newRobot)
{
	dpoint_t answer;
	answer.x = newRobot.position.x - point.y * (sin(newRobot.D_angle)) + (point.x * cos(newRobot.D_angle));
	answer.y = newRobot.position.y + point.y * (cos(newRobot.D_angle)) + (point.x * sin(newRobot.D_angle));
	return answer;
}

bool arrived2Target()
{
	/*dpoint_t answer;
	answer.x = myRobot.position.x + myMap.target.y * (sin(myRobot.D_angle)) - (myMap.target.x * cos(myRobot.D_angle));
	answer.y = myRobot.position.y - myMap.target.y * (cos(myRobot.D_angle)) - (myMap.target.x * sin(myRobot.D_angle));*/
	return false;
}

robot_t W_rotateRobot(robot_t robot)
{
	if (robot.R_velocity == 0)
		return robot;

	dpoint_t realPoint = absolutePointWRobot(robot.R_point, robot);
	double deltaY, deltaX;
	deltaY = realPoint.y - robot.position.y;
	deltaX = realPoint.x - robot.position.x;
	double c = sqrt(pow(deltaY, 2) + pow(deltaX, 2));
	double tita;
	if (deltaY >= 0 && deltaX > 0)
		tita = -((W_PI / 2) - absoluteValue(atan((deltaY) / (deltaX))));
	else if (deltaY <= 0 && deltaX > 0)
		tita = -((W_PI / 2) + absoluteValue(atan((deltaY) / (deltaX)))); //deberia ir un + en el arcotangente, pero como el arco tg me da un angulo negativo le pongo un -
	else if (deltaY <= 0 && deltaX < 0)
		tita = (W_PI / 2) + absoluteValue(atan((deltaY) / (deltaX)));
	else if (deltaY >= 0 && deltaX < 0)
		tita = (W_PI / 2) - absoluteValue(atan((deltaY) / (deltaX)));  //lo mismo que en el segundo caso con los signos del atan
	else if (deltaX == 0)
		tita = 0;

	double beta = tita + robot.R_velocity;
	robot.position.x = realPoint.x + sin(beta) * c;
	robot.position.y = realPoint.y - cos(beta) * c;
	robot.D_angle += robot.R_velocity;
	return robot;
}

