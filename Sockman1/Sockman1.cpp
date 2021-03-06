// Sockman1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Sockman1.h"
#include "Common.h"

#define IDC_STATUSBAR						1000
#define IDC_MAIN_EDITOR						2000

// Global Variables:
HINSTANCE hInstanceSockman;					// current instance
CHAR szTitle[MAX_LOADSTRING];				// The title bar text
CHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND hWndMain;								// Handle to the main window
HWND hWndMainEditor;						// Handle to the main text control

CHAR szPrintBuffer[8192];					// Buffer to print messages to the main window with

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    AboutProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	OnCommand(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param);
LRESULT CALLBACK	OnPaint(HWND hWnd);
LRESULT CALLBACK	OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);

///////////////////////////////////////////////////////////////////////////////
// Graphical user interface helpers

HWND DoCreateMainEditControl(HWND hWndParent)
{
	if (hWndParent == nullptr
		|| hWndParent == INVALID_HANDLE_VALUE)
	{
		return static_cast<HWND>(INVALID_HANDLE_VALUE);
	}

	hWndMainEditor = CreateWindowEx(
		0, "EDIT",   // predefined class
		nullptr,         // no window title
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_LEFT,
		1, 1, 100, 100,   // set size in WM_SIZE message
		hWndParent,         // parent window
		reinterpret_cast<HMENU>(IDC_MAIN_EDITOR),   // edit control ID
		hInstanceSockman,
		nullptr);        // pointer not needed

	if (hWndMainEditor == nullptr
		|| hWndMainEditor == INVALID_HANDLE_VALUE)
	{
		// failed to create edit control
		return static_cast<HWND>(INVALID_HANDLE_VALUE);
	}

	HFONT hFont = CreateFont(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Courier New"));
	
	SendMessage(hWndMainEditor, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), MAKELPARAM(TRUE, 0));

	RECT rcMainWnd;
	GetClientRect(hWndMain, &rcMainWnd);
	SetWindowPos(hWndMainEditor, nullptr, 0, 0, rcMainWnd.right,
		rcMainWnd.bottom, SWP_NOZORDER);

	return hWndMainEditor;
}

///////////////////////////////////////////////////////////////////////////////
// Windows Sockets helper function

void PaintWindow(LPSTR lpszText)
{
	lstrcpy(szPrintBuffer, lpszText);
	lstrcat(szPrintBuffer, "\r\n");

	RECT rectMainWindowClientArea;
	ZeroMemory(&rectMainWindowClientArea, sizeof rectMainWindowClientArea);

	if (!GetClientRect(hWndMain, &rectMainWindowClientArea))
		return;

	InvalidateRect(hWndMain, &rectMainWindowClientArea, true);
	UpdateWindow(hWndMain);
}

BOOL StartWinsock(VOID)
{
	WSADATA wsaData;			// Winsock implementation details

	const auto nResult = WSAStartup(MAKEWORD(1, 1), &wsaData);
	if (nResult == 0)
	{
		// Success
		PaintWindow(wsaData.szDescription);
		return TRUE;
	}

	HandleError();
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Program entry point

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, INT nCmdShow)
{
	// Only allow 1 instance to run at a time.
	if (hPrevInstance)
	{
		ShowStopError("Only one copy of this software may be running at a time.");
		return 1;
	}

	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialize global strings
	LoadStringA(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringA(hInstance, IDR_MAINFRAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	const auto hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	const auto result = static_cast<int>(msg.wParam);

	WSACleanup();	// Free all allocated program resources and exit
	return result;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXA wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOCKMAN1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEA(IDR_MAINFRAME);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExA(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInstanceSockman = hInstance; // Store instance handle in our global variable

	const auto cxWidth = 1024;
	const auto cyHeight = 768;

	RECT centeringRect;
	CenterWindow(GetDesktopWindow(), cxWidth, cyHeight, &centeringRect);

	// Get a handle to the main window after creating it
	hWndMain = CreateWindowA(szWindowClass,
		szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		centeringRect.left, centeringRect.top,
		cxWidth, cyHeight, nullptr, nullptr, hInstance, nullptr);

	if (!hWndMain)
	{
		return FALSE;
	}

	// Show the main window and re-paint it
	ShowWindow(hWndMain, nCmdShow);
	UpdateWindow(hWndMain);

	// Initialize the Winsock stack
	StartWinsock();

	szPrintBuffer[0] = '\0';

	return TRUE;
}

LRESULT CALLBACK OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	UNREFERENCED_PARAMETER(lpCreateStruct);

	DoCreateMainEditControl(hWnd);
	if (hWndMainEditor == nullptr
		|| hWndMainEditor == INVALID_HANDLE_VALUE)
		return -1;			// failed to create main editor

	return 0;		// success
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto nResult = 0;
	const auto lpCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);

	switch (message)
	{
	case WM_SIZE:
	{
		RECT rcMainWnd;
		GetClientRect(hWndMain, &rcMainWnd);
		SetWindowPos(hWndMainEditor, nullptr, 0, 0, rcMainWnd.right,
			rcMainWnd.bottom, SWP_NOZORDER);
	}
	break;

	case WM_CREATE:
		nResult = OnCreate(hWnd, lpCreateStruct);
		break;

	case WM_COMMAND:
		nResult = OnCommand(hWnd, message, wParam, lParam);
		break;

	case WM_PAINT:
		nResult = OnPaint(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return nResult;
}

void OnFileClear()
{
	// Before clearing the contents of the main window, ask
	// the user if they are sure.
	if (IDNO ==
		AskQuestion("Are you sure you want to clear the lines of text from the main window?\r\n\r\nThis action cannot be undone."))
	{
		return;
	}

	szPrintBuffer[0] = '\0';
	InvalidateRect(hWndMain, nullptr, TRUE);
	UpdateWindow(hWndMain);
}

void OnAbout(HWND hWnd)
{
	DialogBox(hInstanceSockman,
		MAKEINTRESOURCE(IDD_ABOUTBOX),
		hWnd, AboutProc);
}

void OnExit(HWND hWnd)
{
	DestroyWindow(hWnd);
}

LRESULT CALLBACK OnCommand(HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam)
{
	auto result = 0;
	const int commandID = LOWORD(wParam);
	// Parse the menu selections:
	switch (commandID)
	{
		case ID_FILE_CLEAR:
			OnFileClear();
			break;

		case IDM_ABOUT:
			OnAbout(hWnd);
			break;

		case IDM_EXIT:
			OnExit(hWnd);
			break;

		default:
			result = DefWindowProc(hWnd, nMessage, wParam, lParam);
	}
	return result;
}

LRESULT CALLBACK OnPaint(HWND hWnd)
{
	if (hWnd == nullptr
		|| hWnd == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	if (hWndMainEditor == nullptr
		|| hWndMainEditor == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	PAINTSTRUCT ps;
	//RECT rect;

	// Get a handle to the graphics device context to be used
	// for all drawing operations.
	const auto hDC = BeginPaint(hWndMain, &ps);

	//GetClientRect(hWndMain, &rect);

	// We want an 11-pt font.  The formula used here is from
	// the Windows GDI docs on MSDN
	const auto nHeight = -MulDiv(11, GetDeviceCaps(hDC, LOGPIXELSY), 72);

	const auto hFont = CreateFont(nHeight, 0, 0, 0, 300,
		false, false, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, "Courier New");

	const auto hOldFont = static_cast<HFONT>(SelectObject(hDC, hFont));

	SendMessage(hWndMainEditor, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);

	// Add text to the window.
	SendMessage(hWndMainEditor, WM_SETTEXT, 0,
		reinterpret_cast<LPARAM>(szPrintBuffer));

	SelectObject(hDC, hOldFont);

	DeleteObject(hFont);	// done using the font

	EndPaint(hWndMain, &ps);

	return 0;
}