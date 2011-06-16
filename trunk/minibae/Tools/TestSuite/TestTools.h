/*****************************************************************************/
/*
** "TestTools.c"
**
**	© Copyright 1999-2000 Beatnik, Inc, All Rights Reserved.
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
**	12/19/99	Changed GetInt() to GetLong()
**	1/11/2000	Added GetPathName
**	1/14/00		Removed GetPathName
**	1/17/00		Added functions to convert BAE types to strings
**	3/15/2000	sh: Added strcmpnocase and removed use of X_API.c for ANSI compatibility.
**
*/
/*****************************************************************************/


#ifndef TEST_TOOLS_H
#define TEST_TOOLS_H

#include "MiniBAE.h"

// GetLine()
// -----------------------------
// Displays your prompt, then fills character pointer 'buffer' with
// characters from stdin, until '\n' with a max of 'length' characters.
int		GetLine(char *prompt, char *buffer, int length);


// GetDouble()
// -----------------------------
// Gets a double from stdin.  Will continue to prompt until the user
// types in a valid number.
double	GetDouble(char *prompt);


// GetInteger()
// -----------------------------
// Gets an integer from stdin.  Will continue to prompt until the user
// types in a valid number.
long	GetLong(char *prompt);


// GetBool()
// -----------------------------
// returns 1 for inputs: "y", "yes", "t", "true", "1"
// returns 0 for inputs: "n", "no", "f", "false", "0"
int		GetBool(char *prompt);


// BAEResultToStr()
// -----------------------------
// Given a BAEResult code, returns the text string equivalent.
//
char *	BAEResultToStr(BAEResult r);

// Standard strcmp, but ingores case
short int strcmpnocase(char const* s1, char const* s2);

const char *BAEBoolToYesNo(BAE_BOOL b);
const char *BAEBoolToTrueFalse(BAE_BOOL b);
char *BAECPUTypeToStr(BAECPUType t);
char *BAETerpModeToStr(BAETerpMode m);
char *BAEQualityToStr(BAEQuality q);
char *BAEAudioModifiersToStr(BAEAudioModifiers m);


#endif

