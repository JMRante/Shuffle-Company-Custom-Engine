/*

    SHUFFLE COMPANY SOURCE CODE

    sc_world.h
    ------------------------------------



*/

#ifndef SC_WORLD
#define SC_WORLD

#include <vector>

#include "sc_log.h"
#include "sc_gameElement.h"

namespace sc
{
	class World
	{
	public:
		std::vector<GameElement> elements;
	};
}

#endif