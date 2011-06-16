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
**				All functions handle 'comments',  anything after "//" is ignored
**	1/17/00		Added functions to convert BAE types to strings
**	1/20/00		Removed extra ; at end of BAEQualityToStr
**	1/21/00		GetLine() now calls LogText() to support test logging.
**  3/6/00		MSD: Added support for 32kHz, 40kHz
**	3/15/2000	sh: Added strcmpnocase and removed use of X_API.c for ANSI compatibility.
**				sh:	Removed mac include
*/
/*****************************************************************************/



#include <stdio.h>
#include <string.h>
#include "TestTools.h"
#include "TestSuite.h"



#define LOWER_CASE(c)	 ( ((c >= 'A') && (c <= 'Z')) ? c | 0x20 : c)

// standard strcmp, but ignore case
short int strcmpnocase(char const* s1, char const* s2)
{
	if (s1 == NULL)
	{
		s1 = "";
	}
	if (s2 == NULL)
	{
		s2 = "";
	}
	
	while (1) 
	{
		if (LOWER_CASE(*s1) == LOWER_CASE(*s2)) 
		{
			if (*s1 == 0) 
			{
				return 0;
			} 
			else 
			{
				s1++;
				s2++;
			}
		} 
		else if (LOWER_CASE(*s1) > LOWER_CASE(*s2)) 
		{
			return 1;
		} 
		else 
		{
			return -1;
		}
	}
}

int GetBool(char *prompt)
{
	int result;
	char	buffer[256];

	result = 0;
	while (1)
	{
		GetLine(prompt, buffer, 256);
		if (
			(strcmpnocase(buffer,"y")==0) ||
			(strcmpnocase(buffer,"yes")==0) ||
			(strcmpnocase(buffer,"true")==0) ||
			(strcmpnocase(buffer,"t")==0) ||
			(strcmpnocase(buffer,"1")==0) )
		{
			result = 1;
			break;
		}
		else if (
			(strcmpnocase(buffer,"n")==0) ||
			(strcmpnocase(buffer,"no")==0) ||
			(strcmpnocase(buffer,"false")==0) ||
			(strcmpnocase(buffer,"f")==0) ||
			(strcmpnocase(buffer,"0")==0) )
		{
			result = 0;
			break;
		}
		printf("Try again:\n");
	}
	return result;
}


double GetDouble(char *prompt)
{
	double	result;
	char	termChar;
	char	buffer[256];
	
	while (1)
	{
		GetLine(prompt, buffer, 256);
		switch (sscanf(buffer, "%lf%c", &result, &termChar))
		{
			case 1:
				return result;
			case 2:
				printf("Unexpected character: '%c'\n", termChar);
				break;
			default:
				break;
		}
		printf("Try again:\n");
	}

	return result;
}


long GetLong(char *prompt)
{
	long	result;
	char	termChar;
	char	buffer[256];

	while (1)
	{
		GetLine(prompt, buffer, 256);
		switch (sscanf(buffer, "%ld%c", &result, &termChar))
		{
			case 1:
				return result;
			case 2:
				printf("Unexpected character: '%c'\n", termChar);
				break;
			default:
				break;
		}
		printf("Try again:\n");
	}
}

int GetLine(char *prompt, char *buffer, int length)
{
	int n, ch;
	int i;


	printf("%s", prompt);
	n = 0;
	if (buffer)
	{
		while (n + 1 < length)
		{
			ch = getc(stdin);
			if ((ch != '\n') && (ch != EOF))
			{
				buffer[n] = ch;
				n++;
			}
			else
			{
				break;
			}
		}
		buffer[n] = '\0';

		// look for '//' comment mark
		for (i=0; i<n; i++)
		{
			if ((buffer[i] == '/') && (buffer[i+1] == '/'))
			{
				buffer[i] = '\0';
				break;
			}
		}

		// whole line is a comment, or didn't type anything, so get next one.
		if (i==0)
		{
			return GetLine(prompt, buffer, length);
		}

		// trim white space off end.
		while (i>0)
		{
			i--;
			if ((buffer[i] == ' ') || (buffer[i] == '\t'))
			{
				buffer[i] = '\0';
			}
			else
			{
				break;
			}
		}
	}
	LogText(buffer);
	return n;
}



char *BAEResultToStr(BAEResult r)
{
	switch (r)
	{
		case BAE_NO_ERROR:				return "BAE_NO_ERROR";
		case BAE_PARAM_ERR:				return "BAE_PARAM_ERR";
		case BAE_MEMORY_ERR:			return "BAE_MEMORY_ERR";
		case BAE_BAD_INSTRUMENT:		return "BAE_BAD_INSTRUMENT";
		case BAE_BAD_MIDI_DATA:			return "BAE_BAD_MIDI_DATA";
		case BAE_ALREADY_PAUSED:		return "BAE_ALREADY_PAUSED";
		case BAE_ALREADY_RESUMED:		return "BAE_ALREADY_RESUMED";
		case BAE_DEVICE_UNAVAILABLE:	return "BAE_DEVICE_UNAVAILABLE";
		case BAE_NO_SONG_PLAYING:		return "BAE_NO_SONG_PLAYING";
		case BAE_STILL_PLAYING:			return "BAE_STILL_PLAYING";
		case BAE_TOO_MANY_SONGS_PLAYING:	return "BAE_TOO_MANY_SONGS_PLAYING";
		case BAE_NO_VOLUME:				return "BAE_NO_VOLUME";
		case BAE_GENERAL_ERR:			return "BAE_GENERAL_ERR";
		case BAE_NOT_SETUP:				return "BAE_NOT_SETUP";
		case BAE_NO_FREE_VOICES:		return "BAE_NO_FREE_VOICES";
		case BAE_STREAM_STOP_PLAY:		return "BAE_STREAM_STOP_PLAY";
		case BAE_BAD_FILE_TYPE:			return "BAE_BAD_FILE_TYPE";
		case BAE_GENERAL_BAD:			return "BAE_GENERAL_BAD";
		case BAE_BAD_FILE:				return "BAE_BAD_FILE";
		case BAE_NOT_REENTERANT:		return "BAE_NOT_REENTERANT";
		case BAE_BAD_SAMPLE:			return "BAE_BAD_SAMPLE";
		case BAE_BUFFER_TOO_SMALL:		return "BAE_BUFFER_TOO_SMALL";
		case BAE_BAD_BANK:				return "BAE_BAD_BANK";
		case BAE_BAD_SAMPLE_RATE:		return "BAE_BAD_SAMPLE_RATE";
		case BAE_TOO_MANY_SAMPLES:		return "BAE_TOO_MANY_SAMPLES";
		case BAE_UNSUPPORTED_FORMAT:	return "BAE_UNSUPPORTED_FORMAT";
		case BAE_FILE_IO_ERROR:			return "BAE_FILE_IO_ERROR";
		case BAE_SAMPLE_TOO_LARGE:		return "BAE_SAMPLE_TOO_LARGE";
		case BAE_UNSUPPORTED_HARDWARE:	return "BAE_UNSUPPORTED_HARDWARE";
		case BAE_ABORTED:				return "BAE_ABORTED";
		case BAE_FILE_NOT_FOUND:		return "BAE_FILE_NOT_FOUND";
		case BAE_RESOURCE_NOT_FOUND:	return "BAE_RESOURCE_NOT_FOUND";
		case BAE_NULL_OBJECT:			return "BAE_NULL_OBJECT";
		case BAE_ERROR_COUNT:			return "BAE_ERROR_COUNT";
		case BAE_ALREADY_EXISTS:		return "BAE_ALREADY_EXISTS";
	}
	return "#Unknown Error Code#";
}




// ---------------------------------------------------------------------
// BAEBoolToYesNo()
// ---------------------------------------------------------------------
const char *BAEBoolToYesNo(BAE_BOOL b)
{
	return (b ? "Yes" : "No");
}


// ---------------------------------------------------------------------
// BAEBoolToTrueFalse()
// ---------------------------------------------------------------------
const char *BAEBoolToTrueFalse(BAE_BOOL b)
{
	return (b ? "True" : "False");
}


// ---------------------------------------------------------------------
// BAECPUTypeToStr()
// ---------------------------------------------------------------------
char *BAECPUTypeToStr(BAECPUType t)
{
	switch (t)
	{
		case BAE_CPU_UNKNOWN:			return "BAE_CPU_UNKNOWN";
		case BAE_CPU_POWERPC:			return "BAE_CPU_POWERPC";
		case BAE_CPU_SPARC:				return "BAE_CPU_SPARC";
		case BAE_CPU_JAVA:				return "BAE_CPU_JAVA";
		case BAE_CPU_MIPS:				return "BAE_CPU_MIPS";
		case BAE_CPU_INTEL_PENTIUM:		return "BAE_CPU_INTEL_PENTIUM";
		case BAE_CPU_INTEL_PENTIUM3:	return "BAE_CPU_INTEL_PENTIUM3";
		case BAE_CPU_CRAY_XMP3:			return "BAE_CPU_CRAY_XMP3";
	}
	return "#Unknown CPU Type#";
}


// ---------------------------------------------------------------------
// BAETerpModeToStr()
// ---------------------------------------------------------------------
char *BAETerpModeToStr(BAETerpMode m)
{
	switch (m)
	{
		case BAE_DROP_SAMPLE:			return "BAE_DROP_SAMPLE";
		case BAE_2_POINT_INTERPOLATION:	return "BAE_2_POINT_INTERPOLATION";
		case BAE_LINEAR_INTERPOLATION:	return "BAE_LINEAR_INTERPOLATION";
	}
	return "#Unknown Terp Mode#";
}


// ---------------------------------------------------------------------
// BAEQualityToStr()
// ---------------------------------------------------------------------
char *BAEQualityToStr(BAEQuality q)
{
	switch (q)
	{ 
		case BAE_8K:					return "BAE_8K";
		case BAE_11K:					return "BAE_11K";
		case BAE_11K_TERP_22K:			return "BAE_11K_TERP_22K";
		case BAE_22K:					return "BAE_22K";
		case BAE_22K_TERP_44K:			return "BAE_22K_TERP_44K";
		case BAE_24K:					return "BAE_24K";
		case BAE_32K:					return "BAE_32K";
		case BAE_40K:					return "BAE_40K";
		case BAE_44K:					return "BAE_44K";
		case BAE_48K:					return "BAE_48K";
	}
	return "#Unknown Quality#";
}

/*
// ---------------------------------------------------------------------
// BAEAudioModifiersToStr()
// ---------------------------------------------------------------------
char *BAEAudioModifiersToStr(BAEAudioModifiers m)
{
	string result("");
	
	if (m == BAE_NONE) 			result.append("BAE_NONE, ");
	if (m & BAE_USE_16) 		result.append("BAE_USE_16, ");
	if (m & BAE_USE_STEREO) 	result.append("BAE_USE_STEREO, ");
	if (m & BAE_DISABLE_REVERB) result.append("BAE_DISABLE_REVERB, ");
	if (m & BAE_STEREO_FILTER) 	result.append("BAE_STEREO_FILTER, ");

	return result.c_str();
}
*/

