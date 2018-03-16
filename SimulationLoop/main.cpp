#include <stdio.h>
#include <Windows.h>
#include "Game.h"

const char TITLE[] = "Window Creation";

Game *game;

//********************OpenGL Initialization********************************
#include <gl\gl.h>                                // Header File For The OpenGL32 Library
#include <gl\glu.h>                               // Header File For The GLu32 Library
HDC			hDC=NULL;		// Private GDI Device Context
//*****************************************************************************

//************************ MESSAGE HANDLER **************************

LRESULT CALLBACK WindowProc(HWND hwnd, 
	UINT msg, 
	WPARAM wparam, 
	LPARAM lparam)
{
	// this is the main message handler of the system
	PAINTSTRUCT		ps;	// used in WM_PAINT
	HDC				hdc;	// handle to a device context

	// what is the message 
	switch(msg)
	{	
	case WM_CREATE: 
		{
			// do initialization stuff here
			return(0);
		} 

	case WM_PAINT: 
		{
			// validate the window
			hdc = BeginPaint(hwnd,&ps);	 
			EndPaint(hwnd,&ps);
			return(0);
		} 

	case WM_KEYDOWN:
		{
			// Handle any non-accelerated key commands
			switch (wparam)
			{
			case 0x31:	game->AddNewSphere();											//1
				break;
			case 0x33: game->RemoveTopTray();											//3
				break;
			case 0x34: game->AddTopTray();												//4
				break;
			case 0x35: game->RemoveBottomTray();										//5
				break;
			case 0x36: game->AddBottomTray();											//6
				break;
			case 0x50: game->PauseGame();												//P
				break;
			case 0x52: game->ResetGame();												//R
				break;
			case 0x55: game->ChangeTimescale(2.0f);										//U
				break;
			case 0x4A: game->ChangeTimescale(0.5f);										//J
				break;
			case 0x57: game->MoveCameraY(0.05f);										//W
				break;
			case 0x53: game->MoveCameraY(-0.05f);										//S
				break;
			case 0x41: game->MoveCameraX(-0.05f);										//A
				break;
			case 0x44: game->MoveCameraX(0.05f);							 			//D
				break;
			case VK_UP: game->ZoomCamera(-5.0f);										//UP ARROW
				break;
			case VK_DOWN: game->ZoomCamera(5.0f);										//DOWN ARROW
				break;
			case 0x64:																	//NUMPAD_4
			case VK_LEFT: game->RotateCameraY(5.0f);									//LEFT ARROW
				break;
			case 0x66:																	//NUMPAD_6
			case VK_RIGHT: game->RotateCameraY(-5.0f);									//RIGHT ARROW
				break;
			case 0x68: game->RotateCameraX(5.0f);										//NUMPAD_8
				break;
			case 0x62: game->RotateCameraX(-5.0f);										//NUMPAD_2
				break;
			case 0x65: game->ResetCamera();												//NUMPAD_5
				break;
			case 0x4F: game->ChangeElasticity(0.1f);									//O
				break;
			case 0x4C: game->ChangeElasticity(-0.1f);									//L
				break;
			case VK_BACK: game->RemoveLastItem();										//Backspace
				break;
			case 0x49: game->ChangeFriction(0.01f);										//I
				break;
			case 0x4B: game->ChangeFriction(-0.01f);									//K
				break;
			case VK_ESCAPE:
			case VK_F12:
				PostMessage(hwnd, WM_CLOSE, 0, 0);
				return (0);
			default:
				char message[15];
				sprintf_s(message, "Key Pressed: %c", (char)wparam);
				MessageBox(NULL, message, "Key Pressed", MB_OK);
			}
			break;
		}

	case WM_DESTROY: 
		{
			// kill the application			
			PostQuitMessage(0);

			return(0);
		}

	default:
		break;

	} // end switch

	// process any messages that we didn't take care of 
	return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc



//**************************Setup OpenGL***********************
void InitializeOpenGL(HWND hwnd, int width, int height)
{ 
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	HGLRC		hRC=NULL;		// Permanent Rendering Context

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC=GetDC(hwnd)))							// Did We Get A Device Context?
	{
		MessageBox(NULL, "Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		MessageBox(NULL, "Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		MessageBox(NULL, "Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	}

	ShowWindow(hwnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hwnd);						// Slightly Higher Priority
	SetFocus(hwnd);									// Sets Keyboard Focus To The Window

	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	//gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);
	//glOrtho(-25,25,-25,25,-10, 10);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
}




//************************ WIN MAIN***********************

int WINAPI WinMain( HINSTANCE hinstance,
	HINSTANCE hprevinstance,
	LPSTR lpcmdline,
	int ncmdshow)
{
	WNDCLASS	winclass;	// this will hold the class we create
	HWND		hwnd;		// generic window handle
	MSG			msg;		// generic message

	// first fill in the window class stucture
	winclass.style			= CS_HREDRAW | CS_VREDRAW;                  
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinstance;
	winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL;
	winclass.lpszClassName	= "WindowCreation";

	// register the window class
	if (!RegisterClass(&winclass))
		return(0);

	// create the window
	if (!(hwnd = CreateWindow( "WindowCreation", // class
		TITLE,	     // title
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0,
		0,
		//Set the size of the window to the size of the screen 
		800,
		800,
		//GetSystemMetrics(SM_CXSCREEN),
		//GetSystemMetrics(SM_CYSCREEN),
		NULL,	   // handle to parent 
		NULL,	   // handle to menu
		hinstance,	// instance
		NULL)))	// creation parms
		return(0);

	InitializeOpenGL(hwnd, 800, 800);

	game = new Game(hDC);

	// enter main event loop
	bool quit = false;
	while(!quit)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
			// test if this is a quit
			if (msg.message == WM_QUIT) quit = true;

			// translate any accelerator keys
			TranslateMessage(&msg);
			// send the message to the window proc
			DispatchMessage(&msg);
		} // end if
		else {
			game->Update();
		}

	} // end while

	delete game;

	// return to Windows like this
	return(msg.wParam);

} // end WinMain


//************************ END OF WIN MAIN ***********************
