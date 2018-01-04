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

#include "Enemy.h"
#include "Fireball.h"
#include "Hero.h"
#include "StartScreen.h"
#include "Vertex.h"

#define PI 3.141592654

// Tasti
const int VK_KEY_W = 0x57;
const int VK_KEY_A = 0x41;
const int VK_KEY_S = 0x53;
const int VK_KEY_D = 0x44;

// Stati del gioco 
const int START_SCREEN = 0;
const int PLAY_SCREEN = 1;
const int WIN_SCREEN = 2;
const int LOSE_SCREEN = 3;

class MyModel {
public:
	int state = START_SCREEN;
	//  general data for window and input controls
	HDC			hDC;		        // Private GDI Device Context
	HGLRC		hRC;		        // Permanent Rendering Context
	HWND		hWnd;		        // Holds Our Window Handle
	HINSTANCE	hInstance;		// Holds The Instance Of The Application

	bool	keys[256];			// Array Used For The Keyboard Routine
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

	StartScreen startScreen;

	Fireball fireball;

	//  model data
	std::vector<Vertex> Background;   // background
	std::vector<Vertex> fire;         // floating fire
	std::vector<Vertex> bat;          // floating bat
	std::vector<Vertex> rbat;
	std::vector<Vertex> temp;
	Hero hero;
	Enemy enemy;

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
		Background.push_back(Vertex(1, -1, -5, 0.5, 0));
		Background.push_back(Vertex(1, 1, -5, 0.5, 1));
		Background.push_back(Vertex(-1, 1, -5, 0, 1));
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

		this->Tstart = this->Tstamp = clock();
		this->Full_elapsed = 0;
		this->frameTime = 0;
	}
	~MyModel() {
		this->KillFont();
	}
	bool DrawGLScene(void);
	bool InitGL(void);
	void ReSizeGLScene(int width, int height);
	void glPrint(const char *fmt, ...);			// Custom GL "Print" Routine

private:
	bool LoadGLTextures(void);
	void BuildFont(void);
	void KillFont(void);
	//  Conversion from pixel distance to float X and Y distance
	inline float PixToCoord_X(int pix) { return (2.0f * (float)pix * (float)plx) / (float)Wwidth; }
	inline float PixToCoord_Y(int pix) { return (2.0f * (float)pix * (float)ply) / (float)Wheight; }
};

extern class MyModel Data;