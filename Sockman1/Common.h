#ifndef __COMMON_H__
#define __COMMON_H__

#include "stdafx.h"

/**
 * \brief Provides coordinates for centering windows on the owner window given
 * by the handle hWndParent.
 * \param hWndParent Handle of the window to base centering on.
 * \param width Width of the window.
 * \param height Height of the window.
 * \param lpRect Address of storage which will receive the correct centering
 * coordinates.
 * \return Zero if the operation succeeded; nonzero otherwise.
 */
int CenterWindow(HWND hWndParent, int width, int height, LPRECT lpRect);

/**
 * \brief Moves the dialog box whose window handle is given by hDlg to the center of
 * its parent window, or the center of the Desktop if there is no parent.
 * \param hDlg Handle of the window to base centering on.
 * \param lpRect Address of storage which will receive the new size and position
 * of the dialog box.  Set this value to NULL if you don't care about getting
 * the new size and position values.
 * \return Zero if the operation succeeded; nonzero otherwise.
 */
int CenterDialog(HWND hDlg, LPRECT lpRect);

#endif //__COMMON_H__
