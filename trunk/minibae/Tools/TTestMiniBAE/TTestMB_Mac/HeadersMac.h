/*
 *	Headers.h
 *
 *	Processor independant interface to the MacHeaders<xxx> files ...
 */


#ifdef __cplusplus
#if __POWERPC__
	#include <MacHeadersPPC++>
#elif __CFM68K__
	#include <MacHeadersCFM68K++>
#else
	#include <MacHeaders68K++>
#endif
#else
#if __POWERPC__
	#include "HeadersPPC"
#elif __CFM68K__
	#include "HeadersCFM68K"
#else
	#include "Headers68K"
#endif
#endif



// Build options for HAE
#define X_PLATFORM			X_MACINTOSH
#define BAE_COMPLETE		1





