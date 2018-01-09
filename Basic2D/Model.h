///////////////////////////////////////////////////////////////////
//  A basic skeleton for 2D like game developpers.
//
//  model data
///////////////////////////////////////////////////////////////////

//  world geometry
// eye    (0,0,0)
// center (0,0,-1)
// up     (0,1,0)
// left -1, right 1
//  top & bottom centered on Y to match the window aspect ratio
// clipping planes: 1, 5.1

#pragma warning(disable: 4996)

#include <vector>
#include <time.h>
#include <math.h>

#include "audiere.h"
#include "Enemy.h"
#include "Fireball.h"
#include "Hero.h"
#include "Platform.h"
#include "Screen.h"
#include "Vertex.h"

#define PI 3.141592654

// Tasti
const int VK_KEY_W = 0x57;
const int VK_KEY_A = 0x41;
const int VK_KEY_S = 0x53;
const int VK_KEY_D = 0x44;
const int VK_KEY_H = 0x48;

// Stati del gioco 
const int START_SCREEN = 0;
const int PLAY_SCREEN = 1;
const int WIN_SCREEN = 2;
const int LOSE_SCREEN = 3;
const int HELP_SCREEN = 4;

class MyModel {
public:
	int state = START_SCREEN;
	//  general data for window and input controls
	HDC			hDC;		        // Private GDI Device Context
	HGLRC		hRC;		        // Permanent Rendering Context
	HWND		hWnd;		        // Holds Our Window Handle
	HINSTANCE	hInstance;		// Holds The Instance Of The Application

	bool	keys[256];			// Array Used For The Keyboard Routine
	bool	keysPrevious[256];			// Array Used For The Previous Keyboard Routine
	bool	active;		      // Window Active Flag Set To TRUE By Default
	bool	fullscreen;	    // Fullscreen Flag 

private:
	//  projection limits in X and Y: x in [-plx, plx], y in [-ply, ply]
	double plx = 1;
	double ply = 1;
	int Wheight, Wwidth;  // window dimensions in pixels

	int frames;           // n. of frames
	double frameTime;     // for fps computation
	double fps;

	Screen screen;

	Fireball fireball;

	//  model data
	std::vector<Vertex> Background;   // 
	double movement;
	std::vector<Vertex> fire;         // floating fire
	std::vector<Vertex> bat;          // floating bat
	std::vector<Vertex> rbat;
	Hero hero;
	std::vector<Enemy> enemy;
	Enemy e;
	int numEnemies = 0;
	std::vector<Platform> platforms;
	int numPlatforms = 0;
	Platform p;
	double posx = 0;

	clock_t Tstamp, Tstart;
	double Full_elapsed;  // elapsed time in seconds from the beginning of the program

	GLuint	background[28];			// Storage For 28 Textures!
	GLuint	enemies[100];
	GLuint	base;				// Base Display List For The Font Set

public:
	//  methods
	MyModel() : hDC(NULL), hRC(NULL), hWnd(NULL), active(true), fullscreen(true), frames(0), fps(0) {
		Background.clear();
		Background.push_back(Vertex(-1, -1, -5, 0, 0));
		Background.push_back(Vertex(9, -1, -5, 1, 0));
		Background.push_back(Vertex(9, 1, -5, 1, 1));
		Background.push_back(Vertex(-1, 1, -5, 0, 1));
		movement = 0;
		// Bat textures
		bat.clear();
		bat.push_back(Vertex(-1, -1, -5, 0, 0));
		bat.push_back(Vertex(1, -1, -5, 1, 0));
		bat.push_back(Vertex(1, 1, -5, 1, 1));
		bat.push_back(Vertex(-1, 1, -5, 0, 1));
		// Revesed texture (flipping 0 to 1 and 1 to 0)
		rbat.clear();
		rbat.push_back(Vertex(-1, -1, -5, 1, 0));
		rbat.push_back(Vertex(1, -1, -5, 0, 0));
		rbat.push_back(Vertex(1, 1, -5, 0, 1));
		rbat.push_back(Vertex(-1, 1, -5, 1, 1));
		enemy.clear();

		this->Tstart = this->Tstamp = clock();
		this->Full_elapsed = 0;
		this->frameTime = 0;
	}
	~MyModel() {
		this->KillFont();
	}
	bool DrawGLScene(audiere::OutputStreamPtr stream, audiere::OutputStreamPtr missionStart, audiere::OutputStreamPtr missionComplete, audiere::OutputStreamPtr heroDeathSound, audiere::OutputStreamPtr enemyDeathSound);
	bool InitGL(void);
	void ReSizeGLScene(int width, int height);
	void glPrint(const char *fmt, ...);			// Custom GL "Print" Routine
	void colliderFireballsEnemies(double movement, audiere::OutputStreamPtr enemyDeathSound);
	void colliderHeroEnemies(double movement);
	void colliderHeroPlatforms(double movement);

private:
	bool LoadGLTextures(void);
	void BuildFont(void);
	void KillFont(void);
	//  Conversion from pixel distance to float X and Y distance
	inline float PixToCoord_X(int pix) { return (2.0f * (float)pix * (float)plx) / (float)Wwidth; }
	inline float PixToCoord_Y(int pix) { return (2.0f * (float)pix * (float)ply) / (float)Wheight; }
};

extern class MyModel Data;