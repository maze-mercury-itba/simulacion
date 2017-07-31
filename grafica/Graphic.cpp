#include "Graphic.h"
#include <cstdio>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_native_dialog.h>
#include <cmath>
#include <iostream>

Graphic::Graphic(const char * robotPath, uipoint_t robotSize, map_t map, const char * backgroundPath)
{
	display = NULL;
	robot = NULL;
	background = NULL;
	isValid = false;

	if ((display = al_create_display(DISP_WIDTH, DISP_HEIGHT)) != NULL) {
		if (al_init_image_addon()) {
			if (al_init_primitives_addon()) {
				if ((robot = al_load_bitmap(robotPath)) != NULL) {
					if ((backgroundPath != NULL && (background = al_load_bitmap(backgroundPath)) != NULL)
						|| backgroundPath == NULL) {
						isValid = true;
						robotScaleFactor.x = float(robotSize.x) / float(al_get_bitmap_width(robot));
						robotScaleFactor.y = float(robotSize.y) / float(al_get_bitmap_height(robot));

						this->map.nWalls = map.nWalls;
						this->map.target = map.target;
						this->map.walls = new dvector_t[map.nWalls];
						memcpy(this->map.walls, map.walls, sizeof(dvector_t)*map.nWalls);
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
	else {
		fprintf(stderr, "Unable to create display\n");
	}
}

Graphic::~Graphic()
{
	if (this->isValid) {
		if (background != NULL) {
			al_destroy_bitmap(background);
			background = NULL;
		}

		al_uninstall_keyboard();
		al_destroy_bitmap(robot);
		robot = NULL;
		al_destroy_display(display);
		display = NULL;
		delete[] map.walls;
		map.walls = NULL;
	}
}

void Graphic::drawBackground()
{
	if (background == NULL)
		al_clear_to_color(al_map_rgb(255, 255, 255));
	else
		al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background),
			0, 0, DISP_WIDTH, DISP_HEIGHT, 0);

	for (unsigned int i = 0; i<map.nWalls; i++) {
		al_draw_line(map.walls[i].start.x, map.walls[i].start.y, map.walls[i].end.x, map.walls[i].end.y,
			al_map_rgb(0, 0, 0), WALL_THICKNESS);
	}

	al_draw_filled_circle(map.target.x, map.target.y, 5, al_map_rgb(255, 0, 0));	//dibujar el target
}

void Graphic::drawRobot(dpoint_t pos, double angle)
{
	lastRobotPos = pos;
	lastRobotAngle = angle;
	al_draw_scaled_rotated_bitmap(robot, al_get_bitmap_width(robot)/2, al_get_bitmap_height(robot)/2, 
		pos.x, pos.y, robotScaleFactor.x, robotScaleFactor.y,angle, 0);
	//al_draw_scaled_rotated_bitmap(robot, 0, 0, pos.position.x, pos.position.y, robotScaleFactor.x, robotScaleFactor.y, 0, 0);

}

void Graphic::drawSensorInfo(sensor_t s, double distance) 
{
	//double sx = s.positionOnRobot.x - al_get_bitmap_width(robot) * robotScaleFactor.x/2;
	//double sy = s.positionOnRobot.y - al_get_bitmap_width(robot) * robotScaleFactor.y/2; //para acortar los nombres nada mas

	//double sensDist = sqrt(sx*sx + sy*sy);
	//
	//double sensAngle = 3.159/2; //caso sx == 0 (angle va a estar entre 0 y 90 porque el robot es cuadrado y se mide de la esq sup izq
	//if (sy == 0 && sx < 0)
	//	sensAngle += 3.159;
	//else if (sy != 0) {
	//	sensAngle = atan(sy / sx);
	//	if (sensAngle > 0) {
	//		sensAngle = 3*3.159/2 - sensAngle; //primer cuadrante cartesiano
	//		if (sx < 0)
	//			sensAngle -= 3.159; //tercer cuadrante cartesiano
	//	}		
	//	else {
	//		sensAngle = 3.159 / 2 - sensAngle; //segundo cuadrante cartesiano
	//		if (sx > 0) //cuarto cuadrante
	//			sensAngle += 3.159;
	//	}
	//	//ahora tengo con el sistema de coordenadas "normal" (respecto de x, antiohorario)
	//	//lo paso a respecto de y, horario
	//	sensAngle = 3.159 / 2 + 2 * 3.159 - sensAngle;
	//}
	////en sensAngle tengo que tan corrido esta por default del eje x. ahora tengo que ver como se realciona con la pos actual
	//sensAngle += lastRobotPos.angle;

	//float x = lastRobotPos.position.x + sensDist*sin(sensAngle);
	//float y = lastRobotPos.position.y + sensDist*cos(sensAngle);

	//al_draw_line(x, y, x + distance*sin(sensAngle+s.angle), y + distance*cos(sensAngle+s.angle), al_map_rgb(0, 0, 255), 1);
	std::cout << "Sensor:" << distance <<"   ";

}

void Graphic::showChanges()
{
	al_flip_display();
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
