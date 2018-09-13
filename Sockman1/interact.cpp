// Implementations of error-handling and user-interaction helper functions
//

#include "stdafx.h"
#include "globals.h"
#include "interact.h"
#include <cstdio>

/**
 * \brief Shows an informational message to the user.
 * \param pszMessage The content of the message box.
 * \param pszCaption The caption (title) of the message box.
 */
void ShowInformationWithCaption(LPCSTR pszMessage, LPCSTR pszCaption)
{
	if (pszMessage == nullptr
		|| strlen(pszMessage) == 0)
	{
		return;
	}

	if (pszCaption == nullptr
		|| strlen(pszCaption) == 0)
	{
		return;
	}

	MessageBoxA(nullptr, pszMessage, pszCaption, MB_OK | MB_ICONINFORMATION);
}

/**
 * \brief Shows an informational message to the user.
 * \param pszMessage The content of the message box.
 * \remarks The caption of the message box is automatically set to the name
 * of this application, as given by the szTitle global variable.
 */
void ShowInformation(LPCSTR pszMessage)
{
	if (pszMessage == nullptr
		|| strlen(pszMessage) == 0)
	{
		return;
	}

	MessageBoxA(nullptr, pszMessage, szTitle, MB_OK | MB_ICONINFORMATION);
}

/**
 * \brief Shows an error (i.e., fatal) message to the user.
 * \param pszMessage The content of the message box.
 * \remarks The caption of the message box is automatically set to the name
 * of this application, as given by the szTitle global variable.
 */
void ShowStopError(LPCSTR pszMessage)
{
	if (pszMessage == nullptr
		|| strlen(pszMessage) == 0)
	{
		return;
	}

	MessageBoxA(nullptr, pszMessage, szTitle, MB_OK | MB_ICONSTOP);
}

/**
 * \brief Gets the error information from the Windows Sockets subsystem for
 * the most-recently executed socket operation that has failed.
 * \remarks Translates the result of calling WSAGetLastError() into a human-
 * readable error message.  Displays the message in a Stop message box.
 */
void HandleError()
{
	char *s = nullptr;

	// Translates the output of the WSAGetLastError() function into a
	// human-understandable message.
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&s), 0, nullptr);

	ShowStopError(s);

	LocalFree(s);
}

/**
 * \brief Gets the error information from the Windows Sockets subsystem for
 * the most-recently executed socket operation that has failed.
 * \param pszMessage Additional message that should be displayed on the first
 * line of the message box, above the Windows Sockets error text.  This can
 * be used to provide additional error information to the user.
 * \remarks Translates the result of calling WSAGetLastError() into a human-
 * readable error message.  Displays the message in a Stop message box.
 */
void HandleErrorWithUserMessage(PCSTR pszMessage)
{
	if (pszMessage == nullptr
		|| strlen(pszMessage) == 0)
		return;

	char szBuffer[512];
	char *s = nullptr;

	// Translates the output of the WSAGetLastError() function into a
	// human-understandable message.
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&s), 0, nullptr);

	sprintf_s(szBuffer, "%s\r\n\r\n%s", pszMessage, s);

	ShowStopError(szBuffer);

	LocalFree(s);
}