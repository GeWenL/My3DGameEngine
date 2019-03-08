// DEMOII7_2.CPP
// READ THIS!
// To compile make sure to include DDRAW.LIB, DSOUND.LIB,
// DINPUT.LIB, DINPUT8.LIB, WINMM.LIB in the project link list, and of course 
// the C++ source modules T3DLIB1-5.CPP and the headers T3DLIB1-5.H
// be in the working directory of the compiler

// INCLUDES ///////////////////////////////////////////////

#define INITGUID       // make sure al the COM interfaces are available
					   // instead of this you can include the .LIB file
					   // DXGUID.LIB

#define WIN32_LEAN_AND_MEAN  

#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h>
#include <iostream> // include important C/C++ stuff
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h> 
#include <math.h>
#include <io.h>
#include <fcntl.h>

#include <ddraw.h>  // directX includes
#include <dinput.h>
#include "T3DLIB1.h" // game library includes
#include "Math.h"
#include "Input.h"
#include "MVPTrans.h"

// DEFINES ////////////////////////////////////////////////

// defines for windows interface
#define WINDOW_CLASS_NAME "WireFrameEngine"  // class name
#define WINDOW_TITLE      "WireFrameEngine-��ľ"
#define WINDOW_WIDTH      640 // size of window
#define WINDOW_HEIGHT     480

#define WINDOW_BPP        16    // bitdepth of window (8,16,24 etc.)
								// note: if windowed and not
								// fullscreen then bitdepth must
								// be same as system bitdepth
								// also if 8-bit the a pallete
								// is created and attached

#define WINDOWED_APP      1     // 0 not windowed, 1 windowed

// PROTOTYPES /////////////////////////////////////////////

// game console
int Game_Init(void *parms = NULL);
int Game_Shutdown(void *parms = NULL);
int Game_Main(void *parms = NULL);

// GLOBALS ////////////////////////////////////////////////

HWND main_window_handle = NULL; // save the window handle
HINSTANCE main_instance = NULL; // save the instance
char buffer[256];                         // used to print text

// initialize camera position and direction
POINT4D  cam_pos = { 0,0,0,1 };
VECTOR4D cam_dir = { 0,0,0,1 };

// all your initialization code goes here...
VECTOR4D vscale = { 3.0,3.0,3.0,1 },  // scale of object
vpos = { 0,0,100,1 },        // position of object
vrot = { 0,0,0,1 };        // initial orientation of object

CAM4DV1    cam;     // the single camera
OBJECT4DV1 obj;     // used to hold our cube mesh                   

// FUNCTIONS //////////////////////////////////////////////

LRESULT CALLBACK WindowProc(HWND hwnd,
	UINT msg,
	WPARAM wparam,
	LPARAM lparam)
{
	// this is the main message handler of the system
	PAINTSTRUCT	ps;		   // used in WM_PAINT
	HDC			hdc;	   // handle to a device context

	// what is the message 
	switch (msg)
	{
	case WM_CREATE:
	{
		// do initialization stuff here
		return(0);
	} break;

	case WM_PAINT:
	{
		// start painting
		hdc = BeginPaint(hwnd, &ps);

		// end painting
		EndPaint(hwnd, &ps);
		return(0);
	} break;

	case WM_DESTROY:
	{
		// kill the application			
		PostQuitMessage(0);
		return(0);
	} break;

	default:break;

	} // end switch

// process any messages that we didn't take care of 
	return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

// WINMAIN ////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE hprevinstance,
	LPSTR lpcmdline,
	int ncmdshow)
{
	// this is the winmain function

	WNDCLASS winclass;	// this will hold the class we create
	HWND	 hwnd;		// generic window handle
	MSG		 msg;		// generic message
	HDC      hdc;       // generic dc
	PAINTSTRUCT ps;     // generic paintstruct

	// first fill in the window class stucture
	winclass.style = CS_DBLCLKS | CS_OWNDC |
		CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;

	// register the window class
	if (!RegisterClass(&winclass))
		return(0);

	// create the window, note the test to see if WINDOWED_APP is
	// true to select the appropriate window flags
	if (!(hwnd = CreateWindow(WINDOW_CLASS_NAME, // class
		WINDOW_TITLE,	 // title
		(WINDOWED_APP ? (WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION) : (WS_POPUP | WS_VISIBLE)),
		0, 0,	   // x,y
		WINDOW_WIDTH,  // width
		WINDOW_HEIGHT, // height
		NULL,	   // handle to parent 
		NULL,	   // handle to menu
		hinstance,// instance
		NULL)))	// creation parms
		return(0);

	// save the window handle and instance in a global
	main_window_handle = hwnd;
	main_instance = hinstance;

	// resize the window so that client is really width x height
	if (WINDOWED_APP)
	{
		// now resize the window, so the client area is the actual size requested
		// since there may be borders and controls if this is going to be a windowed app
		// if the app is not windowed then it won't matter
		RECT window_rect = { 0,0,WINDOW_WIDTH - 1,WINDOW_HEIGHT - 1 };

		// make the call to adjust window_rect
		AdjustWindowRectEx(&window_rect,
			GetWindowStyle(main_window_handle),
			GetMenu(main_window_handle) != NULL,
			GetWindowExStyle(main_window_handle));

		// save the global client offsets, they are needed in DDraw_Flip()
		window_client_x0 = -window_rect.left;
		window_client_y0 = -window_rect.top;

		// now resize the window with a call to MoveWindow()
		MoveWindow(main_window_handle,
			0, // x position
			0, // y position
			window_rect.right - window_rect.left, // width
			window_rect.bottom - window_rect.top, // height
			FALSE);

		// show the window, so there's no garbage on first render
		ShowWindow(main_window_handle, SW_SHOW);
	} // end if windowed

	// perform all game console specific initialization
	Game_Init();

	// disable CTRL-ALT_DEL, ALT_TAB, comment this line out 
	// if it causes your system to crash
	SystemParametersInfo(SPI_SCREENSAVERRUNNING, TRUE, NULL, 0);

	// enter main event loop
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// test if this is a quit
			if (msg.message == WM_QUIT)
				break;

			// translate any accelerator keys
			TranslateMessage(&msg);

			// send the message to the window proc
			DispatchMessage(&msg);
		} // end if

	// main game processing goes here
		Game_Main();

	} // end while

// shutdown game and release all resources
	Game_Shutdown();

	// enable CTRL-ALT_DEL, ALT_TAB, comment this line out 
	// if it causes your system to crash
	SystemParametersInfo(SPI_SCREENSAVERRUNNING, FALSE, NULL, 0);

	// return to Windows like this
	return(msg.wParam);

} // end WinMain

// T3D II GAME PROGRAMMING CONSOLE FUNCTIONS ////////////////

int Game_Init(void *parms)
{
	// this function is where you do all the initialization 
	// for your game

	// start up DirectDraw (replace the parms as you desire)
	DDraw_Init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, WINDOWED_APP);

	// hide the mouse
	if (!WINDOWED_APP)
		ShowCursor(FALSE);

	// seed random number generator
	srand(Start_Clock());

	Open_Error_File("ERROR.TXT");

	// initialize math engine
	Build_Sin_Cos_Tables();

	// initialize the camera with 90 FOV, normalized coordinates
	Init_CAM4DV1(&cam,      // the camera object
		CAM_MODEL_EULER, // the euler model
		&cam_pos,  // initial camera position
		&cam_dir,  // initial camera angles
		NULL,      // no target
		50.0,      // near and far clipping planes
		500.0,
		120.0,      // field of view in degrees
		WINDOW_WIDTH,   // size of final screen viewport
		WINDOW_HEIGHT);

	// load the cube
	Load_OBJECT4DV1_PLG(&obj, "cube1.plg", &vscale, &vpos, &vrot);

	// return success
	return(1);

} // end Game_Init

///////////////////////////////////////////////////////////

int Game_Shutdown(void *parms)
{
	// this function is where you shutdown your game and
	// release all resources that you allocated

	// shutdown directdraw last
	DDraw_Shutdown();
	Close_Error_File();
	return(1);
} // end Game_Shutdown

//////////////////////////////////////////////////////////

int Game_Main(void *parms)
{
	// this is the workhorse of your game it will be called
	// continuously in real-time this is like main() in C
	// all the calls for you game go here!

	

	static float x_ang = 0, // these track the rotation
		y_ang = 0, // of the object
		z_ang = 0;

	int index = 0; // looping var

	// start the timing clock
	Start_Clock();

	// ��վ�����
	// clear the drawing surface 
	DDraw_Fill_Surface(lpddsback, 0);
	// game logic here...

	// reset the object (this only matters for backface and object removal)
	Reset_OBJECT4DV1(&obj);

	static MATRIX4X4 mRot; // general rotation matrix
	static MATRIX4X4 mScale;
	static MATRIX4X4 mTrans;
	static MATRIX4X4 mTemp;
	static MATRIX4X4 mSRT;
	// generate rotation matrix around y axis
	Build_XYZ_Rotation_MATRIX4X4(x_ang, y_ang--, z_ang, &mRot);
	Build_Model_To_World_MATRIX4X4(&vpos, &mTrans);
	Mat_Init_4X4(&mScale, vscale.x, 0, 0, 0,
		0, vscale.y, 0, 0,
		0, 0, vscale.z, 0,
		0, 0, 0, 1);
	Mat_Mul_4X4(&mScale,&mRot, &mTemp);
	Mat_Mul_4X4(&mTemp, &mTrans, &mSRT);
	Transform_OBJECT4DV1(&obj, &mSRT, TRANSFORM_LOCAL_TO_TRANS, 1);

	// generate camera matrix
	Build_CAM4DV1_Matrix_Euler(&cam, CAM_ROT_SEQ_ZYX);

	// remove backfaces
	Remove_Backfaces_OBJECT4DV1(&obj, &cam);

	MATRIX4X4 mVP; // view���� * Projection���� * Screen����
	MATRIX4X4 mPer;
	MATRIX4X4 mScr;
	MATRIX4X4 mPerScr;// Projection���� * Screen����
	Build_Camera_To_Perspective_MATRIX4X4(&cam, &mPer);
	Build_Perspective_To_Screen_MATRIX4X4(&cam, &mScr);
	Mat_Mul_4X4(&mPer, &mScr, &mPerScr);
	Mat_Mul_4X4(&(&cam)->mcam, &mPerScr, &mVP);
	Transform_OBJECT4DV1(&obj, &mVP, TRANSFORM_TRANS_ONLY, 1);
	Convert_From_Homogeneous4D_OBJECT4DV1(&obj);

	// lock the back buffer
	DDraw_Lock_Back_Surface();

	// render the object
	Draw_OBJECT4DV1_Wire16(&obj, back_buffer, back_lpitch);

	// unlock the back buffer
	DDraw_Unlock_Back_Surface();

	// flip the surfaces
	DDraw_Flip();

	// sync to 30ish fps
	Wait_Clock(30);

	//// check of user is trying to exit
	//if (KEY_DOWN(VK_ESCAPE) || keyboard_state[DIK_ESCAPE])
	//{
	//	PostMessage(main_window_handle, WM_DESTROY, 0, 0);
	//} // end if

// return success
	return(1);

} // end Game_Main

//////////////////////////////////////////////////////////