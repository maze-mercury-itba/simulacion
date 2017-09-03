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
}
#include "grafica\Dispatcher.h"
#include "tobysarah.h"

#ifndef		DEBUG
#define ALLEGRO_STATICLINK
#endif



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

	dpoint_t p0 = { 0.2, 0.2 };
	rInfo_t r[2];
	r[1].r = loadSarah(p0);
	r[0].r = loadToby(p0);
	r[1].img = S_IMG;
	r[0].img = T_IMG;
	r[1].size = sarahSize();
	r[0].size = tobySize();

	//r.robotPoints = new dpoint_t[100];
	//r.amountOfPoints = 100;
	//for (int i = 0; i < 25; i++)
	//{
	//	r.robotPoints[i].x = i * 2;
	//	r.robotPoints[i].y = 0;
	//	r.robotPoints[i + 25].x = 0;
	//	r.robotPoints[i + 25].y = i * 2;
	//	r.robotPoints[i + 50].x = i * 2;
	//	r.robotPoints[i + 50].y = 50;
	//	r.robotPoints[i + 75].x = 50;
	//	r.robotPoints[i + 75].y = i * 2;
	//}
	//r.R_point.x = .15/2;
	//r.R_point.y = .15/2;
	//r.position.x = 1.0;
	//r.position.y = 1.0;
	//r.D_angle = 0;
	//r.D_velocity = 0;
	//r.R_velocity = 0;
	//r.sensorArray[0].positionOnRobot.x = 25;
	//r.sensorArray[0].positionOnRobot.y = 0;
	//r.sensorArray[0].angle = 0;
	//r.sensorArray[1].positionOnRobot.x = 0;
	//r.sensorArray[1].positionOnRobot.y = 25;
	//r.sensorArray[1].angle = -3.14195 / 5.0;
	//r.sensorArray[2].positionOnRobot.x = 50;
	//r.sensorArray[2].positionOnRobot.y = 25;
	//r.sensorArray[2].angle = 3.14195 / 5.0;
	//for (unsigned int i = 0; i < 100; i++) {
	//	r.robotPoints[i].x *= (.15/50.0);
	//	r.robotPoints[i].y *= (.15 / 50.0);
	//}
	//for (unsigned int i = 0; i < 3; i++) {
	//	r.sensorArray[i].positionOnRobot.x *= (.15 / 50.0);
	//	r.sensorArray[i].positionOnRobot.y *= (.15 / 50.0);
	//}

	W_Init(&map);

	Graphic g(r[0].img, r[0].size, &map, NULL, 1000, 700);
	if (!g.isValid())
		return EXIT_FAILURE;
	
	EventGenerator e(g.getDisplay(), g.getButtons());
	if (!e.isValid())
		return EXIT_FAILURE;
			
	
	Dispatcher d(g, r);
	g.drawBackground();
	g.drawButtons();
	g.drawSimSpeed(1.0);
	g.showChanges();

	Event * ev = new Event(NO_EVENT);

	do {
		delete ev;
		ev = e.getNextEvent();
		d.dispatch(ev);
	} while (ev->name != EXIT);
		
	delete ev;
	delete[] r[0].r.robotPoints;
	delete[] r[1].r.robotPoints;

	return EXIT_SUCCESS;
}



