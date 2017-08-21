#pragma once
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_ttf.h>
#include "../vector.h"
#include "../World/World.h"
#include "Button.h"
#include <vector>


class Graphic
{
public:
	Graphic(const char * robotPath, fpoint_t robotSize, map_t * map, const char * backgroundPath, uint16_t width = 0, uint16_t height = 0);
	~Graphic();

	ALLEGRO_DISPLAY * getDisplay();
	const std::vector<Button> * getButtons();
	void setButtonState(button_t name, bool active);

	void drawBackground();
	void drawRobot(dpoint_t pos, double angle);
	void drawRobot(double x = DBL_MAX, double y = DBL_MAX); // si alguno es dblmax lo dibujo donde estaba antes
	void drawSensorInfo(sensor_t s, double distance);
	void drawButtons(button_t id = N_BUTTONS);
	void showChanges();

	void drawSimSpeed(float speed);
//	mostrar cosas de los sensores!

	void showWinMsg();
	void showLoseMsg();

	bool newDispSize(uint16_t width = 0, uint16_t height = 0); //si recibe 0 en alguna fullscreen
	dpoint_t realFromPixel(uint16_t x, uint16_t y); //dadas coordenadas del display, devuelve las reales

private:
	bool isValid;

	ALLEGRO_DISPLAY * display;
	fpoint_t dispSize;
	fpoint_t map0;			//coordenada 0,0 del mapa
	fpoint_t mapSize;		//tamanio del mapa en el display
	fvector_t mapArea;		//donde puedo llegar a encontrar el robot (aunque se haya salido del mapa
	fvector_t realMap;	//alto/ancho del mapa real (empieza siempre en el 0,0)

	ALLEGRO_BITMAP * robot;
	ALLEGRO_BITMAP * background;
	map_t * map;						//el mapa real
	std::vector<uivector_t> currMap;	//el mapa en escala
	std::vector<Button> b;

	fpoint_t robotSize;
	fpoint_t robotScaleFactor;
	dpoint_t lastRobotPos; //para saber donde dibujar si me llega data de los sensores
	double lastRobotAngle;

	ALLEGRO_FONT * font;
	uint16_t fontSize;

	void rescaleMap();
	fvector_t scaleVector(dvector_t wall);
	fpoint_t scalePoint(dpoint_t p);
};


#define G_MARGIN	(min(dispSize.x, dispSize.y) * 0.01)
#define G_BUTTON_W	(dispSize.x * 0.10)
#define G_BUTTON_H	(dispSize.y - 2*G_MARGIN)
#define G_MAP_X0	(2*G_MARGIN+G_BUTTON_W)
#define G_MAP_Y0	G_MARGIN

//	FONT SIZES
#define SPEED_FS	20