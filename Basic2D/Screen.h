#pragma once

#include <vector>

#include "Vertex.h"

// Id immagini di sfondo 
const int START = 0;
const int WIN = 1;
const int LOSE = 2;
const int HELP = 3;

class Screen {
private:
	std::vector<Vertex> vectVertex;

	GLuint texture[4];

	float width = 0.08;
	float height = 0.08;

public:
	Screen() {}

	Screen(double width, double height) {
		this->width = width;
		this->height = height;

		vectVertex.clear();
		vectVertex.push_back(Vertex(-width, -height, -5, 0, 0));	//basso dx
		vectVertex.push_back(Vertex(+width, -height, -5, 1, 0));	//basso sx
		vectVertex.push_back(Vertex(+width, +height, -5, 1, 1));	//alto sx
		vectVertex.push_back(Vertex(-width, +height, -5, 0, 1));	//alto dx

		this->loadGLTexture();
	}

	~Screen();

	bool loadGLTexture();
	bool drawGL(int id);
};
