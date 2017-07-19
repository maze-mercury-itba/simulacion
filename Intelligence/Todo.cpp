#include "..\World\World.h"
#include "..\grafica\EventGenerator.h"
#include "..\senact\senact.h"
#include "Intelligence.h"

static uint16_t amMoving;
static robot_t robot;
static map_t map;

#define PI		3.14159
#define SPEED	10
enum dirs { STILL, UP, DOWN, LEFT, RIGHT };

// Sensores
double S_getStateValue(sensor_id) { return 0; }
void * S_getStateSens(sensor_id) { return NULL; }
uint16_t S_getAmountSen(void) { return 0; }

// Actuadores
uint16_t S_setActuatorMovUnproc(uint16_t actuator_id, int8_t actuator_percentage) { return 0; }
uint16_t S_getAmountAct(void) { return 0; }


// Mantenimiento
uint16_t S_Update(void) { W_configureRobot(robot.direction, robot.velocity, robot.rotation); return 0; }

// Configuracion
uint16_t S_setSensorError(sensor_id, void*) { return 0; }
uint16_t S_setActuatorError(sensor_id, void*) { return 0; }

void I_Init(uint16_t mode) { robot.velocity = 0; amMoving = STILL; } //configuracion inicial

void I_Update(void)
{
	if (amMoving == STILL)
		robot.velocity = 0;
	else
		robot.velocity = SPEED;
} // va a usar las funciones getInfo, processData, whatDoIDoNext, y doAction.

int16_t I_setMode(uint16_t mode) { return 0; }



//int16_t W_Init(map_t * mapInfo)
//{
//	map = *mapInfo;
//	return true;
//}
//
//int16_t W_Update(void)
//{
//	switch (amMoving) {
//	case UP:
//		robot.position.coord.y -= SPEED;
//		break;
//
//	case DOWN:
//		robot.position.coord.y += SPEED;
//		break;
//
//	case LEFT:
//		robot.position.coord.x -= SPEED;
//		break;
//
//	case RIGHT:
//		robot.position.coord.x += SPEED;
//		break;
//	}
//
//	if (robot.position.coord.x < 0 || robot.position.coord.y < 0)
//		return GAMEOVER;
//	else if (robot.position.coord.x > map.target.x - 10 && robot.position.coord.x < map.target.y + 10
//		&& robot.position.coord.y > map.target.y - 10 && robot.position.coord.y < map.target.y + 10)
//		return WIN;
//	else
//		return CONTINUE;
//}


void I_Drive(void * event)
{
	uint16_t ev = *((uint16_t *)event);
	dpoint_t centerPoint;
	centerPoint.x = 25;
	centerPoint.y = 25;

	switch (ev) {
	case UP_UP:
		if (amMoving = UP) {
			amMoving = STILL;
		}
		break;

	case DOWN_UP:
		if (amMoving = DOWN) {
			amMoving = STILL;
		}
		break;

	case LEFT_UP:
		if (amMoving = LEFT) {
			amMoving = STILL;
		}
		break;

	case RIGHT_UP:
		if (amMoving = RIGHT) {
			amMoving = STILL;
		}
		break;

	case UP_DOWN:
		amMoving = UP;
		robot.direction = 0;
		break;

	case DOWN_DOWN:
		amMoving = DOWN;
		robot.direction = PI;
		break;

	case LEFT_DOWN:
		amMoving = LEFT;
		robot.direction = 3 * PI / 2;
		break;

	case RIGHT_DOWN:
		amMoving = RIGHT;
		robot.direction = PI / 2;
		break;
	case A_DOWN:
		robot.rotation = robot.rotation + PI / 10;
		break;
	case D_DOWN:
		robot.rotation = robot.rotation - PI / 10;
		break;
	}
}

//
//void W_setRobotConfiguration(robot_t * _myRobot)
//{
//	robot = *_myRobot;
//	robot.position.coord.x = 300;
//	robot.position.coord.y = 400;
//}
//
//position_t W_getRobotPosition(void)
//{
//	return robot.position;
//}