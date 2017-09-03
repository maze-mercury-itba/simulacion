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
	Graphic(const char * robotPath, dpoint_t robotSize, map_t * map, const char * backgroundPath, uint16_t width = 0, uint16_t height = 0);
	~Graphic();

	bool isValid(void);

	ALLEGRO_DISPLAY * getDisplay();
	const std::vector<Button> * getButtons();
	void setButtonState(button_t name, bool active);

	void drawBackground();
	void drawRobot(dpoint_t pos, double angle);
	void drawSensorInfo(sensor_t s, double distance);
	void drawButtons(button_t id = N_BUTTONS);
	void showChanges();

	void drawSimSpeed(float speed);

	void showWinMsg();
	void showLoseMsg();

	bool newRobot(const char * img, dpoint_t size);
	bool newDispSize(uint16_t width = 0, uint16_t height = 0); //si recibe 0 en alguna fullscreen
	dpoint_t realFromPixel(uint16_t x, uint16_t y); //dadas coordenadas del display, devuelve las reales

private:
	bool valid;

	ALLEGRO_DISPLAY * display;
	dpoint_t dispSize;
	dpoint_t map0;			//coordenada 0,0 del mapa
	dpoint_t mapSize;		//tamanio del mapa en el display
	dvector_t mapArea;		//donde puedo llegar a encontrar el robot (aunque se haya salido del mapa
	dvector_t realMap;	//alto/ancho del mapa real (empieza siempre en el 0,0)

	ALLEGRO_BITMAP * robot;
	ALLEGRO_BITMAP * background;
	map_t * map;						//el mapa real
	std::vector<uivector_t> currMap;	//el mapa en escala
	std::vector<Button> b;

	dpoint_t robotSize;
	dpoint_t robotScaleFactor;
	dpoint_t lastRobotPos; //para saber donde dibujar si me llega data de los sensores
	double lastRobotAngle;

	ALLEGRO_FONT * font;
	uint16_t fontSize;

	void rescaleMap();
	void rescaleRobot();
	dvector_t scaleVector(dvector_t wall);
	dpoint_t scalePoint(dpoint_t p);
	
	bool isInDisplay(dpoint_t p);
	bool isInDisplay(dvector_t v);

};


#define G_MARGIN	(min(dispSize.x, dispSize.y) * 0.01)
#define G_BUTTON_W	(dispSize.x * 0.10)
#define G_BUTTON_H	(dispSize.y - 2*G_MARGIN)
#define G_MAP_X0	(2*G_MARGIN+G_BUTTON_W)
#define G_MAP_Y0	G_MARGIN

//	FONT SIZES
#define SPEED_FS	20