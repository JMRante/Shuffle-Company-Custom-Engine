/*

    SHUFFLE COMPANY SOURCE CODE

    sc_utility.h
    ------------------------------------

    This file contains various utility functions and classes that have general purpose use
    throughout the Shuffle Company code base.

*/

#ifndef SC_UTILITY
#define SC_UTILITY

#include <GL/glew.h>
#include <SDL.h>

#include <cstdlib>
#include <sstream>
#include <string>

#include "sc_log.h"
#include "sc_main.h"

#define ID_SIZE 32

namespace sc
{
	char* fileRead(const char* filename);
	std::string iToS(int i);
	std::string fToS(float f);
	GLuint powerOfTwo(GLuint num);
	float getDeltaSec();
	float lerp(float start, float end, float t);

	class ID
	{
	private:
		char id[ID_SIZE];

	public:
		ID();
		ID(const char* id);
		ID(std::string str);
		bool is(const char* id) const;
		bool is(ID id) const;
		const char* get() const;
		char get(int i) const;
		std::string getStr();
	};

	bool operator<(const ID& l, const ID& r);
}

#endif