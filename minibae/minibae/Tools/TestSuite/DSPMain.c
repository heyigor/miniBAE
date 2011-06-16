/******************************************************************************
**
**	"DSPMain.c"
**
**	A simple shell for acting as the receptor for MCU calls
**
**	© Copyright 2000 Beatnik, Inc, All Rights Reserved.
**	Written by Andrew Ezekiel Rostaing
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
**	Modification History:
**
**	2000.02.24 AER	File Created
**
*****************************************************************************/


// ANSI includes
#include <stdio.h>

// Windows includes
#include <windows.h>

// Local includes
#include "BAE_API_Msg.h"


int main()
{
	//MSG					msg;

	printf("BAE Receiver Module (DSP/Engine)\n");
	printf("--------------------------------\n");

	if (BAE_SetupMsgReceiver() != BAE_MSG_NO_ERROR)
	{
		printf("Failure to setup Receiver (DSP polling mechanism");
		return 0;
	}

	printf("\nInterprocess Communication successfully established\n\n");

	printf("Hit control-c to quit\n");

	while (1)
	{
		// Do standard polling
		BAE_PollForMessages();
		// Sleep prevents us from consuming the whole CPU
		Sleep(1);
	}
	return 0;
}