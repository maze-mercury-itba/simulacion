#include <Windows.h>
#include <iostream>
#include <cstdio>
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
#include "./TFD/tinyfiledialogs.h"
}

enum robotData {WIDTH, HEIGHT, SHAPE, N_SENS};

#define N_WALLS 4

int main(void)
{
	ShowWindow(GetConsoleWindow(), SW_HIDE); //oculto la consola.
	tinyfd_messageBox("Bienvenido", "Esta es una prueba beta del modulo\nMundo del Maze Mercurry Itba.", "info", "info", 0);
	tinyfd_messageBox("Instrucciones", "Las flechas del teclado se usan para mover al robot. La tecla F se usa para fastfoward y la S para slowmotion.\nESC para salir", "ok", "info", 0);
	tinyfd_messageBox("Informacion del Robot", "El robot mide 50x50 y tiene 2 sensores uno mirando hacia adelante desde el centro del borde superior del robot y otro mirando hacia la izquierda desde el centro del borde izquierdo del robot", "ok", "info", 0);
	int check = tinyfd_messageBox("Continuar...", "¿Desea ver la informacion de los sensores?\nTenga en cuenta que si acepta continuar se va abrir una ventana de consola mostrando los valores de los sensores.", "yesno", "question", 1);
	if (check == 1)
		ShowWindow(GetConsoleWindow(), SW_SHOW); //oculto la consola.
	if (!al_init())
		return EXIT_FAILURE;
	map_t map;
	if (readMap("mapa", map) == false)
		return EXIT_FAILURE;

	//if (S_Init("robot.txt") != F_OK)
		//return EXIT_FAILURE;


	robot_t r;
	//r.D_velocity = 0;
	//r.width = 50;
	//r.height = 50;
	r.robotPoints = new dpoint_t[40];
	r.amountOfPoints = 40;
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
	
	r.robotPoints[12].x = 0;
	r.robotPoints[12].y = 10;
	r.robotPoints[13].x = 0;
	r.robotPoints[13].y = 5;
	r.robotPoints[14].x = 0;
	r.robotPoints[14].y = 20;
	r.robotPoints[15].x = 0;
	r.robotPoints[15].y = 25;
	r.robotPoints[16].x = 0;
	r.robotPoints[16].y = 35;
	r.robotPoints[17].x = 0;
	r.robotPoints[17].y = 40;
	r.robotPoints[18].x = 0;
	r.robotPoints[18].y = 45;

	r.robotPoints[19].x = 5;
	r.robotPoints[19].y = 0;
	r.robotPoints[20].x = 10;
	r.robotPoints[20].y = 0;
	r.robotPoints[21].x = 20;
	r.robotPoints[21].y = 0;
	r.robotPoints[22].x = 25;
	r.robotPoints[22].y = 0;
	r.robotPoints[23].x = 35;
	r.robotPoints[23].y = 0;
	r.robotPoints[24].x = 40;
	r.robotPoints[24].y = 0;
	r.robotPoints[25].x = 45;
	r.robotPoints[25].y = 0;

	r.robotPoints[26].x = 50;
	r.robotPoints[26].y = 5;
	r.robotPoints[27].x = 50;
	r.robotPoints[27].y = 10;
	r.robotPoints[28].x = 50;
	r.robotPoints[28].y = 20;
	r.robotPoints[29].x = 50;
	r.robotPoints[29].y = 25;
	r.robotPoints[30].x = 50;
	r.robotPoints[30].y = 35;
	r.robotPoints[31].x = 50;
	r.robotPoints[31].y = 40;
	r.robotPoints[32].x = 50;
	r.robotPoints[32].y = 45;

	r.robotPoints[33].x = 5;
	r.robotPoints[33].y = 50;
	r.robotPoints[34].x = 10;
	r.robotPoints[34].y = 50;
	r.robotPoints[35].x = 20;
	r.robotPoints[35].y = 50;
	r.robotPoints[36].x = 25;
	r.robotPoints[36].y = 50;
	r.robotPoints[37].x = 35;
	r.robotPoints[37].y = 50;
	r.robotPoints[38].x = 40;
	r.robotPoints[38].y = 50;
	r.robotPoints[39].x = 45;
	r.robotPoints[39].y = 50;

	r.R_point.x = 25;
	r.R_point.y = 25;
	r.position.x = 270;
	r.position.y = 270;
	r.D_angle = 0;
	r.D_velocity = 0;
	r.R_velocity = 0;
	r.sensorArray[0].positionOnRobot.x = 25;
	r.sensorArray[0].positionOnRobot.y = 0;
	r.sensorArray[0].angle = 0;
	r.sensorArray[1].positionOnRobot.x = 0;
	r.sensorArray[1].positionOnRobot.y = 25;
	r.sensorArray[1].angle = -3.14195/2.0;

	//r.width = F_getBasicInfo(WIDTH);
	//r.height = F_getBasicInfo(HEIGHT);
	uint16_t nSens = S_getAmountSen();

	for (unsigned int i = 0; i < nSens; i++) {
		r.sensorArray[i].positionOnRobot.x = F_getSensorXPos(i);
		r.sensorArray[i].positionOnRobot.y = F_getSensorYPos(i);
		r.sensorArray[i].angle = F_getSensorAngle(i);
	}
	

	uipoint_t rSize = {50, 50};
	const char robotPath[] = "./robot.png";

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
				g.drawRobot(W_absolutePoint(centerPoint), W_getRobotPosition().angle);
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
			g.drawRobot(W_absolutePoint(centerPoint), W_getRobotPosition().angle);

			//for (unsigned int i = 0; i<nSens; i++)
			g.drawSensorInfo(r.sensorArray[0], S_getStateValue(0)); //aca mostras lo que te devuelve sen&act de alguna manera
			g.drawSensorInfo(r.sensorArray[1], S_getStateValue(1));
			std::cout << std::endl;
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