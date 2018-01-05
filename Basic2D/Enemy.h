#pragma once

#include <vector>

#include "Coordinates.h"
#include "Vertex.h"

#define PI 3.141592654

// Tipo di nemico 
const int BAT = 0;
const int WALKING_MONSTER = 1;
//const int WIN_SCREEN = 2;
//const int LOSE_SCREEN = 3;

class Enemy {
private:
	Coordinates center;
	std::vector<Vertex> vector;
	std::vector<Vertex> reverseVector;
	std::vector<Vertex> temp;

	GLuint enemyTexture[30];

	float width = 0.2;
	float height = 0.2;

	float z;

	int health = 100;		// Indicatore di punti vita

	int id;
	// Cambiare il raggio per aumentare lo spazio coperto dal mostro e la omega per variarne la velocità
	double radious = 0.2;
	double omega = PI / 4.0;  // PI/4 each second
	double px, py;
	double minx;
	double maxx;

public:

	Enemy() {}
	Enemy(Coordinates center, float z, int id) {
		this->center = center;
		this->z = z;
		this->id = id;
		vector.clear();
		vector.push_back(Vertex(center.x - width / 2, center.y - height / 2, z, 0, 0));	//basso dx
		vector.push_back(Vertex(center.x + width / 2, center.y - height / 2, z, 1, 0));	//basso sx
		vector.push_back(Vertex(center.x + width / 2, center.y + height / 2, z, 1, 1));	//alto sx
		vector.push_back(Vertex(center.x - width / 2, center.y + height / 2, z, 0, 1));	//alto dx
		reverseVector.clear();
		reverseVector.push_back(Vertex(center.x - width / 2, center.y - height / 2, z, 1, 0));	//basso dx
		reverseVector.push_back(Vertex(center.x + width / 2, center.y - height / 2, z, 0, 0));	//basso sx
		reverseVector.push_back(Vertex(center.x + width / 2, center.y + height / 2, z, 0, 1));	//alto sx
		reverseVector.push_back(Vertex(center.x - width / 2, center.y + height / 2, z, 1, 1));	//alto dx
		temp.clear();
		temp = reverseVector;

		this->loadGLTexture();
	}

	~Enemy() {}

	bool loadGLTexture();

	void moveX(float incrx);
	void moveX(double Full_elapsed);
	void setXPosition(float xPos);

	bool drawGL(double Full_elapsed);

	void calcGravity(float);
	float squareDistance(Vertex other);

	void calcolaDanno(Vertex, float);
	//boolean drawBullet(Earth &earthModel, Tank* tank1, Tank* tank2);

	int getHealth() { return health; }

};