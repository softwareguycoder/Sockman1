// Definitions for commonly-used functions
//

#include "stdafx.h"
#include "Common.h"

///////////////////////////////////////////////////////////////////////////////
// Window-centering routines

// Code for this function thanks to 
// <http://msdn.microsoft.com/en-gb/library/windows/desktop/ms644996(v=vs.85).aspx>
//
int CenterDialog(HWND hDlg, LPRECT lpRect)
{
	RECT ownerRectangle;	// Will be updated with location and size of owner window
	ZeroMemory(&ownerRectangle, sizeof ownerRectangle);

	RECT dialogRectangle;	// Will be updated with current location and size of dialog box
	ZeroMemory(&dialogRectangle, sizeof dialogRectangle);

	RECT scratchRectangle;	// 'Scratch' rect for use in centering math
	ZeroMemory(&scratchRectangle, sizeof scratchRectangle);

	const auto result = 0;

	if (lpRect != nullptr)
	{
		// Initialize the output coordinates to zero if
		// the caller even wants the output coordinates
		ZeroMemory(lpRect, sizeof *lpRect);
	}

	if (hDlg == nullptr
		|| hDlg == INVALID_HANDLE_VALUE)
	{
		return -1;	// failed to center dialog we don't have the handle to
	}

	// Attempt to figure out where to center the dialog box.  If we can
	// determine the parent of this dialog box, then get a handle to that
	// window and base centering off of the parent window's location and
	// size.  Otherwise, if we are not able to determine who is the dialog
	// box's parent window, then use the Desktop.
	auto hWndOwner = GetParent(hDlg);
	if (hWndOwner == nullptr
		|| hWndOwner == INVALID_HANDLE_VALUE)
	{
		hWndOwner = GetDesktopWindow();
	}

	if ((hWndOwner = GetParent(hDlg)) == nullptr
		|| hWndOwner == INVALID_HANDLE_VALUE)
	{
		hWndOwner = GetDesktopWindow();
	}

	GetWindowRect(hWndOwner, &ownerRectangle);
	GetWindowRect(hDlg, &dialogRectangle);
	CopyRect(&scratchRectangle, &ownerRectangle);

	// Offset the owner and dialog box rectangles so that right and bottom 
	// values represent the width and height, and then offset the owner again 
	// to discard space taken up by the dialog box. 

	OffsetRect(&dialogRectangle, -dialogRectangle.left, -dialogRectangle.top);
	OffsetRect(&scratchRectangle, -scratchRectangle.left, -scratchRectangle.top);
	OffsetRect(&scratchRectangle, -dialogRectangle.right, -dialogRectangle.bottom);

	// The new position is the sum of half the remaining space and the owner's 
	// original position.

	SetWindowPos(hDlg,
		HWND_TOP,
		ownerRectangle.left + (scratchRectangle.right / 2),
		ownerRectangle.top + (scratchRectangle.bottom / 2),
		0, 0,          // Ignores size arguments. 
		SWP_NOSIZE);

	if (lpRect != nullptr)
	{
		// Store the new coordinates of the dialog box's boundary in
		// the storage pointed to by the lpRect parameter.
		GetWindowRect(hDlg, lpRect);
	}

	return result;
}

int CenterWindow(HWND hWndParent, int width, int height, LPRECT lpRect)
{
	if (lpRect == nullptr)
	{
		/* Cannot proceed if lpRect is null */
		return -1;
	}

	auto hWndOwner = hWndParent;

	if (hWndParent == INVALID_HANDLE_VALUE
		|| hWndParent == nullptr)
	{
		hWndOwner = GetDesktopWindow();
	}

	GetClientRect(hWndOwner, lpRect);
	lpRect->left = lpRect->right / 2 - width / 2;
	lpRect->top = lpRect->bottom / 2 - height / 2;

	return 0;
}