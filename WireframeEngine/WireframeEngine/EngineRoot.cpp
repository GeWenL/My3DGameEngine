
#include "EngineRoot.h"
#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h>


#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <sys/timeb.h>
#include <time.h>
#include <ddraw.h>    // directX includes


int Game_Init(void *parms)
{
	// this function is where you do all the initialization 
	// for your game

	// start up DirectDraw (replace the parms as you desire)
	DDraw_Init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, WINDOWED_APP);


	// add calls to acquire other directinput devices here...

	//// initialize directsound and directmusic
	//DSound_Init();
	//DMusic_Init();

	// hide the mouse
	if (!WINDOWED_APP)
		ShowCursor(FALSE);

	// seed random number generator
	srand(Start_Clock());

	Open_Error_File("ERROR.TXT");

	// initialize math engine
	Build_Sin_Cos_Tables();

	// initialize a single polygon
	poly1.state = POLY4DV1_STATE_ACTIVE;
	poly1.attr = 0;
	poly1.color = RGB16Bit(0, 255, 0);

	poly1.vlist[0].x = 0;
	poly1.vlist[0].y = 50;
	poly1.vlist[0].z = 0;
	poly1.vlist[0].w = 1;

	poly1.vlist[1].x = 50;
	poly1.vlist[1].y = -50;
	poly1.vlist[1].z = 0;
	poly1.vlist[1].w = 1;

	poly1.vlist[2].x = -50;
	poly1.vlist[2].y = -50;
	poly1.vlist[2].z = 0;
	poly1.vlist[2].w = 1;

	poly1.next = poly1.prev = NULL;

	// initialize the camera with 90 FOV, normalized coordinates
	Init_CAM4DV1(&cam,      // the camera object
		CAM_MODEL_EULER, // euler camera model
		&cam_pos,  // initial camera position
		&cam_dir,  // initial camera angles
		NULL,      // no initial target
		50.0,      // near and far clipping planes
		500.0,
		90.0,      // field of view in degrees
		WINDOW_WIDTH,   // size of final screen viewport
		WINDOW_HEIGHT);

	// return success
	return(1);

} // end Game_Init


int Game_Shutdown(void *parms)
{
	// return success
	return(1);
}
int Game_Main(void *parms)
{
	// return success
	return(1);
}

