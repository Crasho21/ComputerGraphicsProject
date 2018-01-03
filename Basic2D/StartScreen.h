#pragma once

#include <vector>

#include "Vertex.h"

class StartScreen {
private:
	std::vector<Vertex> vectVertex;

	GLuint texture;

	float width = 0.08;
	float height = 0.08;

public:
	StartScreen() {}

	StartScreen(double width, double height) {
		this->width = width;
		this->height = height;

		vectVertex.clear();
		vectVertex.push_back(Vertex(-width, -height, -5, 0, 0));	//basso dx
		vectVertex.push_back(Vertex(+width, -height, -5, 1, 0));	//basso sx
		vectVertex.push_back(Vertex(+width, +height, -5, 1, 1));	//alto sx
		vectVertex.push_back(Vertex(-width, +height, -5, 0, 1));	//alto dx

		this->loadGLTexture();
	}

	~StartScreen();

	bool loadGLTexture();
	bool drawGL(void);
};
