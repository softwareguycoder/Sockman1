#include "stdafx.h"
#include "AboutDlg.h"
#include "Common.h"

INT_PTR CALLBACK OnDlgCommand(HWND hDlg, WPARAM wParam)
{
	const auto nCommand = LOWORD(wParam);

	auto result = static_cast<INT_PTR>(FALSE);

	switch(nCommand)
	{
	case IDOK:
	case IDCANCEL:
		result = static_cast<INT_PTR>(EndDialog(hDlg, nCommand));		
		break;

	default:
		break;
	}

	return result;
}

INT_PTR CALLBACK OnInitDialog(HWND hDlg)
{
	CenterDialog(hDlg, nullptr);

	return static_cast<INT_PTR>(TRUE);
}

// Message handler for about box.
INT_PTR CALLBACK AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	auto result = static_cast<INT_PTR>(FALSE);

	switch (message)
	{
	case WM_INITDIALOG:
		result = OnInitDialog(hDlg);
		break;

	case WM_COMMAND:
		result = OnDlgCommand(hDlg, wParam);
		break;

	default:
		break;
	}

	return result;
}
