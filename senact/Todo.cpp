#include "..\World\World.h"
#include "..\grafica\EventGenerator.h"
#include "../senact/senact.h"
#include "../FileHandler/FileHandler.h"
#include "../Intelligence/Intelligence.h"

static uint16_t amMoving;
static robot_t robot;
static map_t map;

#define PI		3.14159
#define SPEED	1
#define ROTATION_SPEED (PI/100.0)	
enum dirs { STILL, UP, DOWN, LEFT, RIGHT };

void initTodo(robot_t hello) { robot = hello; return; }

// Sensores
double S_getStateValue(int sensorID) { return W_getSensorData(sensorID).distance; }
double F_getSensorXPos(uint16_t sensornumber) { return robot.sensorArray[sensornumber].positionOnRobot.x; }
double F_getSensorYPos(uint16_t sensornumber) { return robot.sensorArray[sensornumber].positionOnRobot.y; }
double F_getSensorAngle(uint16_t sensornumber) { return robot.sensorArray[sensornumber].angle; }
