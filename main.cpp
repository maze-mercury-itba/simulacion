
#include ".\World\World.h"
#include ".\grafica\EventGenerator.h"
#include ".\grafica\Graphic.h"
//extern "C" {
#include ".\senact\senact.h"
#include ".\Intelligence\Intelligence.h"
//}
#include <allegro5\allegro.h>
#include ".\FileHandler\mapFile.h"
extern "C" {
#include ".\FileHandler\FileHandler.h"
}

enum robotData {WIDTH, HEIGHT, SHAPE, N_SENS};

#define N_WALLS 4

int main(void)
{
	if (!al_init())
		return EXIT_FAILURE;
	map_t map;
	if (readMap("mapa", map) == false)
		return EXIT_FAILURE;

	//if (S_Init("robot.txt") != F_OK)
		//return EXIT_FAILURE;


	robot_t r;
	r.velocity = 0;
	//r.width = 50;
	//r.height = 50;
	r.robotPoints = new dpoint_t[12];
	r.amountOfPoints = 12;
	r.robotPoints[0].x = 0;
	r.robotPoints[0].y = 0;
	r.robotPoints[1].x = 50;
	r.robotPoints[1].y = 0;
	r.robotPoints[2].x = 0;
	r.robotPoints[2].y = 50;
	r.robotPoints[3].x = 50;
	r.robotPoints[3].y = 50;
	r.robotPoints[4].x = 0;
	r.robotPoints[4].y = 15;
	r.robotPoints[5].x = 0;
	r.robotPoints[5].y = 30;
	r.robotPoints[6].x = 15;
	r.robotPoints[6].y = 0;
	r.robotPoints[7].x = 30;
	r.robotPoints[7].y = 0;
	r.robotPoints[8].x = 50;
	r.robotPoints[8].y = 15;
	r.robotPoints[9].x = 50;
	r.robotPoints[9].y = 30;
	r.robotPoints[10].x = 15;
	r.robotPoints[10].y = 50;
	r.robotPoints[11].x = 30;
	r.robotPoints[11].y = 50;

	r.position.x = 270;
	r.position.y = 270;

	//r.width = F_getBasicInfo(WIDTH);
	//r.height = F_getBasicInfo(HEIGHT);
	uint16_t nSens = S_getAmountSen();

	for (unsigned int i = 0; i < nSens; i++) {
		r.sensorArray[i].positionOnRobot.x = F_getSensorXPos(i);
		r.sensorArray[i].positionOnRobot.y = F_getSensorYPos(i);
		r.sensorArray[i].angle = F_getSensorAngle(i);
	}
	

	uipoint_t rSize = {50, 50};
	const char robotPath[] = "grafica/robot.png";

	Graphic g(&robotPath[0], rSize, map);
	g.drawBackground();
	g.showChanges();

	W_Init(&map);
	I_Init(0);
	W_setRobotConfiguration(&r);
	S_setActuatorError(0, NULL);
	S_setSensorError(0, NULL);
	EventGenerator e(g.getDisplay());
	uint16_t ev = NO_EVENT;
	uint16_t worldState = NOTHING_HAPPENED;

	//Orden de updates: mundo, sensores y actuadores, inteligencia. Es importante.
	do {
		ev = e.getNextEvent();

		switch (ev) {
		case SIMULATION_TIMEOUT:
			worldState = W_Update();
			S_Update();	//en estas dos funcs habria que verificar el error
			I_Update();
			
			switch (worldState) {
			case CRASHED:
				g.drawBackground();
				dpoint_t centerPoint;
				centerPoint.x = 25;
				centerPoint.y = 25;
				g.drawRobot(absolutePoint(centerPoint), W_getRobotPosition().angle);
				g.showChanges();
				//ev = EXIT;
				//g.showLoseMsg();
				break;

			case ARRIVED_2_TARGET:
				ev = EXIT;
				g.showWinMsg();
				break;
			}
			break;

		case FRAME_TIMEOUT:
			g.drawBackground();
			dpoint_t centerPoint;
			centerPoint.x = 25;
			centerPoint.y = 25;
			g.drawRobot(absolutePoint(centerPoint), W_getRobotPosition().angle);

			for (unsigned int i = 0; i<nSens; i++)
				g.drawSensorInfo(r.sensorArray[i], S_getStateValue(i)); // aca mostras lo que te devuelve sen&act de alguna manera
			S_getAmountAct(); 
			g.showChanges();
			break;


		default:
			if (ev != NO_EVENT)
				I_Drive(&ev); //si el evento no es para mi, es para la inteligencia
			break;
		}

	} while (ev != EXIT);

	return EXIT_SUCCESS;
}