/*

    SHUFFLE COMPANY SOURCE CODE

    sc_game.h
    ------------------------------------



*/

#ifndef SC_GAME
#define SC_GAME

#include <vector>

#include <SDL.h>

#include "sc_log.h"
#include "sc_world.h"
// #include "sc_ui.h"
#include "sc_assets.h"

namespace sc
{
	class Game
	{
	private:
		sc::World *nextState;

	public:
		sc::World *currentState;
		// sc::UI *ui; 

		void start();
		bool update();
	};
}

#endif