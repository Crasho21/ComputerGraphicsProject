///////////////////////////////////////////////////////////////////
//  A basic skeleton for 2D like game developpers.
//
//  model data
///////////////////////////////////////////////////////////////////

#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#include "Model.h"
#include "SOIL.h"

// All Setup For OpenGL Goes Here
bool MyModel::InitGL(void)
{
	if (!this->LoadGLTextures())				// Jump To Texture Loading Routine
	{
		return false;
	}							// If Texture Didn't Load Return FALSE


	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	this->BuildFont();

	// eye    (0,0,0)
	// center (0,0,-1)
	// up     (0,1,0)
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);

	// Initializations
	startScreen = StartScreen(plx, ply);
	hero = Hero(Coordinates(-0.8, -0.35), -4);
	enemy = Enemy(Coordinates(0.5, -0.35), -4, BAT);
	temp = bat;
	//fireball = Fireball(Coordinates(0.0, -0.35), -4, 1);

	return true;										// Initialization Went OK
}


void MyModel::ReSizeGLScene(int width, int height)
{
	if (height == 0) height = 1;					// Prevent A Divide By Zero By
	if (width == 0) width = 1;					// Prevent A Divide By Zero By

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

														// orthographic projection
														// background in [-1, 1] on both x and y
														// fill th background according to the window aspect ratio
														// void WINAPI glOrtho( GLdouble left,   GLdouble right,
														//                      GLdouble bottom, GLdouble top,
														//                      GLdouble zNear,  GLdouble zFar );
	if (width >= height) {
		this->plx = 1.0;
		this->ply = (double)height / (double)width;
	}
	else {
		this->plx = (double)width / (double)height;
		this->ply = 1.0;
	}
	glOrtho(-this->plx, this->plx, -this->ply, this->ply, 1, 5.1);

	//  saving the window width and height in pixels
	this->Wheight = height;
	this->Wwidth = width;

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}



// Load Bitmaps And Convert To Textures
bool MyModel::LoadGLTextures(void)
{
	/* load an image file directly as a new OpenGL texture */
	background[0] = SOIL_load_OGL_texture
	("../Data/Background/WinterJungleBackground.PNG",
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	if (background[0] == 0) return false;
	//char ll[200];


	// Typical Texture Generation Using Data From The Bitmap
	//glBindTexture(GL_TEXTURE_2D, background[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;										// Return Success
}

bool MyModel::DrawGLScene(void)
{
	//  TIMING - start
	clock_t t = clock();
	// elapsed time in seconds from the last draw
	double elapsed = double(t - Tstamp) / (double)CLOCKS_PER_SEC;
	// elapsed time in milliseconds from the last draw
	int ms_elapsed = (int)(t - Tstamp);
	if (ms_elapsed < 10) return true;
	// elapsed time in seconds from the beginning of the program
	this->Full_elapsed = double(t - Tstart) / (double)CLOCKS_PER_SEC;
	this->frameTime += double(t - Tstamp) / (double)CLOCKS_PER_SEC;

	this->Tstamp = t;
	//  TIMING - end
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
	glLoadIdentity();									// Reset The View

	switch (state) {
		case START_SCREEN:
			if (keys[VK_RETURN]) state = PLAY_SCREEN;

			//earthModel.drawGLEarth();

			startScreen.drawGL();

			break;

		case PLAY_SCREEN:
			//glFlush();
			glBindTexture(GL_TEXTURE_2D, background[0]);

			//  Background
			glBegin(GL_QUADS);
			for (int i = 0; i < 4; i++) {
				glTexCoord2f(Background[i].u, Background[i].v);
				glVertex3f(Background[i].x, Background[i].y, Background[i].z);
			}
			glEnd();
			//if ((int)(Full_elapsed * 10) % 2 == 0) {
				hero.userMove(keys[VK_KEY_A], keys[VK_KEY_D], keys[VK_SPACE], plx, -0.4, (int)(Full_elapsed * 10));
			//}
			hero.drawGL(Full_elapsed);
			//fireball.drawFireball(Full_elapsed);
			//enemy.moveX(Full_elapsed);
			enemy.drawGL(Full_elapsed);

			break;
	}
	/*//  Texture for the hero, change every 1/6 sec.
	int heroTexF = ((int((Full_elapsed * 6))) % 6);
	if (Data.keys[0x41]) {		// A key pressed
	for (int i = 0; i < 4; i++) {
	hero[i].x -= 0.1;
	rhero[i].x = hero[i].x;
	}
	heroTexF += 6;
	left = true;
	}
	if (Data.keys[0x44]) {		// D key pressed
	for (int i = 0; i < 4; i++) hero[i].x += 0.1;
	heroTexF += 6;
	left = false;
	}
	glBindTexture(GL_TEXTURE_2D, heroTextures[heroTexF]);

	//  Hero geometrical trasformations
	glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
	glLoadIdentity();						// Reset The View
	/*if (Data.keys[0x41]) {		// A key pressed
	for (int i = 0; i < 4; i++) {
	hero[i].x -= 0.001;
	rhero[i].x -= 0.001;
	}
	}
	if (Data.keys[0x44]) {		// D key pressed
	for (int i = 0; i < 4; i++) {
	hero[i].x += 0.001;
	rhero[i].x += 0.001;
	}
	}*/
	/*glScalef(0.05f, 0.04f, 1);    // 1- scale the hero

	//  Hero
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);

	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++) {
	if (left) {
	glTexCoord2f(rhero[i].u, rhero[i].v);
	glVertex3f(rhero[i].x, rhero[i].y, rhero[i].z);
	}
	else {
	glTexCoord2f(hero[i].u, hero[i].v);
	glVertex3f(hero[i].x, hero[i].y, hero[i].z);
	}
	}
	glEnd();

	//  Texture for the bat, change every 1/19 sec.
	/*int texF = 1 + ((int((Full_elapsed * 19))) % 8);
	glBindTexture(GL_TEXTURE_2D, background[texF]);*/
	/*
	int texF = ((int((Full_elapsed * 19))) % 8);
	glBindTexture(GL_TEXTURE_2D, enemies[texF]);

	// Bat geometrical trasformations
	glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
	glLoadIdentity();									// Reset The View

	//  circular path from window center. Radious and angular velocity
	//  in radians as follows
	// Cambiare il raggio per aumentare lo spazio coperto dal mostro e la omega per variarne la velocità
	double radious = 0.5;
	double omega = PI / 4.0;  // PI/8 each second
	double px, py;
	px = radious * cos(omega * Full_elapsed);
	double minx = bat[0].x - radious + 0.1;
	double maxx = bat[0].x + radious - 0.1;
	py = 0;
	glTranslatef((float) px, (float) py, 0);
	glScalef(0.05f, 0.04f, 1);    // 1- scale the bat

	// Bat
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);

	glBegin(GL_QUADS);
	if (bat[0].x + px >= maxx) {
	temp = rbat;
	}
	else if (rbat[0].x + px <= minx) {
	temp = bat;
	}
	for (int i = 0; i < 8; i++) {
	glTexCoord2f(temp[i].u, temp[i].v);
	glVertex3f(temp[i].x, temp[i].y, temp[i].z);
	}
	glEnd();

	*/

	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);

	//  Some text
	glMatrixMode(GL_MODELVIEW);				// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Current Modelview Matrix
	glDisable(GL_TEXTURE_2D);

	// Color
	glColor3f(1.0f, 1.0f, 1.0f);

	// Position The Text On The Screen
	glRasterPos3f(-(float)plx + PixToCoord_X(10), (float)ply - PixToCoord_Y(21),
		-4);

	// compute fps and write text
	this->frames++;
	if (this->frames > 18) {
		this->fps = frames / frameTime;
		this->frames = 0; this->frameTime = 0;
	}
	this->glPrint("Elapsed time: %6.2f sec.  -  Fps %6.2f",
		Full_elapsed, fps);

	if (this->Full_elapsed < 6) {
		glRasterPos3f(-(float)plx + PixToCoord_X(10), (float)-ply + PixToCoord_Y(21),
			-4);
		this->glPrint("F2/F3/F4 for sounds");
	}

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	return true;
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//  bitmap fonts
void MyModel::BuildFont(void)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(-20,							// Height Of Font
		0,								// Width Of Font
		0,								// Angle Of Escapement
		0,								// Orientation Angle
		FW_BOLD,						// Font Weight
		FALSE,							// Italic
		FALSE,							// Underline
		FALSE,							// Strikeout
		ANSI_CHARSET,					// Character Set Identifier
		OUT_TT_PRECIS,					// Output Precision
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		FF_DONTCARE | DEFAULT_PITCH,		// Family And Pitch
		"Courier New");					// Font Name

	oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
	wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
	SelectObject(hDC, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}

void MyModel::KillFont(void)									// Delete The Font List
{
	glDeleteLists(base, 96);							// Delete All 96 Characters
}

void MyModel::glPrint(const char *fmt, ...)					// Custom GL "Print" Routine
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}