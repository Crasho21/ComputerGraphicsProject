#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#include "Platform.h"
#include "SOIL.h"

bool Platform::loadGLTexture() {
	/* load an image file directly as a new OpenGL texture */
	char ll[200];
	// Earth textures
	for (int i = 0; i < 4; i++) {
		sprintf(ll, "../Data/Background/earth_%02d.PNG", i + 1);
		this->platformTexture[i] = SOIL_load_OGL_texture(ll, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (platformTexture[i] == 0) return false;
	}
	// Platform textures
	for (int i = 0; i < 4; i++) {
		sprintf(ll, "../Data/Background/mounted_platform_%02d.PNG", i + 1);
		this->platformTexture[i + 4] = SOIL_load_OGL_texture(ll, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (platformTexture[i + 4] == 0) return false;
	}
	// Snowman textures
	for (int i = 0; i < 3; i++) {
		sprintf(ll, "../Data/Background/snowman_%02d.PNG", i + 1);
		this->platformTexture[i + 8] = SOIL_load_OGL_texture(ll, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (platformTexture[i + 8] == 0) return false;
	}

	// Typical Texture Generation Using Data From The Bitmap
	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;										// Return Success
}

bool Platform::drawGL() {
	glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, platformTexture[texF]);

	//  Platform geometrical trasformations
	glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
	glLoadIdentity();						// Reset The View
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);

	glBegin(GL_QUADS);
	for (int i = 0; i < 4; i++) {
		glTexCoord2f(vector[i].u, vector[i].v);
		glVertex3f(vector[i].x, vector[i].y, vector[i].z);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);

	return true;
}