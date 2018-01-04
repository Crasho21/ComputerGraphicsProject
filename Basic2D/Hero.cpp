#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#include "Hero.h"
#include "SOIL.h"

bool Hero::loadGLTexture() {
	/* load an image file directly as a new OpenGL texture */
	char ll[200];
	// Hero idle textures
	for (int i = 0; i < 6; i++) {
		sprintf(ll, "../Data/Hero/idle_%02d.PNG", i + 1);
		this->texture[i] = SOIL_load_OGL_texture(ll, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (texture[i] == 0) return false;
	}
	// Hero walk textures
	for (int i = 0; i < 6; i++) {
		sprintf(ll, "../Data/Hero/walk_%02d.PNG", i + 1);
		this->texture[i + 6] = SOIL_load_OGL_texture(ll, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (texture[i + 6] == 0) return false;
	}

	// Typical Texture Generation Using Data From The Bitmap
	//glBindTexture(GL_TEXTURE_2D, texture[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;										// Return Success
}

void Hero::moveX(float incrx) {
	idle = false;
	moving = true;
	for (int i = 0; i < 4; i++) {
		vector[i].x += incrx;
		reverseVector[i].x += incrx;
	}
	center.x += incrx;
}

bool Hero::drawGL(double Full_elapsed) {
	glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadIdentity();

	int heroTexF;
	if (idle) {
		heroTexF = ((int((Full_elapsed * 6))) % 6);
		glBindTexture(GL_TEXTURE_2D, texture[heroTexF]);
	}
	else if (moving) {
		heroTexF = 6 + ((int((Full_elapsed * 6))) % 6);
		glBindTexture(GL_TEXTURE_2D, texture[heroTexF]);
	}

	//  Hero geometrical trasformations
	glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
	glLoadIdentity();						// Reset The View
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);

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

	idle = true;
	moving = false;

	return true;
}

//void Hero::calcGravity(float earthY) {
//	if (vector[0].y > earthY) {
//		vector[0].y -= incrMove;
//		vector[1].y -= incrMove;
//		vector[2].y -= incrMove;
//		vector[3].y -= incrMove;
//		center.y -= incrMove;
//	} else {
//		vector[0].y = earthY;
//		vector[1].y = earthY;
//		vector[2].y =earthY + height;
//		vector[3].y = earthY + height;
//		center.y = earthY + height/2;
//	}
//}


void Hero::userMove(int leftKey, int rightKey, double limitWindow, float earthY) {
	if (vector[0].y > earthY) {	//Se sto volando
		for (int i = 0; i < 4; i++) {
			vector[i].y -= incry;
			reverseVector[i].y -= incry;
		}
		center.y -= incry;

		if (rightKey) {
			left = false;
			moveX(incrx);
		}
		else if (leftKey) {
			left = true;
			moveX(-incrx);
		}

	}
	else if ((vector[1].y - earthY)>-0.1) {	//Se il dislivello non è eccessivo
		for (int i = 0; i < 2; i++) {
			vector[i].y = earthY;
			vector[i + 2].y = earthY + height;
			reverseVector[i].y = earthY;
			reverseVector[i + 2].y = earthY + height;
		}
		center.y = earthY + height / 2;

		if (rightKey) {
			left = false;
			moveX(incry);
		}
		else if (leftKey) {
			left = true;
			moveX(-incry);
		}
	}

	if (vector[1].x < -limitWindow) {	//sbatto a sx
		vector[0].x = -limitWindow + width;
		vector[1].x = -limitWindow;
		vector[2].x = -limitWindow;
		vector[3].x = -limitWindow + width;
		center.x = -limitWindow + width / 2;
	}

	if (vector[0].x > limitWindow) {	//sbatto a dx
		vector[0].x = limitWindow;
		vector[1].x = limitWindow - width;
		vector[2].x = limitWindow - width;
		vector[3].x = limitWindow;
		center.x = limitWindow - width / 2;
	}
}

boolean Hero::userFireCommand(int keyFire) {
	if (keyFire) {
		shotFireball(powerFire, angleFire);
		return true;
	}
	return false;
}

void Hero::shotFireball(int power, int angle) {
	// Sparo della fireball a partire dalla posizione attuale dell'eroe
	fireball = new Fireball(power, angle, Coordinates(center.x, center.y + (height / 2)), z);
}

void Hero::calcolaDanno(Vertex center, float radius) { // TODO
	//se tank è entro il raggio di esplosione
	float res = squareDistance(center);
	if (squareDistance(center) < pow(radius, 2)) {
		//Togli una vita
		health = 0;
	}
}

// True se la fireball esiste, false altrimenti
/*boolean Hero::drawBullet(Earth & earthModel, Hero* tank1, Hero* tank2) {
	if (fireball != NULL) {
		Vertex bulletPosition = fireball->moveBullet();
		if (fireball->drawBullet(&earthModel, tank1, tank2)) {	//se fireball è caduto
																//Calcola danno
			tank1->calcolaDanno(fireball->getCenter(), fireball->getRadius());
			if (tank2 != NULL) tank2->calcolaDanno(fireball->getCenter(), fireball->getRadius());

			return false;
		}
		return true;
	}
	return false;
}*/

float Hero::squareDistance(Vertex other) {
	return pow((this->center.x) - other.x, 2) + pow((this->center.y) - other.y, 2);
}