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
#include <sstream>
#include <string>

#define ID_SIZE 32

namespace sc
{
	char* fileRead(const char* filename);
	std::string IntToString(int i);

	class ID
	{
	private:
		char id[ID_SIZE];

	public:
		ID();
		ID(const char* id);
		bool is(const char* id);
		bool is(ID id);
		char* get();
	};
}

#endif