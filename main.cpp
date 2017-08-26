#include <Windows.h>
#include <iostream>
#include <cstdio>
#include "World\World.h"
#include "grafica\EventGenerator.h"
#include "grafica\Graphic.h"
//extern "C" {
#include "senact\senact.h"
#include "Intelligence\Intelligence.h"
//}
#include <allegro5\allegro.h>
#include "FileHandler\mapFile.h"
extern "C" {
#include "FileHandler\FileHandler.h"
#include "TFD\tinyfiledialogs.h"
}
#include "grafica\Button.h"
#include "grafica\Dispatcher.h"

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
		r.robotPoints[i].x = i * 2;
		r.robotPoints[i].y = 0;
		r.robotPoints[i + 25].x = 0;
		r.robotPoints[i + 25].y = i * 2;
		r.robotPoints[i + 50].x = i * 2;
		r.robotPoints[i + 50].y = 50;
		r.robotPoints[i + 75].x = 50;
		r.robotPoints[i + 75].y = i * 2;
	}

	r.R_point.x = .15/2;
	r.R_point.y = .15/2;
	r.position.x = 1;
	r.position.y = 1;
	r.D_angle = 0;
	r.D_velocity = 0;
	r.R_velocity = 0;

	r.sensorArray[0].positionOnRobot.x = 25;
	r.sensorArray[0].positionOnRobot.y = 0;
	r.sensorArray[0].angle = 0;
	r.sensorArray[1].positionOnRobot.x = 0;
	r.sensorArray[1].positionOnRobot.y = 25;
	r.sensorArray[1].angle = -3.14195 / 5.0;
	r.sensorArray[2].positionOnRobot.x = 50;
	r.sensorArray[2].positionOnRobot.y = 25;
	r.sensorArray[2].angle = 3.14195 / 5.0;

	for (unsigned int i = 0; i < 100; i++) {
		r.robotPoints[i].x *= (.15/50.0);
		r.robotPoints[i].y *= (.15 / 50.0);
	}

	for (unsigned int i = 0; i < 3; i++) {
		r.sensorArray[i].positionOnRobot.x *= (.15 / 50.0);
		r.sensorArray[i].positionOnRobot.y *= (.15 / 50.0);
	}

	//r.width = F_getBasicInfo(WIDTH);
	//r.height = F_getBasicInfo(HEIGHT);
	//	uint16_t nSens = S_getAmountSen();

	//for (unsigned int i = 0; i < AMOUNT_OF_SENSORS; i++) {
	//	r.sensorArray[i].positionOnRobot.x = F_getSensorXPos(i);
	//	r.sensorArray[i].positionOnRobot.y = F_getSensorYPos(i);
	//	r.sensorArray[i].angle = F_getSensorAngle(i);
	//}


	fpoint_t rSize = { .15, .15 };
	const char robotPath[] = "grafica/robot.png";
	initTodo(r);
	W_Init(&map);
	I_Init(0);
	W_setRobotConfiguration(&r);


	Graphic g(&robotPath[0], rSize, &map, NULL, 1000, 700);
	g.drawBackground();
	g.drawButtons();
	g.drawSimSpeed(1.0);
	g.showChanges();

	EventGenerator e(g.getDisplay(), g.getButtons());
	Event * ev = new Event(NO_EVENT);

	Dispatcher d(g, r);
	//Orden de updates: mundo, sensores y actuadores, inteligencia. Es importante.
	do {
		delete ev;
		ev = e.getNextEvent();
		d.dispatch(ev);
	} while (ev->name != EXIT);
	delete ev;

	return EXIT_SUCCESS;
}