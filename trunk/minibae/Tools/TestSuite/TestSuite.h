/*****************************************************************************/
/*
** "TestSuite.h"
**
**	© Copyright 1999 Beatnik, Inc, All Rights Reserved.
**	Written by Mark Deggeller
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
**	1/11/2000	Changed global variables to extern and defined them in
**				TestSuite.c. Caused duplication problems.
**	1/21/00		added logFile to support test logging
**
*/
/*****************************************************************************/

#ifndef TEST_SUITE_H
#define TEST_SUITE_H

#include "MiniBAE.h"
#include <stdio.h>

#define MAX_SLOTS		4

extern BAEMixer	gMixer;
extern BAESong	gSongs[MAX_SLOTS];
extern BAESound	gSounds[MAX_SLOTS];
extern FILE		*logFile;

typedef int		(*GetNumMenuItemsFn)(void);
typedef int		(*GetNumSubMenuItemsFn)(int menu);
typedef	int		(*GetMenuTextFn)(int menu, const char **outMenuText);
typedef int		(*GetSubMenuTextFn)(int menu, int subMenu, const char **outMenuText);
typedef void	(*DoMenuItemFn)(int menu, int subMenu);


void	FailedTest(const char *text);
void	LogText(char *text);

#endif // TEST_SUITE_H
