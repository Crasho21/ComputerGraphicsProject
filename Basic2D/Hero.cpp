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
		this->heroTexture[i] = SOIL_load_OGL_texture(ll, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (heroTexture[i] == 0) return false;
	}
	// Hero walk textures
	for (int i = 0; i < 6; i++) {
		sprintf(ll, "../Data/Hero/walk_%02d.PNG", i + 1);
		this->heroTexture[i + 6] = SOIL_load_OGL_texture(ll, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (heroTexture[i + 6] == 0) return false;
	}
	// Hero attack textures
	for (int i = 0; i < 3; i++) {
		sprintf(ll, "../Data/Hero/attack_%02d.PNG", i + 1);
		this->heroTexture[i + 12] = SOIL_load_OGL_texture(ll, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (heroTexture[i + 12] == 0) return false;
	}
	// Hero die textures
	for (int i = 0; i < 10; i++) {
		sprintf(ll, "../Data/Hero/die_%02d.PNG", i + 1);
		this->heroTexture[i + 15] = SOIL_load_OGL_texture(ll, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (heroTexture[i + 15] == 0) return false;
	}
	// Hero fly textures
	for (int i = 0; i < 3; i++) {
		sprintf(ll, "../Data/Hero/fly_%02d.PNG", i + 1);
		this->heroTexture[i + 25] = SOIL_load_OGL_texture(ll, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (heroTexture[i + 25] == 0) return false;
	}

	// Typical Texture Generation Using Data From The Bitmap
	//glBindTexture(GL_TEXTURE_2D, texture[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;										// Return Success
}

double Hero::moveX() {
	if(!(center.y > -0.3)) state = MOVE;
	if (left) {
		center.x -= incrx;
	}
	else {
		center.x += incrx;
	}
	if (center.x > 0.3) {
		movement -= 0.005;
		center.x = 0.3;
	}
	else if (center.x < -0.8) {
		movement += 0.005;
		center.x = -0.8;
	}
	recalculateVectors();
	if (movement > 0) movement = 0;
	else if (movement < -8) movement = -8;
	return movement;
}

void Hero::moveY() {
	state = FLY;
	if (up) {
		center.y += incrx;
	}
	else {
		center.y -= incrx;
	}
	if (center.y < -0.3) {
		center.y = -0.3;
		state = IDLE;
	}
	else if (center.y > 0.5) {
		center.y = 0.5;
	}
	recalculateVectors();
}

bool Hero::drawGL(double Full_elapsed, audiere::OutputStreamPtr flameShot) {
	glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadIdentity();

	int heroTexF;
	int id = 0;
	movy = 0;
	double elapsed, omega, newmovy;
	Coordinates c;
	switch (state){
		case IDLE:
			heroTexF = ((int((Full_elapsed * 6))) % 6);
			glBindTexture(GL_TEXTURE_2D, heroTexture[heroTexF]);
			break;
		case MOVE:
			heroTexF = 6 + ((int((Full_elapsed * 6))) % 6);
			glBindTexture(GL_TEXTURE_2D, heroTexture[heroTexF]);
			break;
		case ATTACK:
			if (!oneFireball) {
				if (flameShot->isPlaying()) flameShot->reset();
				else flameShot->play();
				if (left) {
					c = Coordinates(center.x - 0.2, center.y);
				}
				else {
					c = Coordinates(center.x + 0.2, center.y);
				}
				fireball.push_back(Fireball(c, z + 1, left, id));
				id++;
				fireballIndex++;
				numFireball++;
				if (fireballIndex % 10 == 0) fireballIndex = 0;
				if (numFireball > 10) {
					numFireball = 10;
					fireball.erase(fireball.begin());
				}
				oneFireball = true;
			}
			heroTexF = 12 + ((int((Full_elapsed * 3))) % 3);
			glBindTexture(GL_TEXTURE_2D, heroTexture[heroTexF]);
			break;
		case FLY:
			heroTexF = 25 + ((int((Full_elapsed * 3))) % 3);
			glBindTexture(GL_TEXTURE_2D, heroTexture[heroTexF]);
			break;
		case HURT:
			break;
		case DIE:
			heroTexF = 15 + ((int((Full_elapsed * 10))) % 10);
			glBindTexture(GL_TEXTURE_2D, heroTexture[heroTexF]);
			break;
	}

	// Hero geometrical trasformations
	glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
	glLoadIdentity();						// Reset The View
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);

	// Ricalcolo i vettori per controllare che non sforino le dimensioni dello schermo (asse x)
	recalculateVectors();
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

	if (attacking) {
		for (int i = 0; i < numFireball; i++) {
			fireball[i].moveFireball();
			fireball[i].drawFireball(Full_elapsed);
		}
	}

	if (state != DIE && center.y >-0.3) state = FLY;
	else if (state != DIE) state = IDLE;
	if (heroTexF == 24) dead = true;

	return true;
}

double Hero::userMove(bool keys[], int leftKey, int rightKey, int spaceKey, int upKey, int downKey, float earthY, double Full_elapsed) {
	if (state == DIE) return false;
	if (rightKey) {
		left = false;
		movement = moveX();
	}
	else if (leftKey) {
		left = true;
		movement = moveX();
	}
	if (/*(int)(Full_elapsed * 10) % 1 == 0 &&*/ keys[VK_SPACE] && spaceKey) {
		state = ATTACK;
		attacking = true;
		//Sleep(100);
	}
	else if (!keys[VK_SPACE]) {
		oneFireball = false;
	}
	if (upKey) {
		up = true;
		moveY();
	}
	else if (downKey) {
		up = false;
		moveY();
	}
	return movement;
}