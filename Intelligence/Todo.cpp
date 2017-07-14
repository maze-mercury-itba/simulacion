#include "../World/World.h"
#include "../grafica/EventGenerator.h"
#include "../senact/senact.h"
#include "Intelligence.h"
extern "C" {
#include "../FileHandler/FileHandler.h"
}


static uint16_t amRotating;
static robot_t robot;
static map_t map;

static unsigned char random;


#define PI		3.14159
#define SPEED	10
enum dirs { STILL, UP, DOWN, LEFT, RIGHT };

// Sensores

int16_t S_Init(char * f) { return F_Startup(f);  };
double S_getStateValue(sensor_id) { return random++/16 ; }
void * S_getStateSens(sensor_id) { return NULL; }
uint16_t S_getAmountSen(void) { return F_getBasicInfo(3); }

// Actuadores
uint16_t S_setActuatorMovUnproc(uint16_t actuator_id, int8_t actuator_percentage) { return 0; }
uint16_t S_getAmountAct(void) { return 0; }


// Mantenimiento
uint16_t S_Update(void) { W_configureRobot(robot.direction, robot.velocity, 0); return 0; }

// Configuracion
uint16_t S_setSensorError(sensor_id, void*) { return 0; }
uint16_t S_setActuatorError(sensor_id, void*) { return 0; }


void I_Init(uint16_t mode) { robot.velocity = 0; amRotating = STILL; } //configuracion inicial

void I_Update(void)
{
	if (amRotating == LEFT)
		robot.direction -= PI / 20;
	else if (amRotating == RIGHT)
		robot.direction += PI / 20;
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

	switch (ev) {
	case UP_UP:
		if (robot.velocity == SPEED) {
			robot.velocity = 0;
		}
		break;

	case DOWN_UP:
		if (robot.velocity == -SPEED) {
			robot.velocity = 0;
		}
		break;

	case LEFT_UP:
		if (amRotating == LEFT) {
			amRotating = STILL;
		}
		break;

	case RIGHT_UP:
		if (amRotating == RIGHT)
			amRotating = STILL;
		break;

	case UP_DOWN:
		robot.velocity = SPEED;
		break;

	case DOWN_DOWN:
		robot.velocity = -SPEED;
		break;

	case LEFT_DOWN:
		amRotating = LEFT;
		break;

	case RIGHT_DOWN:
		amRotating = RIGHT;
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
