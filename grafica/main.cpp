#include "World.h"
#include "EventGenerator.h"
#include "Graphic.h"
#include <allegro5\allegro.h>


#define N_WALLS 4

int main(void)
{
	if (!al_init())
		return EXIT_FAILURE;

	uint16_t worldState = CONTINUE;
	robot_t r = { 40, 400, 40, {{300, 400}, 0} };
	uipoint_t rSize = {uint16_t(r.width), uint16_t(r.height)};
	const char robotPath[]=  "robot.png" ;

	uivector_t  wall[N_WALLS] = { { {100, 100},  {100, 500} }, { {100, 500}, {700, 500} },
	{ {700, 500}, {700, 100} }, { {700, 100}, {100, 100} } };

	map_t map = { N_WALLS, &wall[0], {100, 100} };

	Graphic g(&robotPath[0], rSize, map);
	g.drawBackground();
	g.showChanges();

	W_Init(&map);
	W_setRobotConfiguration(&r);
	EventGenerator e((ALLEGRO_DISPLAY *)NULL);
	uint16_t ev = NO_EVENT;

	do {
		ev = e.getNextEvent();

		switch (ev) {
		case SIMULATION_TIMEOUT:
			worldState = W_Update();
			
			switch (worldState) {
			case GAMEOVER:
				ev = EXIT;
				g.showLoseMsg();
				break;

			case WIN:
				ev = EXIT;
				g.showWinMsg();
				break;
			}
			break;

		case FRAME_TIMEOUT:
			g.drawBackground();
			g.drawRobot(W_getRobotPosition());
			g.showChanges();
			break;


		default:
			if (ev!=NO_EVENT)
				funcion_turbia(ev);
			break;
		}

	} while (ev != EXIT);

	return EXIT_SUCCESS;
}