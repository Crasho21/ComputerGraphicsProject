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
	// Screen per gestire i vari sfondi
	screen = Screen(plx, ply);
	// Hero
	hero = Hero(Coordinates(-0.8, -0.3), -3);
	e = Enemy(Coordinates(0.5, -0.3), -4, BAT);
	enemy.push_back(e);
	numEnemies++;
	// Enemies
	e = Enemy(Coordinates(0.5, 0.25), -4, WALKING_MONSTER);
	enemy.push_back(e);
	numEnemies++;
	e = Enemy(Coordinates(1.5, -0.15), -4, BAT);
	enemy.push_back(e);
	numEnemies++;
	e = Enemy(Coordinates(2.0, 0.1), -4, BAT);
	enemy.push_back(e);
	numEnemies++;
	e = Enemy(Coordinates(2.5, -0.3), -4, WALKING_MONSTER);
	enemy.push_back(e);
	numEnemies++;
	e = Enemy(Coordinates(3.5, 0.45), -4, WALKING_MONSTER);
	enemy.push_back(e);
	numEnemies++;
	e = Enemy(Coordinates(4.0, -0.3), -4, WALKING_MONSTER);
	enemy.push_back(e);
	numEnemies++;
	e = Enemy(Coordinates(4.5, 0.4), -4, BAT);
	enemy.push_back(e);
	numEnemies++;
	e = Enemy(Coordinates(5.0, 0.2), -4, BAT);
	enemy.push_back(e);
	numEnemies++;
	e = Enemy(Coordinates(4.5, 0.0), -4, BAT);
	enemy.push_back(e);
	numEnemies++;
	e = Enemy(Coordinates(5.0, -0.2), -4, BAT);
	enemy.push_back(e);
	numEnemies++;
	e = Enemy(Coordinates(5.5, -0.2), -4, BAT);
	enemy.push_back(e);
	numEnemies++;
	e = Enemy(Coordinates(6.0, 0.0), -4, BAT);
	enemy.push_back(e);
	numEnemies++;
	e = Enemy(Coordinates(6.5, 0.2), -4, BAT);
	enemy.push_back(e);
	numEnemies++;
	e = Enemy(Coordinates(7.0, 0.4), -4, BAT);
	enemy.push_back(e);
	numEnemies++;
	// Piattaforma su cui poggia il giocatore
	p = Platform(Coordinates(0.0, -0.59), 2, 0.4, -4, 0, 0);
	platforms.push_back(p);
	numPlatforms++;
	// Piattafrme fluttuanti
	p = Platform(Coordinates(0.5, 0.1), 0.5, 0.1, -4, 3, 1);
	platforms.push_back(p);
	numPlatforms++;
	p = Platform(Coordinates(3.5, 0.3), 0.5, 0.1, -4, 3, 1);
	platforms.push_back(p);
	numPlatforms++;

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

bool MyModel::DrawGLScene(audiere::OutputStreamPtr stream, audiere::OutputStreamPtr missionStart, audiere::OutputStreamPtr missionComplete, audiere::OutputStreamPtr flameShot, audiere::OutputStreamPtr heroDeathSound, audiere::OutputStreamPtr enemyDeathSound)
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
			if (keys[VK_RETURN]) {
				missionStart->play();
				state = PLAY_SCREEN;
			}
			else if (keys[VK_KEY_H]) state = HELP_SCREEN;
			screen.drawGL(START);
			break;

		case PLAY_SCREEN:
			if (!stream->isPlaying()) stream->play();
			if (hero.getDead()) {
				heroDeathSound->play();
				state = LOSE_SCREEN;
			}
			glBindTexture(GL_TEXTURE_2D, background[0]);
			glTranslatef((float)movement, 0, 0);
			//  Background
			glBegin(GL_QUADS);
			for (int i = 0; i < 4; i++) {
				glTexCoord2f(Background[i].u, Background[i].v);
				glVertex3f(Background[i].x, Background[i].y, Background[i].z);
			}
			glEnd();
			movement = hero.userMove(Data.keys, keys[VK_KEY_A], keys[VK_KEY_D], keys[VK_SPACE], keys[VK_KEY_W], keys[VK_KEY_S], -0.4, Full_elapsed);
			if (movement == -8) {
				missionComplete->play();
				state = WIN_SCREEN;
			}
			hero.drawGL(Full_elapsed, flameShot);
			for (int i = 0; i < numEnemies; i++) {
				enemy[i].drawGL(Full_elapsed, movement);
			}
			for (int i = 0; i < numPlatforms; i++) {
				platforms[i].drawGL(movement);
			}
			if (hero.getAttacking()) {
				colliderFireballsEnemies(movement, enemyDeathSound);
			}
			colliderHeroEnemies(movement);
			colliderHeroPlatforms(movement);
			break;

		case WIN_SCREEN:
			if (stream->isPlaying()) stream->reset();
			screen.drawGL(WIN);
			break;

		case LOSE_SCREEN:
			if (stream->isPlaying()) stream->reset();
			screen.drawGL(LOSE);
			break;

		case HELP_SCREEN:
			if (keys[VK_RETURN]) {
				missionStart->play();
				state = PLAY_SCREEN;
			}
			screen.drawGL(HELP);
			break;
	}

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

void MyModel::colliderFireballsEnemies(double movement, audiere::OutputStreamPtr enemyDeathSound) {
	std::vector<Fireball> f = hero.getFireball();
	for (int i = 0; i < f.size(); i++) {
		for (int j = 0; j < enemy.size(); j++) {
			if (enemy[j].getIsVisible()) {
				if (f[i].getCenter().y - (f[i].getHeigth() / 2) <= enemy[j].getCenter().y + (enemy[j].getHeigth() / 2) &&		// Il bordo inferiore della fireball deve essere <= del bordo superiore dell'enemy
					f[i].getCenter().y + (f[i].getHeigth() / 2) >= enemy[j].getCenter().y - (enemy[j].getHeigth() / 2)) {		// Il bordo superiore della fireball deve essere >= del bordo inferiore dell'enemy
					if (enemy[j].getPx() != 0) {
						posx = enemy[j].getPx();
					}
					if (f[i].getCenter().x - (f[i].getWidth() / 2) <= enemy[j].getCenter().x + posx + movement + (enemy[j].getWidth() / 2) &&		// Il bordo sinistro della fireball deve essere <= del bordo destro dell'enemy
						f[i].getCenter().x + (f[i].getWidth() / 2) >= enemy[j].getCenter().x + posx + movement - (enemy[j].getWidth() / 2)) {		// Il bordo destro della fireball deve essere >= del bordo sinistro dell'enemy
						f[i].setIsVisible(false);
						hero.setFireball(f);
						enemy[j].setDead(true);
						enemy[j].setIsVisible(false);
						enemyDeathSound->play();
						//TODO Sostituire con esplosione
						//p = Platform(Coordinates(enemy[j].getCenter().x + posx + movement, enemy[j].getCenter().y), 0.1, 0.1, -3, 7, 2);
						//p.drawGL();
					}
				}
			}
		}
	}
}

void MyModel::colliderHeroEnemies(double movement) {
	for (int j = 0; j < enemy.size(); j++) {
		if (enemy[j].getIsVisible()) {
			if (hero.center.y - (hero.getHeigth() / 2) <= enemy[j].getCenter().y + (enemy[j].getHeigth() / 2) &&		// Il bordo inferiore della fireball deve essere <= del bordo superiore dell'enemy
				hero.center.y + (hero.getHeigth() / 2) >= enemy[j].getCenter().y - (enemy[j].getHeigth() / 2)) {		// Il bordo superiore della fireball deve essere >= del bordo inferiore dell'enemy
				if (enemy[j].getPx() != 0) {
					posx = enemy[j].getPx();
				}
				if (hero.center.x - (hero.getWidth() / 2) <= enemy[j].getCenter().x + posx + movement + (enemy[j].getWidth() / 2) &&		// Il bordo sinistro della fireball deve essere <= del bordo destro dell'enemy
					hero.center.x + (hero.getWidth() / 2) >= enemy[j].getCenter().x + posx + movement - (enemy[j].getWidth() / 2)) {		// Il bordo destro della fireball deve essere >= del bordo sinistro dell'enemy
					hero.setState(DIE);
					enemy[j].setDead(true);
				}
			}
		}
	}
}

void MyModel::colliderHeroPlatforms(double movement) {
	double epsilon = 0.005;
	for (int j = 1; j < platforms.size(); j++) {
		if (hero.center.y - (hero.getHeigth() / 2) <= platforms[j].getCenter().y + (platforms[j].getHeigth() / 2) &&					// Il bordo inferiore del hero deve essere <= del bordo superiore della platform
			hero.center.y + (hero.getHeigth() / 2) >= platforms[j].getCenter().y - (platforms[j].getHeigth() / 2)) {					// Il bordo superiore del hero deve essere >= del bordo inferiore della platform
			if (hero.center.x + (hero.getWidth() / 2) > platforms[j].getCenter().x + movement - (platforms[j].getWidth() / 2) - epsilon &&
				hero.center.x + (hero.getWidth() / 2) < platforms[j].getCenter().x + movement - (platforms[j].getWidth() / 2) + epsilon) {		// Il bordo destro del hero deve essere > del bordo sinistro della platform
				hero.center.x = platforms[j].getCenter().x + movement - (platforms[j].getWidth() / 2) - (hero.getWidth() / 2) - epsilon;
			}
			else if (hero.center.x - (hero.getWidth() / 2) > platforms[j].getCenter().x + movement + (platforms[j].getWidth() / 2) - epsilon &&
					 hero.center.x - (hero.getWidth() / 2) < platforms[j].getCenter().x + movement + (platforms[j].getWidth() / 2) + epsilon) {		// Il bordo destro della fireball deve essere >= del bordo sinistro dell'enemy
				hero.center.x = platforms[j].getCenter().x + movement + (platforms[j].getWidth() / 2) + (hero.getWidth() / 2) + epsilon;
			}
		}
		if (hero.center.x + (hero.getWidth() / 2) > platforms[j].getCenter().x + movement - (platforms[j].getWidth() / 2) &&
			hero.center.x - (hero.getWidth() / 2) < platforms[j].getCenter().x + movement + (platforms[j].getWidth() / 2)) {
			if (hero.center.y + (hero.getHeigth() / 2) > platforms[j].getCenter().y - (platforms[j].getHeigth() / 2) - epsilon &&					// Il bordo inferiore del hero deve essere <= del bordo superiore della platform
				hero.center.y + (hero.getHeigth() / 2) < platforms[j].getCenter().y - (platforms[j].getHeigth() / 2) + epsilon) {
				hero.center.y = platforms[j].getCenter().y - (platforms[j].getHeigth() / 2) - (hero.getHeigth() / 2) - epsilon;
			}
			else if(hero.center.y - (hero.getHeigth() / 2) > platforms[j].getCenter().y + (platforms[j].getHeigth() / 2) - epsilon &&				// Il bordo inferiore del hero deve essere <= del bordo superiore della platform
					hero.center.y - (hero.getHeigth() / 2) < platforms[j].getCenter().y + (platforms[j].getHeigth() / 2) + epsilon) {				// Il bordo destro della fireball deve essere >= del bordo sinistro dell'enemy
				hero.center.y = platforms[j].getCenter().y + (platforms[j].getHeigth() / 2) + (hero.getHeigth() / 2) + epsilon;
			}
		}
	}
}