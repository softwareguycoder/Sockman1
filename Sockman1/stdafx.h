// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef __STDAFX_H__
#define __STDAFX_H__

#include "targetver.h"

#undef UNICODE							// We only want ANSI/ASCII here
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <winsock.h>
#include <commctrl.h>

#include "globals.h"
#include "interact.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "Ws2_32.lib")

// reference additional headers your program requires here

#endif //__STDAFX_H__