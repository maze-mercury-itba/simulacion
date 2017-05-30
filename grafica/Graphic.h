#pragma once
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include "vector.h"
#include "World.h"

#define DISP_WIDTH		800
#define	DISP_HEIGHT		600	

#define WALL_THICKNESS	5

class Graphic
{
public:
	Graphic(const char * robotPath, uipoint_t robotSize, map_t map, const char * backgroundPath = NULL);
	~Graphic();

	void drawBackground();
	void drawRobot(position_t pos);
	void showChanges();

//	mostrar cosas de los sensores!

	void showWinMsg();
	void showLoseMsg();

	ALLEGRO_DISPLAY * getDisplay();

private:
	bool isValid;

	ALLEGRO_DISPLAY * display;

	ALLEGRO_BITMAP * robot;
	ALLEGRO_BITMAP * background;
	uint16_t nWalls;
	uivector_t * wall;

	fpoint_t robotScaleFactor;
};