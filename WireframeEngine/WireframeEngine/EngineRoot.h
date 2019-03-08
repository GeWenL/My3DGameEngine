#pragma once
#include <iostream> // include important C/C++ stuff
#include <conio.h>
#include <stdlib.h>
#include "T3DLIB1.h" // game library includes
#include "T3DLIB4.h" // game library includes
#include "Camera.H"

#define WINDOW_WIDTH      600   // size of window
#define WINDOW_HEIGHT     400
#define WINDOWED_APP      1     // 0 not windowed, 1 windowed
#define WINDOW_BPP        16    // bitdepth of window (8,16,24 etc.)
								// note: if windowed and not
								// fullscreen then bitdepth must
								// be same as system bitdepth
								// also if 8-bit the a pallete
								// is created and attached

// GLOBALS ////////////////////////////////////////////////

HWND main_window_handle = NULL; // save the window handle
HINSTANCE main_instance = NULL; // save the instance
char buffer[256];                         // used to print text

// initialize camera position and direction
POINT4D  cam_pos = { 0,0,-100,1 };
VECTOR4D cam_dir = { 0,0,0,1 };

// all your initialization code goes here...
VECTOR4D vscale = { .5,.5,.5,1 },
vpos = { 0,0,0,1 },
vrot = { 0,0,0,1 };

CAM4DV1        cam;                     // the single camera
RENDERLIST4DV1 rend_list;               // the single renderlist
POLYF4DV1      poly1;                   // our lonely polygon
POINT4D        poly1_pos = { 0,0,100,1 }; // world position of polygon



int Game_Init(void *parms = NULL);
int Game_Shutdown(void *parms = NULL);
int Game_Main(void *parms = NULL);

int DDraw_Init(int width, int height, int bpp, int windowed = 0);
