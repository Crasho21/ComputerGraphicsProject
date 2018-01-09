#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// header file for the opengl32 library
#include <gl\glu.h>			// header file for the glu32 library
#include <math.h>

#include "Fireball.h"

#include "SOIL.h"

Fireball::Fireball(int power, int angle, Coordinates startPoint, int z) {
	center = startPoint;
	startPower = power;
	startAngle = angle;
	this->z = z;

	vector.clear();
	vector.push_back(Vertex(center.x - width / 2, center.y - height / 2, z, 0, 0));	//basso dx
	vector.push_back(Vertex(center.x + width / 2, center.y - height / 2, z, 1, 0));	//basso sx
	vector.push_back(Vertex(center.x + width / 2, center.y + height / 2, z, 1, 1));	//alto sx
	vector.push_back(Vertex(center.x - width / 2, center.y + height / 2, z, 0, 1));	//alto dx

	incrX = ((float)power / 2000);
	incrY = ((float)power / 2000);

	//isVisible = true;

	this->loadGLTexture();
}

Fireball::Fireball(float incrX, float incrY, Coordinates startPoint, int z) {
	center = startPoint;
	this->z = z;

	vector.clear();
	vector.push_back(Vertex(center.x - width / 2, center.y - height / 2, z, 0, 0));	//basso dx
	vector.push_back(Vertex(center.x + width / 2, center.y - height / 2, z, 1, 0));	//basso sx
	vector.push_back(Vertex(center.x + width / 2, center.y + height / 2, z, 1, 1));	//alto sx
	vector.push_back(Vertex(center.x - width / 2, center.y + height / 2, z, 0, 1));	//alto dx

	this->incrX = incrX;
	this->incrY = incrY;

	//isVisible = true;

	this->loadGLTexture();
}

Fireball::~Fireball() {
}

bool Fireball::loadGLTexture() {
	/* load an image file directly as a new OpenGL texture */
	char ll[200];
	// Fireball textures
	for (int i = 0; i < 3; i++) {
		sprintf(ll, "../Data/Effects/fireball_%02d.PNG", i + 1);
		this->fireballTexture[i] = SOIL_load_OGL_texture(ll, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (fireballTexture[i] == 0) return false;
	}

	// Typical Texture Generation Using Data From The Bitmap
	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;										// Return Success
}

// False se non è scoppiato il proiettile, true altrimenti
bool Fireball::drawFireball(double Full_elapsed) {
	if (!isVisible) return false;

	//Se esce dallo schermo
	/*if (center.x < earth->leftLimit - 0.1 || center.x > earth->rightLimit + 0.1) {
	isVisible = false;
	return true;
	}*/
	Coordinates fireballCenter = Coordinates(this->center.x, this->center.y);
	/*if (
	center.y < earth->earthHeight(center) ||	//tocca terra
	(firingTank == TANK1 && fireballCenter.squareDistance(tank2->center)<pow(0.1, 2)) ||
	(firingTank == TANK2 && fireballCenter.squareDistance(tank1->center)<pow(0.1, 2))
	) {	//o se si avvicina a un tank

	isVisible = false;
	earth->makeExplosion(center, radius);

	return true;
	}*/

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadIdentity();
	int texF = ((int((Full_elapsed * 3))) % 3);
	glBindTexture(GL_TEXTURE_2D, fireballTexture[texF]);
	// Fireball geometrical trasformations
	glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
	glLoadIdentity();						// Reset The View
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);

	//Proietto
	glBegin(GL_QUADS);
	for (int i = 0; i < 4; i++) {
		if (left) {
			glTexCoord2f(reverseVector[i].u, reverseVector[i].v);
			glVertex3f(reverseVector[i].x, reverseVector[i].y, reverseVector[i].z);
		}
		else {
			glTexCoord2f(vector[i].u, vector[i].v);
			glVertex3f(vector[i].x, vector[i].y, vector[i].z);
		}
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);

	return false;
}

void Fireball::moveFireball() {
	if (!isVisible) center.y = -1;

	for (int i = 0; i < 4; i++) {
		if (left) {
			reverseVector[i].x -= incrX;
		}
		else {
			vector[i].x += incrX;
		}
	}

	if (left) {
		center.x -= incrX;
	}
	else {
		center.x += incrX;
	}
}