#include "Graphic.h"
#include <cstdio>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_native_dialog.h>

Graphic::Graphic(const char * robotPath, uidpoint_t robotSize, map_t map, const char * backgroundPath)
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

void Graphic::drawRobot(position_t pos)
{
	al_draw_scaled_rotated_bitmap(robot, 0, 0, pos.position.x, pos.position.y, robotScaleFactor.x, robotScaleFactor.y, pos.angle, 0);
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
