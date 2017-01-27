/*

    SHUFFLE COMPANY SOURCE CODE

    sc_utility.h
    ------------------------------------

    This file contains various utility functions and classes that have general purpose use
    throughout the Shuffle Company code base.

*/

#ifndef SC_UTILITY
#define SC_UTILITY

#include <SDL.h>
#include <cstdlib>

namespace sc
{
	typedef char ID[16];
	
	char* fileRead(const char* filename);
}

#endif