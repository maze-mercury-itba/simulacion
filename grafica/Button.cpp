#include "Button.h"
#include <algorithm>
#include "allegro5\allegro_primitives.h"

Button::Button(button_t name, const char * imgPath)
{
	isValid = false;
	img = NULL;
	active = true;

	if (imgPath != NULL) {
		img = al_load_bitmap(imgPath); //si hay error queda tambien en NULL
		if (img != NULL) {
			this->name = name;
		}	
	}
}

bool Button::resize(uivector_t coord)
{
	if (img == nullptr)
		return false;

	isValid = true;

	if (coord.end.x == coord.start.x || coord.start.y == coord.end.y) {
		isValid = false;
	}

	this->coord.x = std::min(coord.start.x, coord.end.x);
	this->coord.y = std::min(coord.start.y, coord.end.y);

	size.x = std::abs(coord.end.x - coord.start.x);
	size.y = std::abs(coord.end.y - coord.start.y);


	return isValid;
}

void Button::draw()
{
	if (isValid) {
		al_draw_scaled_bitmap(img, 0, 0, al_get_bitmap_width(img), al_get_bitmap_height(img),
			coord.x, coord.y, size.x, size.y, 0);

		if (!active) {
			al_draw_filled_rectangle(coord.x, coord.y, coord.x + size.x, coord.y + size.y, al_map_rgba(0, 0, 0, 255 / 2));
		}
	}
}

void Button::activate()
{
	active = true;
}

void Button::deactivate()
{
	active = false;
}
void Button::toggle()
{
	active = !active;
}

bool Button::wasPressed(uint16_t x, uint16_t y) const
{
	return (isValid && active && x > coord.x && y > coord.y && x-coord.x < size.x && y-coord.y < size.y);
}

button_t Button::getName() const
{
	return name;
}
