#include <Windows.h>
#include <iostream>
#include <cstdio>
#include ".\World\World.h"
#include ".\grafica\EventGenerator.h"
#include ".\grafica\Graphic.h"
//extern "C"
//{
#include ".\senact\senact.h"
#include ".\Intelligence\Intelligence.h"
//}
#include <allegro5\allegro.h>
#include ".\FileHandler\mapFile.h"
extern "C" {
#include ".\FileHandler\FileHandler.h"
#include "./TFD/tinyfiledialogs.h"
}


enum robotData {WIDTH, HEIGHT, SHAPE, N_SENS};

#define N_WALLS 4

int main(void)
{
	ShowWindow(GetConsoleWindow(), SW_HIDE); //oculto la consola.
	//tinyfd_messageBox("Bienvenido", "Esta es una prueba beta del modulo\nMundo del Maze Mercurry Itba.", "info", "info", 0);
	//tinyfd_messageBox("Instrucciones", "Las flechas del teclado se usan para mover al robot. La tecla F se usa para fastfoward y la S para slowmotion.\nESC para salir", "ok", "info", 0);
	if (!al_init())
		return EXIT_FAILURE;
	map_t map;
	if (readMap("mapa", map) == false)
		return EXIT_FAILURE;

	//if (S_Init("robot.txt") != F_OK)
		//return EXIT_FAILURE;
	robot_t r;
	r.robotPoints = new dpoint_t[100];
	r.amountOfPoints = 100;
	for (int i = 0; i < 25; i++)
	{
		r.robotPoints[i].x = i*2;
		r.robotPoints[i].y = 0;
		r.robotPoints[i + 25].x = 0;
		r.robotPoints[i + 25].y = i*2;
		r.robotPoints[i + 50].x = i*2;
		r.robotPoints[i + 50].y = 50;
		r.robotPoints[i + 75].x = 50;
		r.robotPoints[i + 75].y = i*2;
	}

	r.R_point.x = 25;
	r.R_point.y = 25;
	r.position.x = 115;
	r.position.y = 290;
	r.D_angle = 0;
	r.D_velocity = 0;
	r.R_velocity = 0;

	r.sensorArray[0].positionOnRobot.x = 25;
	r.sensorArray[0].positionOnRobot.y = 0;
	r.sensorArray[0].angle = 0;
	r.sensorArray[1].positionOnRobot.x = 0;
	r.sensorArray[1].positionOnRobot.y = 25;
	r.sensorArray[1].angle = -3.14195/5.0;
	r.sensorArray[2].positionOnRobot.x = 50;
	r.sensorArray[2].positionOnRobot.y = 25;
	r.sensorArray[2].angle = 3.14195/5.0;

	//r.width = F_getBasicInfo(WIDTH);
	//r.height = F_getBasicInfo(HEIGHT);
	//	uint16_t nSens = S_getAmountSen();

	//for (unsigned int i = 0; i < AMOUNT_OF_SENSORS; i++) {
	//	r.sensorArray[i].positionOnRobot.x = F_getSensorXPos(i);
	//	r.sensorArray[i].positionOnRobot.y = F_getSensorYPos(i);
	//	r.sensorArray[i].angle = F_getSensorAngle(i);
	//}
	

	uipoint_t rSize = {50, 50};
	const char robotPath[] = "grafica/robot.png";

	initTodo(r);

	Graphic g(&robotPath[0], rSize, map);
	g.drawBackground();
	g.showChanges();

	W_Init(&map);
	I_Init(0);
	W_setRobotConfiguration(&r);
	//S_setActuatorError(0, NULL);
	//S_setSensorError(0, NULL);
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
				//g.drawBackground();
				//dpoint_t centerPoint;
				//centerPoint.x = 25;
				//centerPoint.y = 25;
				//g.drawRobot(W_absolutePoint(centerPoint), W_getRobotPosition().angle);
				//g.showChanges();
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
			g.drawRobot(W_absolutePoint(centerPoint), W_getRobotPosition().angle);

			for (unsigned int i = 0; i < AMOUNT_OF_SENSORS; i++) {
				g.drawSensorInfo(r.sensorArray[i], S_getStateValue(i)); //aca mostras lo que te devuelve sen&act de alguna manera
			}
			g.showChanges();
			break;


		default:
			if (ev != NO_EVENT)
				//I_Drive(&ev); //si el evento no es para mi, es para la inteligencia
			break;
		}

	} while (ev != EXIT);

	return EXIT_SUCCESS;
}