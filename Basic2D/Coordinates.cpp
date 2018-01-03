#include "Coordinates.h"

#include <math.h>

Coordinates::Coordinates() { }

Coordinates::~Coordinates() { }

float Coordinates::squareDistance(Coordinates other) {
	return pow((this->x) - other.x, 2) + pow((this->y) - other.y, 2);
}
