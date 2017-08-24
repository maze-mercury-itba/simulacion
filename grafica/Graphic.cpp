#include "Graphic.h"
#include <cstdio>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_native_dialog.h>
#include <cmath>
#include <iostream>
#include <limits>
#include <algorithm>
extern  "C" {
#include "../FileHandler/moreString.h"
}

using namespace std;


#define SPEED_STRING	"Simulation speed: x"
#define SPEED_DIGITS	"000"		//los numeros van a tener esta cantidad de caracteres


bool setUpButtons(std::vector<Button>& b);

Graphic::Graphic(const char * robotPath, fpoint_t robotSize, map_t * map, const char * backgroundPath, uint16_t width, uint16_t height)
{
	fontSize = 0;
	display = NULL;
	robot = NULL;
	font = NULL;
	background = NULL;
	isValid = false;
	realMap.start.x = realMap.start.y = FLT_MAX;
	realMap.end.x = realMap.end.y = 0;

	if (al_init_image_addon()) {
		if (al_init_primitives_addon()) {
			if ((robot = al_load_bitmap(robotPath)) != NULL) {
				if ((backgroundPath != NULL && (background = al_load_bitmap(backgroundPath)) != NULL)
					|| backgroundPath == NULL) {
					al_init_font_addon();
					al_init_ttf_addon();
					if (setUpButtons(b)) {
						isValid = true;
						for (unsigned int i = 0; i < map->nWalls; i++) {
							realMap.start.x = min(map->walls[i].start.x, double(realMap.start.x));
							realMap.start.x = min(map->walls[i].end.x, double(realMap.start.x));
							realMap.start.y = min(map->walls[i].start.y, double(realMap.start.y));
							realMap.start.y = min(map->walls[i].start.y, double(realMap.start.y));

							realMap.end.x = max(map->walls[i].start.x, double(realMap.end.x));
							realMap.end.x = max(map->walls[i].end.x, double(realMap.end.x));
							realMap.end.y = max(map->walls[i].start.y, double(realMap.end.y));
							realMap.end.y = max(map->walls[i].start.y, double(realMap.end.y));
						}
						this->robotSize = robotSize;
						this->map = map;
						if (newDispSize(width, height)) {
							this->map = map;
							al_clear_to_color(al_map_rgb(255, 255, 255));
						}
						else {
							isValid = false;
							fprintf(stderr, "Unable to create display\n");
							if (background != nullptr)
								al_destroy_bitmap(background);
							al_destroy_bitmap(robot);
							robot = NULL;
							al_destroy_display(display);
							display = NULL;
						}
					}
					else {
						fprintf(stderr, "Unable to set up buttons\n");
						if (background != nullptr)
							al_destroy_bitmap(background);
							al_destroy_bitmap(robot);
							robot = NULL;
							al_destroy_display(display);
							display = NULL;
						}
					}
				else {
					fprintf(stderr, "Unable to load background\n");
					al_destroy_bitmap(robot);
					robot = NULL;
					al_destroy_display(display);
					display = NULL;
				}
			}
			else {
				fprintf(stderr, "Unable to load robot bitmap\n");
				al_destroy_display(display);
				display = NULL;
			}
		}
		else {
			fprintf(stderr, "Unable to initialize primitives add-on\n");
			al_destroy_display(display);
			display = NULL;
		}
	}
	else {
		fprintf(stderr, "Unable to install image add-on\n");
		al_destroy_display(display);
		display = NULL;
	}
}

Graphic::~Graphic()
{
	if (this->isValid) {
		if (background != NULL) {
			al_destroy_bitmap(background);
			background = NULL;
		}
		if (font != nullptr) {
			al_destroy_font(font);
			font = NULL;
		}

		al_shutdown_ttf_addon();
		al_shutdown_font_addon();
		al_shutdown_image_addon();
		al_shutdown_native_dialog_addon();
		al_shutdown_primitives_addon();
		al_uninstall_keyboard();
		al_destroy_bitmap(robot);
		robot = NULL;
		al_destroy_display(display);
		display = NULL;
	}
}

void Graphic::drawBackground()
{
	if (background == NULL) {
		al_draw_filled_rectangle(mapArea.start.x, mapArea.start.y, mapArea.end.x, mapArea.end.y, al_map_rgb(255, 255, 255));
		al_draw_rectangle(mapArea.start.x - G_MARGIN/2, mapArea.start.y - G_MARGIN / 2, 
			mapArea.end.x + G_MARGIN / 2, mapArea.end.y + G_MARGIN / 2, al_map_rgb(0,0,255), G_MARGIN/2);
	}
	else
		al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background),
			mapArea.start.x, mapArea.start.y, mapArea.end.x, mapArea.end.y, 0);

	for (unsigned int i = 0; i<currMap.size(); i++) {
		al_draw_line(currMap[i].start.x, currMap[i].start.y, currMap[i].end.x, currMap[i].end.y,
			al_map_rgb(0, 0, 0), min(mapSize.x, mapSize.y)*0.01);
	}
//	al_draw_filled_circle(map.target.x, map.target.y, 5, al_map_rgb(255, 0, 0));	//dibujar el target
}





void Graphic::drawRobot(dpoint_t pos, double angle)
{
	fpoint_t newPos = scalePoint(pos);
	lastRobotPos.x = newPos.x;	lastRobotPos.y = newPos.y;
	lastRobotAngle = angle;
	dpoint_t bm = { al_get_bitmap_width(robot)/2, al_get_bitmap_height(robot)/2 };

	if (lastRobotPos.x + bm.x < mapArea.end.x && lastRobotPos.x - bm.x > mapArea.start.x &&
		lastRobotPos.y + bm.y < mapArea.end.y && lastRobotPos.y - bm.y > mapArea.start.y) {
		al_draw_scaled_rotated_bitmap(robot, bm.x, bm.y, lastRobotPos.x, lastRobotPos.y, 
			robotScaleFactor.x, robotScaleFactor.y, angle, 0);
	}
}

void Graphic::drawRobot(double x, double y)
{
	dpoint_t pos = { x, y };
	if (x == DBL_MAX || y == DBL_MAX)
		pos = lastRobotPos;

	drawRobot(pos, lastRobotAngle);
}

void Graphic::drawSensorInfo(sensor_t s, double distance) 
{
	dvector_t realLine; fvector_t scaledLine;
	realLine.start = realLine.end = W_absolutePoint(s.positionOnRobot);
	realLine.end.x += distance * sin(s.angle + lastRobotAngle);
	realLine.end.y -= distance * cos(s.angle + lastRobotAngle);
	
	scaledLine = scaleVector(realLine);

	al_draw_line(scaledLine.start.x, scaledLine.start.y, scaledLine.end.x, scaledLine.end.y, al_map_rgb(0, 0, 255), 3);
	
//	std::cout << "Sensor:" << distance <<"   ";

}

void Graphic::drawButtons(button_t id)
{
	if (id < N_BUTTONS)
		b[id].draw();
	else
		for (unsigned int i = 0; i < N_BUTTONS; i++)
			b[i].draw();
}

void Graphic::showChanges()
{
	al_flip_display();
}

void Graphic::drawSimSpeed(float speed)
{
	if (SPEED_FS != fontSize){
		fontSize = SPEED_FS;
		if (font != nullptr) {
			al_destroy_font(font);
		}
		font = al_load_ttf_font("grafica/font.ttf", fontSize, 0);
	}

	if (font != nullptr) {
		char text [sizeof(SPEED_STRING)+sizeof(SPEED_DIGITS)-1]= SPEED_STRING;
		snprintf(text+sizeof(SPEED_STRING)-1, sizeof(SPEED_DIGITS), "%f", speed);

		fpoint_t p0 = { mapArea.start.x, 0 };
		al_draw_filled_rectangle(p0.x, p0.y, p0.x + al_get_text_width(font, text), 
			p0.y + al_get_font_line_height(font), al_map_rgb(255,255,255) );
		al_draw_text(font, al_map_rgb(0, 0, 0), p0.x, p0.y, 0, text);
	}
}

void Graphic::showLoseMsg()
{
	al_show_native_message_box(	//indicar al usuario el tickCount
		NULL,
		"PERDISTE",
		" ",
		" ",
		"close",
		ALLEGRO_MESSAGEBOX_OK_CANCEL);

}

void Graphic::showWinMsg()
{
	al_show_native_message_box(	//indicar al usuario el tickCount
		NULL,
		"GANASTE",
		" ",
		" ",
		"close",
		ALLEGRO_MESSAGEBOX_OK_CANCEL);

}

ALLEGRO_DISPLAY * Graphic::getDisplay()
{
	return display;
}

const std::vector<Button>* Graphic::getButtons()
{
	return &b;
}

void Graphic::setButtonState(button_t name, bool active)
{
	for (std::vector<Button>::iterator it = b.begin(); it != b.end(); it++) {
		if (it->getName() == name) {
			active ? it->activate() : it->deactivate();
		}
	}
}

bool setUpButtons(std::vector<Button>& b)
{
	const char * paths[N_BUTTONS] = { "grafica/fastforward.png","grafica/slow.png", "grafica/rswitch.png", 
		"grafica/mode.png", "grafica/play.png", "grafica/pause.png", "grafica/newPos.png" };

	for (unsigned int i = 0; i < N_BUTTONS; i++) {
		Button button(button_t(i), paths[i]);
		b.push_back(button);
	}
	return true;
}

bool Graphic::newDispSize(uint16_t width, uint16_t height)
{
	if (isValid == false)
		return false;

	if (display != nullptr && width != 0 && height != 0 && al_get_display_flags(display) != ALLEGRO_FULLSCREEN_WINDOW) {
		al_resize_display(display, width, height); //solo tengo que cambiar el tamanio del display que ya tengo
		dispSize.x = width;
		dispSize.y = height;
	}
	else {
		if (display != nullptr) {
			al_destroy_display(display); //si habia display antes, lo borro
		}
		al_set_new_display_flags(0); //pongo en default

		if (width == 0 || height == 0) { //pantalla completa: el tamanio lo hago segun la resolucion maxima
			ALLEGRO_DISPLAY_MODE disp_data;
			al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
			dispSize.x = disp_data.width;
			dispSize.y = disp_data.height;
			al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
		}
		else {
			dispSize.x = width;		//me dicen el tamanio entonces uso ese
			dispSize.y = height;
			al_set_new_display_flags(ALLEGRO_RESIZABLE);
		}
		al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
		al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
		display = al_create_display(dispSize.x, dispSize.y);
	}

	if (display == nullptr) {
		isValid = false; //no se hizo bien el display
	}
	else { //si se hizo bien, resizeo el mapa y los botones
		uivector_t coord;
		coord.start.x = coord.start.y = coord.end.y = coord.end.x = G_MARGIN; //dejo un poco de lugar para que no quede pegado al borde
		
		float buttonDist = (dispSize.y - (1+N_BUTTONS)*G_MARGIN) / N_BUTTONS; 
		coord.end.y = coord.end.x += buttonDist;
		buttonDist += G_MARGIN; //los botones son cuadrados -> el alto es igual al ancho.
		//ademas dejo un lugarcito entre los botones, dependiendo de cuanto lugar quiero ocupar verticalmente

		//if (buttonDist < coord.end.x - coord.start.x) {
		//	coord.end.x = coord.start.x + (G_BUTTON_H - G_MARGIN) / N_BUTTONS;
		//	buttonDist = G_MARGIN + coord.end.x - coord.start.x;
		//}

		//coord.start.y = G_MARGIN;					//botones cuadrados!
		//coord.end.y = coord.start.y + coord.end.x - coord.start.x;

		for (unsigned int i = 0; i < b.size(); i++) {
			b[i].resize(coord);

			coord.start.y += buttonDist;	//el proximo boton va abajo del anterior
			coord.end.y += buttonDist;
		}
		
		map0.x = coord.end.x + 2 * G_MARGIN;
		map0.y = 2*G_MARGIN + SPEED_FS;
		mapArea.start = map0;
		mapArea.end.x = dispSize.x - G_MARGIN;
		mapArea.end.y = dispSize.y - G_MARGIN;
		mapSize.y = dispSize.y - G_MARGIN;
		mapSize.x = mapArea.end.x - map0.x;
		mapSize.y = mapArea.end.y - map0.y;

		float widthPerHeight = (realMap.end.x - realMap.start.x) / (realMap.end.y - realMap.start.y);
		if ( mapSize.x/mapSize.y > widthPerHeight ) { //la altura queda, el ancho hay que bajarlo
			map0.x += mapSize.x / 2.0;	//pongo map0.x en el centro de la region del mapa
			mapSize.x = mapSize.y * widthPerHeight;	//calculo el nuevo ancho
			map0.x -= mapSize.x / 2.0;	//lo corro segun el tamanio que va a quedar
		}
		else {
			map0.y += mapSize.y / 2.0;
			mapSize.y = mapSize.x / widthPerHeight;
			map0.y -= mapSize.y / 2.0;
		}
	}
	rescaleMap();

	robotScaleFactor.x = robotSize.x * mapSize.x / (realMap.end.x - realMap.start.x) / al_get_bitmap_width(robot);
	robotScaleFactor.y = robotSize.y * mapSize.y / (realMap.end.y - realMap.start.y) / al_get_bitmap_height(robot);

	al_clear_to_color(al_map_rgb(255, 255, 255));
	drawBackground();
	drawButtons();

	return isValid;
}

dpoint_t Graphic::realFromPixel(uint16_t x, uint16_t y)
{
	dpoint_t ans;
	
	if (x < mapArea.end.x && x > mapArea.start.x && y < mapArea.end.y && y > mapArea.start.y) {
		ans.x = (x - map0.x) / mapSize.x * (realMap.end.x - realMap.start.x) + realMap.start.x;
		ans.y = (y - map0.y) / mapSize.y * (realMap.end.y - realMap.start.y) + realMap.start.y;
	}
	else {
		ans.x = ans.y = DBL_MAX;
	}
	
	return ans;
}

void Graphic::rescaleMap()
{
	currMap.clear();
	fvector_t newWall;
	uivector_t w;
	for (unsigned int i = 0; i < map->nWalls; i++) {
		newWall = scaleVector(map->walls[i]);
		w.start.x = (uint16_t)newWall.start.x;		w.end.x = (uint16_t)newWall.end.x;
		w.start.y = (uint16_t)newWall.start.y;		w.end.y = (uint16_t)newWall.end.y;

		currMap.push_back(w);
	}
}

fvector_t Graphic::scaleVector(dvector_t v)
{
	fvector_t ans;
	ans.start = scalePoint(v.start);
	ans.end = scalePoint(v.end);
	return ans;
}

fpoint_t Graphic::scalePoint(dpoint_t p)
{
	fpoint_t ans;
	ans.x = (p.x - realMap.start.x) / (realMap.end.x - realMap.start.x) * mapSize.x + map0.x;
	ans.y = (p.y - realMap.start.y) / (realMap.end.y - realMap.start.y) * mapSize.y + map0.y;
	
	ans.x = min(ans.x, mapArea.end.x); ans.x = max(ans.x, mapArea.start.x);
	ans.y = min(ans.y, mapArea.end.y); ans.y = max(ans.y, mapArea.start.y);

	return ans;
}