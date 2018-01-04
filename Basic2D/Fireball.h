#pragma once

#include <vector>
#include <gl\gl.h>			// header file for the opengl32 library
#include <gl\glu.h>			// header file for the glu32 library

#include "Coordinates.h"
#include "Vertex.h"

class Fireball {
private:
	float width = 0.05;
	float height = 0.025;
	Coordinates center;
	std::vector<Vertex> vector;

	GLuint texture[3];

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

		vector.clear();

		vector.push_back(Vertex(center.x + width / 2, center.y - height / 2, z, 0, 0));	//basso dx
		vector.push_back(Vertex(center.x - width / 2, center.y - height / 2, z, 1, 0));	//basso sx
		vector.push_back(Vertex(center.x - width / 2, center.y + height / 2, z, 1, 1));	//alto sx
		vector.push_back(Vertex(center.x + width / 2, center.y + height / 2, z, 0, 1));	//alto dx

		this->loadGLTexture();
	}
	Fireball(int power, int angle, Coordinates startPoint, int z);
	Fireball(float incrX, float incrY, Coordinates startPoint, int z);
	~Fireball();

	//bool drawBullet(Earth* earth, Tank* tank1, Tank* tank2);
	bool loadGLTexture();
	Vertex moveFireball();

	Coordinates getCenter() { return center; };
};