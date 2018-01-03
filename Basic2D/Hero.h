#pragma once

#include <vector>

#include "Coordinates.h"
#include "Fireball.h"
#include "Vertex.h"

class Hero {
private:
	std::vector<Vertex> vector;
	std::vector<Vertex> reverseVector;

	GLuint texture[30];

	float incrx = 0.005;
	float incry = 0.01;
	bool left = false;				// hero watching left?
	bool idle = true;				// hero idling?
	bool moving = false;			// hero moving?

	float width = 0.2;
	float height = 0.2;

	float z;

	int angleFire = 0;		// Angolo di fuoco (per ora non serve)
	int powerFire = 100;	// Potenza di fuoco (per ora 100/100)

	int health = 100;		// Indicatore di punti vita

	Fireball *fireball;

public:

	Coordinates center;

	Hero() {}
	Hero(Coordinates center, float z) {
		//center = Coordinates(-0.5, 0.5);
		this->center = center;
		this->z = z;
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

		fireball = NULL;

		this->loadGLTexture();
	}

	~Hero() {}

	bool loadGLTexture();

	void moveX(float incr);
	void setXPosition(float xPos);

	bool drawGL(double Full_elapsed);

	void calcGravity(float);
	float squareDistance(Vertex other);

	void userMove(int leftKey, int rightKey, double limitWindow, float earthY);
	void userChangePowerAngle(int minusPowerKey, int plusPowerKey, int minusAngleKey, int plusAngleKey);

	boolean userFireCommand(int keyFire);
	void shotFireball(int power, int angle);
	void calcolaDanno(Vertex, float);
	//boolean drawBullet(Earth &earthModel, Tank* tank1, Tank* tank2);

	int getHealth() { return health; }
	int getPowerFire() { return powerFire; }
	int getAngleFire() { return angleFire; }

};