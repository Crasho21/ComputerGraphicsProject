#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#include "SOIL.h"

#include "StartScreen.h"

StartScreen::~StartScreen() {}


bool StartScreen::loadGLTexture() {
	/* load an image file directly as a new OpenGL texture */
	texture = SOIL_load_OGL_texture("../Data/Background/background.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	// Typical Texture Generation Using Data From The Bitmap
	//glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;										// Return Success
}


bool StartScreen::drawGL(void) {
	glColor4f(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
	for (int i = 0; i < 4; i++) {
		//printf("%s\n%s\n", vectVertex[i].u, vectVertex[i].v);
		glTexCoord2f(vectVertex[i].u, vectVertex[i].v);
		glVertex3f(vectVertex[i].x, vectVertex[i].y, vectVertex[i].z);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);

	return true;
}