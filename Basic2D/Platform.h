#pragma once

#include <vector>
#include <gl\gl.h>			// header file for the opengl32 library
#include <gl\glu.h>			// header file for the glu32 library

#include "Coordinates.h"
#include "Vertex.h"

class Platform {
private:
	float width = 0.05;
	float height = 0.025;
	float z;
	int id;
	Coordinates center;
	std::vector<Vertex> vector;

	GLuint texture[30];
public:
	Platform() {}
	Platform(Coordinates center, float z, int id) {
		this->center = center;
		this->z = z;
		this->id = id;
		vector.clear();
		vector.push_back(Vertex(center.x + width / 2, center.y - height / 2, z, 0, 0));	//basso dx
		vector.push_back(Vertex(center.x - width / 2, center.y - height / 2, z, 1, 0));	//basso sx
		vector.push_back(Vertex(center.x - width / 2, center.y + height / 2, z, 1, 1));	//alto sx
		vector.push_back(Vertex(center.x + width / 2, center.y + height / 2, z, 0, 1));	//alto dx

		this->loadGLTexture();
	}
	~Platform();

	bool loadGLTexture();
};