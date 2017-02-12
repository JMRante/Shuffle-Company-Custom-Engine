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
#include "sc_world.h"
#include "sc_assets.h"
#include "sc_entity.h"
#include "sc_behaviors.h"

namespace sc
{
	class Game
	{
	private:
		sc::World *nextState;

	public:
		sc::World *currentState;

		void start();
		bool update();
		sc::World *updateWorldState();
	};
}

#endif