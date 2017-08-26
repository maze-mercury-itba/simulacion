#pragma once
#include "../vector.h"
#include "allegro5\allegro.h"

typedef enum buttons { B_FF , B_SLOW, B_SWITCH, B_MODE, B_PLAY, B_PAUSE, B_NEWPOS, N_BUTTONS } button_t;

//FALTA LA POSICION

class Button {
public:
	Button(button_t name, const char * imgPath);

	bool resize(uivector_t coord);
	void draw();
	void activate();
	void deactivate();
	void toggle();

	bool wasPressed(uint16_t x, uint16_t y) const;
	
	button_t getName() const;

private:
	bool isValid;
	uipoint_t coord;	//esquina superior izquierda del boton
	uipoint_t size;		//ancho y alto
	button_t name;
	ALLEGRO_BITMAP * img;
	bool active;
};