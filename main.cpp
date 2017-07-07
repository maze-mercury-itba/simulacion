#include "World/World.h"
#include "grafica/EventGenerator.h"
#include "grafica/Graphic.h"
//extern "C" {
#include "senact/senact.h"
#include "Intelligence/Intelligence.h"
//}
#include <allegro5\allegro.h>


#define N_WALLS 4

int main(void)
{
	if (!al_init())
		return EXIT_FAILURE;

	uint16_t worldState = NOTHING_HAPPENED;
	robot_t r = { 40, 400, 40, 0, 0 }; r.position = { 50,50 };
	uidpoint_t rSize = {uint16_t(r.width), uint16_t(r.height)};
	const char robotPath[]=  "grafica/robot.png" ;

	dvector_t wall[N_WALLS] = { { {100, 100},  {100, 500} }, { {100, 500}, {700, 500} },
	{ {700, 500}, {700, 100} }, { {700, 100}, {100, 100} } };

	map_t map = { N_WALLS, &wall[0], {200, 0} };

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
				g.drawRobot(W_getRobotPosition());
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
			g.drawRobot(W_getRobotPosition());
			S_getAmountSen(); // aca mostras lo que te devuelve sen&act de alguna manera
			S_getAmountAct(); 
			g.showChanges();
			break;


		default:
			if (ev!=NO_EVENT)
				I_Drive(&ev); //si el evento no es para mi, es para la inteligencia
			break;
		}

	} while (ev != EXIT);

	return EXIT_SUCCESS;
}