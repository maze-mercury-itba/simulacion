#pragma once
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include "../vector.h"
#include "../World/World.h"

#define DISP_WIDTH		800
#define	DISP_HEIGHT		600	

#define WALL_THICKNESS	5

class Graphic
{
public:
	Graphic(const char * robotPath, uidpoint_t robotSize, map_t map, const char * backgroundPath = NULL);
	~Graphic();

	ALLEGRO_DISPLAY * getDisplay();

	void drawBackground();
	void drawRobot(position_t pos);
	void showChanges();

//	mostrar cosas de los sensores!

	void showWinMsg();
	void showLoseMsg();

private:
	bool isValid;

	ALLEGRO_DISPLAY * display;

	ALLEGRO_BITMAP * robot;
	ALLEGRO_BITMAP * background;
	map_t map;

	fdpoint_t robotScaleFactor;
};