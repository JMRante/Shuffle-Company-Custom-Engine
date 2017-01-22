/*

	SHUFFLE COMPANY SOURCE CODE

	sc_config.h
	------------------------------------

	The Config class maps values that are defined in an external config file for use throughout the game engine.

*/


#ifndef SC_CONFIG
#define SC_CONFIG

#include <SDL.h>
#include <string>
#include <map>
#include <algorithm>

#include "sc_utility.h"

namespace sc
{
	class Config
	{
	private:
		std::map<std::string, int> members;
	public:
		Config();
		bool loadConfig(std::string filename);
		int get(std::string member);
	};

	extern Config config;
}

#endif