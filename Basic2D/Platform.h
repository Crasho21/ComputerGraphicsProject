#pragma once

#include <vector>
#include <gl\gl.h>			// header file for the opengl32 library
#include <gl\glu.h>			// header file for the glu32 library

#include "Coordinates.h"
#include "Vertex.h"

class Platform {
private:
	float width;
	float height;
	float z;
	int id;
	int texF;
	Coordinates center;
	std::vector<Vertex> vector;

	GLuint platformTexture[30];
public:
	Platform() {}
	Platform(Coordinates center, float width, float height, float z, int texF, int id) {
		this->center = center;
		this->width = width;
		this->height = height;
		this->z = z;
		this->id = id;
		this->texF = texF;
		vector.clear();
		vector.push_back(Vertex(center.x + width / 2, center.y - height / 2, z, 0, 0));	//basso dx
		vector.push_back(Vertex(center.x - width / 2, center.y - height / 2, z, 1, 0));	//basso sx
		vector.push_back(Vertex(center.x - width / 2, center.y + height / 2, z, 1, 1));	//alto sx
		vector.push_back(Vertex(center.x + width / 2, center.y + height / 2, z, 0, 1));	//alto dx

		this->loadGLTexture();
	}
	~Platform() {}

	Coordinates getCenter() {
		return center;
	}

	float getHeigth() {
		return this->height;
	}

	float getWidth() {
		return this->width;
	}

	bool loadGLTexture();
	bool drawGL(double movement);

	void moveX(float incrx);
};