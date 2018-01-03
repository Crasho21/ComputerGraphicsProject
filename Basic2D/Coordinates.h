#pragma once

class Coordinates {
public:
	float x;
	float y;

	Coordinates();
	Coordinates(float x, float y) :x(x), y(y) {};
	~Coordinates();

	float squareDistance(Coordinates other);
};