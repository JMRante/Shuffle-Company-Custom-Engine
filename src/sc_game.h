/*

    SHUFFLE COMPANY SOURCE CODE

    sc_game.h
    ------------------------------------



*/

#ifndef SC_GAME
#define SC_GAME

#include <vector>

#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext.hpp"

#include "sc_log.h"

#include "sc_assets.h"
#include "sc_entity.h"

namespace sc
{
	class World;

	class Game
	{
	public:
		World* currentState;
		World* nextState;

		void start();
		bool update();
		void updateWorldState();
	};

	extern Game game;
}

#endif