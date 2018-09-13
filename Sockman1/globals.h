#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#define MAX_LOADSTRING 100

// Global Variables:
extern HINSTANCE hInstanceSockman;					// current instance
extern CHAR szTitle[MAX_LOADSTRING];				// The title bar text
extern CHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
extern HWND hWndMain;								// Handle to the main window
extern HWND hWndMainEditor;							// Handle to the main text control

extern CHAR szPrintBuffer[8192];					// Buffer to print messages to the main window with

#endif //__GLOBALS_H__
