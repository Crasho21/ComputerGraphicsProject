#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#include "Enemy.h"
#include "SOIL.h"

bool Enemy::loadGLTexture() {
	/* load an image file directly as a new OpenGL texture */
	char ll[200];
	// Bat textures
	for (int i = 0; i < 8; i++) {
		sprintf(ll, "../Data/Enemies/Bat02_Fly_%02d.PNG", i);
		this->enemyTexture[i] = SOIL_load_OGL_texture(ll, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (enemyTexture[i] == 0) return false;
	}

	// Typical Texture Generation Using Data From The Bitmap
	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;										// Return Success
}

bool Enemy::drawGL(double Full_elapsed) {
	glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadIdentity();

	int TexF = id + ((int((Full_elapsed * 8))) % 8);
	glBindTexture(GL_TEXTURE_2D, enemyTexture[TexF]);

	//  Enemy geometrical trasformations
	glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
	glLoadIdentity();						// Reset The View
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);

	px = radious * cos(omega * Full_elapsed);
	minx = vector[0].x - radious + 0.1;
	maxx = vector[0].x + radious - 0.1;
	py = 0;
	glTranslatef((float)px, (float)py, 0);
	
	glBegin(GL_QUADS);
	if (vector[0].x + px >= maxx) {
		temp = reverseVector;
	}
	else if (reverseVector[0].x + px <= minx) {
		temp = vector;
	}
	for (int i = 0; i < 4; i++) {
		glTexCoord2f(temp[i].u, temp[i].v);
		glVertex3f(temp[i].x, temp[i].y, temp[i].z);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);

	return true;
}

float Enemy::squareDistance(Vertex other) {
	return pow((this->center.x) - other.x, 2) + pow((this->center.y) - other.y, 2);
}