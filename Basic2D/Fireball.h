#pragma once

#include <vector>
#include <gl\gl.h>			// header file for the opengl32 library
#include <gl\glu.h>			// header file for the glu32 library

#include "Coordinates.h"
#include "Vertex.h"

class Fireball {
private:
	float width = 0.2;
	float height = 0.1;
	Coordinates center;
	std::vector<Vertex> fireballVector;

	GLuint fireballTexture[3];

	float startPower;
	float startAngle;

	float incrX;
	float incrY;

	float z = -5.0;

	int id;
	bool isVisible = false;

	std::vector<Vertex> direction;

public:
	Fireball() {};
	Fireball(Coordinates center, float z, int id) {
		this->center = center;
		this->z = z;
		this->id = id; 
		isVisible = false;

		fireballVector.clear();
		fireballVector.push_back(Vertex(center.x + width / 2, center.y - height / 2, z, 0, 0));	//basso dx
		fireballVector.push_back(Vertex(center.x - width / 2, center.y - height / 2, z, 1, 0));	//basso sx
		fireballVector.push_back(Vertex(center.x - width / 2, center.y + height / 2, z, 1, 1));	//alto sx
		fireballVector.push_back(Vertex(center.x + width / 2, center.y + height / 2, z, 0, 1));	//alto dx

		this->loadGLTexture();
	}
	Fireball(int power, int angle, Coordinates startPoint, int z);
	Fireball(float incrX, float incrY, Coordinates startPoint, int z);
	~Fireball();

	//bool drawBullet(Earth* earth, Tank* tank1, Tank* tank2);
	bool loadGLTexture();
	bool drawFireball(double Full_elapsed);
	Vertex moveFireball();

	Coordinates getCenter() { return center; };
};