/*****************************************************************************/
/*
** "BAESoundTest.h"
**
**	© Copyright 1999 Beatnik, Inc, All Rights Reserved.
**
**	Beatnik products contain certain trade secrets and confidential and
**	proprietary information of Beatnik.  Use, reproduction, disclosure
**	and distribution by any means are prohibited, except pursuant to
**	a written license from Beatnik. Use of copyright notice is
**	precautionary and does not imply publication or disclosure.
**
**	Restricted Rights Legend:
**	Use, duplication, or disclosure by the Government is subject to
**	restrictions as set forth in subparagraph (c)(1)(ii) of The
**	Rights in Technical Data and Computer Software clause in DFARS
**	252.227-7013 or subparagraphs (c)(1) and (2) of the Commercial
**	Computer Software--Restricted Rights at 48 CFR 52.227-19, as
**	applicable.
**
** Modification History:
**	12/16/99	Created.
**
*/
/*****************************************************************************/


#ifndef SOUND_TEST_H
#define SOUND_TEST_H


// SoundTest_GetNumMenuItems()
// -------------------------------------
// Returns the total number of menu selections (not including submenus).
//
int		SoundTest_GetNumMenuItems(void);


// SoundTest_GetNumSubMenuItems()
// -------------------------------------
// Returns the total number of submenu selections for a given menu.
//
int		SoundTest_GetNumSubMenuItems(int menu);


// SoundTest_GetMenuItem()
// -------------------------------------
// upon returning, outMenuText will point to the text description of menu item
// number 'menu'.  The function returns 0 if the menu asked for does not exist,
// 1 if it does.
int		SoundTest_GetMenuItem(int menu, const char **outMenuText);


// SoundTest_GetMenuItem()
// -------------------------------------
// upon returning, outMenuText will point to the text description of menu item
// number 'menu'.  The function returns 0 if the menu asked for does not exist,
// 1 if it does.
int		SoundTest_GetSubMenuItem(int menu, int subMenu, const char **outMenuText);


// SoundTest_DoMenuItem()
// -------------------------------------
// Executes the fuctionality associtated with the given menu selection.
//
void	SoundTest_DoMenuItem(int menu, int subMenu);


#endif // SOUND_TEST_H