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
#include "sc_config.h"
#include "sc_utility.h"

namespace sc
{
	class State;

	class Game
	{
	public:
		State* state;

		void start();
		bool update();
	};
}

#endif