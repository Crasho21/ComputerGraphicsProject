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
		this->texture[i] = SOIL_load_OGL_texture(ll, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (texture[i] == 0) return false;
	}
	// Platform textures
	for (int i = 0; i < 4; i++) {
		sprintf(ll, "../Data/Background/mounted_platform_%02d.PNG", i + 1);
		this->texture[i + 4] = SOIL_load_OGL_texture(ll, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (texture[i + 4] == 0) return false;
	}
	// Snowman textures
	for (int i = 0; i < 3; i++) {
		sprintf(ll, "../Data/Background/Snowman_%02d.PNG", i + 1);
		this->texture[i + 8] = SOIL_load_OGL_texture(ll, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (texture[i + 8] == 0) return false;
	}

	// Typical Texture Generation Using Data From The Bitmap
	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;										// Return Success
}