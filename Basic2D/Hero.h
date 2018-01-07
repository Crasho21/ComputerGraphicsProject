#pragma once

#include <vector>
#include <time.h>

#include "Coordinates.h"
#include "Fireball.h"
#include "Vertex.h"

#define PI 3.141592654

// Stati di hero 
const int IDLE = 0;
const int MOVE = 1;
const int ATTACK = 2;
const int FLY = 3;
const int HURT = 4;
const int DIE = 5;

const clock_t notrunning = (clock_t)(-1);

class Hero {
private:
	std::vector<Vertex> vector;
	std::vector<Vertex> reverseVector;

	GLuint heroTexture[30];

	float incrx = 0.005;
	float incry = 0.005;
	float movy = 0;
	bool left = false;				// hero watching left?
	bool up = false;
	bool attacking = false;
	bool falling = false;
	bool dead = false;
	int state = IDLE;				// State of the hero
	int fireballIndex = 0;
	int numFireball = 0;
	double movement;			// Movimento dello sfondo e degli elementi in base alla posizione dell'eroe

	float width = 0.2;
	float height = 0.2;

	float z;

	int angleFire = 0;		// Angolo di fuoco (per ora non serve)
	int powerFire = 100;	// Potenza di fuoco (per ora 100/100)

	int health = 100;		// Indicatore di punti vita

	std::vector<Fireball> fireball;
	double jumpStart = 0;

public:

	Coordinates center;

	Hero() {}
	Hero(Coordinates center, float z) {
		this->center = center;
		this->z = z;
		fireball.clear();
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

		movement = 0;

		this->loadGLTexture();
	}

	~Hero() {}

	float getHeigth() {
		return this->height;
	}

	float getWidth() {
		return this->width;
	}

	void setState(int s) {
		this->state = s;
	}

	int getState() {
		return state;
	}

	bool getDead() {
		return dead;
	}

	std::vector<Fireball> getFireball() {
		return fireball;
	}

	void setFireball(std::vector<Fireball> f) {
		this->fireball = f;
	}

	bool getAttacking() {
		return this->attacking;
	}

	bool loadGLTexture();

	double moveX();
	void moveY();
	void setXPosition(float xPos);

	bool drawGL(double Full_elapsed);

	void calcGravity(float);
	float squareDistance(Vertex other);

	double userMove(int leftKey, int rightKey, int spaceKey, int upKey, int downKey, float earthY, double Full_elapsed);
	void userChangePowerAngle(int minusPowerKey, int plusPowerKey, int minusAngleKey, int plusAngleKey);

	boolean userFireCommand(int keyFire);
	void shotFireball(int power, int angle);
	void calcolaDanno(Vertex, float);
	//boolean drawBullet(Earth &earthModel, Tank* tank1, Tank* tank2);

	int getHealth() { return health; }
	int getPowerFire() { return powerFire; }
	int getAngleFire() { return angleFire; }

	inline void recalculateVectors() {
		/*if (center.y < -0.3) center.y = -0.3;
		else if (center.y > 0.9) center.y = 0.9;*/
		vector[0].x = center.x - width / 2;
		vector[0].y = center.y - height / 2;
		vector[1].x = center.x + width / 2;
		vector[1].y = center.y - height / 2;
		vector[2].x = center.x + width / 2;
		vector[2].y = center.y + height / 2;
		vector[3].x = center.x - width / 2;
		vector[3].y = center.y + height / 2;
		reverseVector[0].x = center.x - width / 2;
		reverseVector[0].y = center.y - height / 2;
		reverseVector[1].x = center.x + width / 2;
		reverseVector[1].y = center.y - height / 2;
		reverseVector[2].x = center.x + width / 2;
		reverseVector[2].y = center.y + height / 2;
		reverseVector[3].x = center.x - width / 2;
		reverseVector[3].y = center.y + height / 2;
	}

};