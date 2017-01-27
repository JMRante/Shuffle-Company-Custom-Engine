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
	private:
		std::vector<GameElement> elements;
	public:
		void addGameElement(GameElement element);
	};
}

#endif