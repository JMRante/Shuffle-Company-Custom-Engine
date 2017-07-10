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
#include <vector>

#include "sc_log.h"
#include "sc_main.h"

#include <boost/filesystem.hpp>

#define ID_SIZE 32

namespace fs = boost::filesystem;

namespace sc
{
	char* fileRead(const char* filename);

	std::string iToS(int i);
	std::string fToS(float f);

	GLuint powerOfTwo(GLuint num);

	float getDeltaSec();

	float lerp(float start, float end, float t);

	std::vector<std::string> getFiles(std::string directory);
}

#endif