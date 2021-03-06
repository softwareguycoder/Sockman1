// Definitions for error-handling and user-interaction helper functions
//

#ifndef __INTERACT_H__
#define __INTERACT_H__

#include "stdafx.h"

/**
 * \brief Asks the user a yes/no question with the specified message, and returns the response.
 * \param pszMessage Message for the user.  Should be phrased as a yes-no question.
 * \return IDYES if the user clicked the Yes button; IDNO otherwise.
 */
int AskQuestion(LPCSTR pszMessage);

/**
 * \brief Asks the user a yes/no question where 'no' means something other than
 * cancelling the pending operation.  Such as saving a file before exiting an
 * application.
 * \param pszMessage Message for the user.  Should be phrased as a yes/no question.
 * \return IDYES if the user clicked Yes, IDNO if the user clicked No, and IDCANCEL
 * otherwise.
 */
int AskQuestionWithCancel(LPCSTR pszMessage);

/**
 * \brief Shows an informational message to the user.
 * \param pszMessage The content of the message box.
 * \param pszCaption The caption (title) of the message box.
 */
void ShowInformationWithCaption(LPCSTR pszMessage, LPCSTR pszCaption);

/**
 * \brief Shows an informational message to the user.
 * \param pszMessage The content of the message box.
 * \remarks The caption of the message box is automatically set to the name
 * of this application, as given by the PROG_NAME constant.
 */
void ShowInformation(LPCSTR pszMessage);

/**
 * \brief Shows an error (i.e., fatal) message to the user.
 * \param pszMessage The content of the message box.
 * \remarks The caption of the message box is automatically set to the name
 * of this application, as given by the PROG_NAME constant.
 */
void ShowStopError(LPCSTR pszMessage);

/**
 * \brief Gets the error information from the Windows Sockets subsystem for
 * the most-recently executed socket operation that has failed.
 * \remarks Translates the result of calling WSAGetLastError() into a human-
 * readable error message.  Displays the message in a Stop message box.
 */
void HandleError();

/**
 * \brief Gets the error information from the Windows Sockets subsystem for
 * the most-recently executed socket operation that has failed.
 * \param pszMessage Additional message that should be displayed on the first
 * line of the message box, above the Windows Sockets error text.  This can
 * be used to provide additional error information to the user.
 * \remarks Translates the result of calling WSAGetLastError() into a human-
 * readable error message.  Displays the message in a Stop message box.
 */
void HandleErrorWithUserMessage(PCSTR pszMessage);

#endif //__INTERACT_H__
