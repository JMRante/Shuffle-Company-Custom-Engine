#ifndef SC_CONFIG
#define SC_CONFIG

#include <SDL.h>
#include <string>
#include <map>
#include <algorithm>

#include "sc_utility.h"

class Config
{
private:
	std::map<std::string, int> members;
public:
	Config();
	bool loadConfig(std::string filename);
	int get(std::string member);
};

#endif