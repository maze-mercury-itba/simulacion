#include "World.h"
#include "EventGenerator.h"


static uint16_t amMoving;
static robot_t robot;
static map_t map;

#define PI		3.14159
#define SPEED	20
enum dirs { STILL, UP, DOWN, LEFT, RIGHT };


int16_t W_Init(map_t * mapInfo)
{
	map = *mapInfo;
	return true;
}

int16_t W_Update(void)
{
	switch (amMoving) {
	case UP:
		robot.position.coord.y -= SPEED;
		break;

	case DOWN:
		robot.position.coord.y += SPEED;
		break;

	case LEFT:
		robot.position.coord.x -= SPEED;
		break;

	case RIGHT:
		robot.position.coord.x += SPEED;
		break;
	}

	if (robot.position.coord.x < 0 || robot.position.coord.y < 0)
		return GAMEOVER;
	else if (robot.position.coord.x > map.target.x - 10 && robot.position.coord.x < map.target.y + 10
		&& robot.position.coord.y > map.target.y - 10 && robot.position.coord.y < map.target.y + 10)
		return WIN;
	else
		return CONTINUE;
}


void funcion_turbia(uint16_t ev)
{
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
		robot.position.direction = 0;
		break;

	case DOWN_DOWN:
		amMoving = DOWN;
		robot.position.direction = PI;
		break;

	case LEFT_DOWN:
		amMoving = LEFT;
		robot.position.direction = 3 * PI / 2;
		break;

	case RIGHT_DOWN:
		amMoving = RIGHT;
		robot.position.direction = PI / 2;
		break;
	}
}


void W_setRobotConfiguration(robot_t * _myRobot)
{
	robot = *_myRobot;
	robot.position.coord.x = 300;
	robot.position.coord.y = 400;
}

position_t W_getRobotPosition(void)
{
	return robot.position;
}
