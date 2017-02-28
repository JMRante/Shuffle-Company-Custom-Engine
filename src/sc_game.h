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
#include "sc_prefabs.h"
#include "sc_assets.h"

namespace sc
{
	class State;

	class Game
	{
	public:
		State* currentState;
		State* nextState;

		void start();
		bool update();
		void updateState();
	};

	extern Game game;
}

#endif