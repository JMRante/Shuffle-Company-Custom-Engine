/*

    SHUFFLE COMPANY SOURCE CODE

    sc_game.cpp
    ------------------------------------



*/

#include "sc_game.h"

namespace sc
{
	bool Game::update()
	{
		//Handle events
		SDL_Event event;

		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				return true; //Do not continue to update (close game)
			}
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_UP:
						LOG_D << SDLK_UP;
						break;

					case SDLK_DOWN:
						LOG_D << SDLK_DOWN;
						break;

					case SDLK_LEFT:
						LOG_D << SDLK_LEFT;
						break;

					case SDLK_RIGHT:
						LOG_D << SDLK_RIGHT;
						break;

					case SDLK_RETURN:
						LOG_D << SDLK_RETURN;
						break;

					case SDLK_ESCAPE:
						LOG_D << SDLK_ESCAPE;
						break;

					case SDLK_BACKQUOTE:
						LOG_D << SDLK_BACKQUOTE;
						break;

					default:
						break;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				switch (event.button.button)
				{
					case SDL_BUTTON_LEFT:
						LOG_D << SDL_BUTTON_LEFT << " Down";
						break;
					case SDL_BUTTON_MIDDLE:
						LOG_D << SDL_BUTTON_MIDDLE << " Down";
						break;
					case SDL_BUTTON_RIGHT:
						LOG_D << SDL_BUTTON_RIGHT << " Down";
						break;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				switch (event.button.button)
				{
					case SDL_BUTTON_LEFT:
						LOG_D << SDL_BUTTON_LEFT << " Up";
						break;
					case SDL_BUTTON_MIDDLE:
						LOG_D << SDL_BUTTON_MIDDLE << " Up";
						break;
					case SDL_BUTTON_RIGHT:
						LOG_D << SDL_BUTTON_RIGHT << " Up";
						break;
				}				
			}
		}

		return false; //Continue to update (game not yet closed)
	}
}