// CAMBIOS:
// cambiando estructura robot. Hay que cambiar myRobot y todos los lugares donde se la usa
// funcion configureRobot y setRobotConfiguration (CAMBIARLAS EN BASE A ESTO)
// get next hipothetic position tiene que considerar LOS NUEVOS PARAMETROS 
// AVISAR que se estaria cambiando el prototipo de la funcion W_rotateRobot !!!!
// chequear toda la funcion rotateRobot. VER que cosas quedan y que cosas se borran de esa funcion
// en update habria que asignarle todo el nuevo robot a myRobot
// Ver donde guardar los nuevos puntos hipoteticos de todo el robot 
// Ver donde comparar en crashed with wall los nuevos hipoteticos puntos, con los de myRobot, para ver si se los asigna o no

#include "World.h"
#include <cmath>
#include "Calculus\Calculus.h"
#include "Movement\Movement.h"
#include "SensorCalculus\SensorCalculus.h"


#define W_PI		3.14159265358979323846
#define MAX_HIPO	1000

typedef unsigned int uint;
static map_t myMap;
static robot_t myRobot;

int16_t W_Init(map_t * mapInfo)
{
	myMap = *mapInfo;
	return false;
}

void W_setRobotConfiguration(robot_t * _myRobot) //VER QUE METERLE A ESTA FUNCION. SI INICIALIZAR TODO 
{
	myRobot = *_myRobot;
	myRobot.R_velocity = 0;
	return;
}

bool W_configureRobot(/*double dAngle,*/ double dVelocity, /*dpoint_t rPoint,*/ double rVelocity) //FUNCION NUEVA!!!!!!!!!!!!!
{
	//myRobot.D_angle = dAngle;
	myRobot.D_velocity = dVelocity;
	//myRobot.R_point = rPoint;
	myRobot.R_velocity = rVelocity;  
	return true;
}

position_t W_getRobotPosition(void) //Devuelve la esquina superior izquierda del robot
{
	position_t answer;
	answer.angle = myRobot.D_angle;
	answer.position = myRobot.position;
	return answer;
}

robotState_t W_Update(void)
{
	robot_t newRobot = getNextHipoteticRobotPosition(myRobot);
	if (!hasCrashedWithWall(myRobot.position, newRobot,myMap, myRobot))
	{
		myRobot = newRobot;
		if (arrived2Target())
			return ARRIVED_2_TARGET;
		return NOTHING_HAPPENED;
	}
	else
		return CRASHED;
}

sensData_t W_getSensorData(uint16_t sensorID)
{
	sensData_t answer;
	answer.angle = myRobot.D_angle + getAngle(sensorID,myRobot);
	answer.distance = getMinDistance(answer.angle, sensorID, myMap, myRobot);
	return answer;
}

dpoint_t W_absolutePoint(dpoint_t point)
{
	return absolutePointWRobot(point,myRobot);
}
