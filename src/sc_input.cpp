/*

    SHUFFLE COMPANY SOURCE CODE

    sc_input.cpp
    ------------------------------------



*/

#include "sc_input.h"

namespace sc
{
	Input input;

	Input::Input()
	{
		mouseXDelta = 0;
		mouseYDelta = 0;		
		mouseXLocal = 0;
		mouseYLocal = 0;
		firstMouseInput = true;
	}

	bool Input::eventExists(std::vector<SDL_EventType>* eventInput, SDL_EventType event)
	{
		if (std::find(eventInput->begin(), eventInput->end(), event) != eventInput->end())
		{
			return true;
		}
		
		return false;
	}

	bool Input::keyInputExists(std::vector<SDL_Keycode>* keyInput, SDL_Keycode key)
	{
		if (std::find(keyInput->begin(), keyInput->end(), key) != keyInput->end())
		{
			return true;
		}

		return false;
	}

	bool Input::mouseInputExists(std::vector<Uint8>* mouseInput, Uint8 button)
	{
		if (std::find(mouseInput->begin(), mouseInput->end(), button) != mouseInput->end())
		{
			return true;
		}

		return false;
	}


	void Input::update()
	{
		events.clear();
		keysPressed.clear();
		keysReleased.clear();

		SDL_Event event;
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				events.push_back(SDL_QUIT);
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (!keyInputExists(&keysHeld, event.key.keysym.sym))
				{
					keysPressed.push_back(event.key.keysym.sym);
					keysHeld.push_back(event.key.keysym.sym);
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				keysReleased.push_back(event.key.keysym.sym);
				keysHeld.erase(std::remove(keysHeld.begin(), keysHeld.end(), event.key.keysym.sym), keysHeld.end());
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (!mouseInputExists(&mouseHeld, event.button.button))
				{
					mousePressed.push_back(event.button.button);
					mouseHeld.push_back(event.button.button);
				}
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				mouseReleased.push_back(event.button.button);
				mouseHeld.erase(std::remove(mouseHeld.begin(), mouseHeld.end(), event.button.button), mouseHeld.end());
			}
		}

		if (!firstMouseInput)
		{
			SDL_GetRelativeMouseState(&mouseXDelta, &mouseYDelta);
			SDL_GetMouseState(&mouseXLocal, &mouseYLocal);
		}
		else
		{
			SDL_GetRelativeMouseState(NULL, NULL);
			SDL_GetMouseState(NULL, NULL);
			firstMouseInput = false;
		}
	}


	bool Input::keyPressed(SDL_Keycode key)
	{
		return keyInputExists(&keysPressed, key);
	}

	bool Input::keyHeld(SDL_Keycode key)
	{
		return keyInputExists(&keysHeld, key);
	}

	bool Input::keyReleased(SDL_Keycode key)
	{
		return keyInputExists(&keysReleased, key);
	}


	bool Input::mouseButtonPressed(Uint8 button)
	{
		return mouseInputExists(&mousePressed, button);
	}

	bool Input::mouseButtonHeld(Uint8 button)
	{
		return mouseInputExists(&mouseHeld, button);
	}

	bool Input::mouseButtonReleased(Uint8 button)
	{
		return mouseInputExists(&mouseReleased, button);
	}

	int Input::getMouseXDelta()
	{
		LOG_D << "MXD: " << mouseXDelta;
		return mouseXDelta;
	}

	int Input::getMouseYDelta()
	{
		LOG_D << "MYD: " << mouseYDelta;
		return mouseYDelta;
	}

	int Input::getMouseX()
	{
		return mouseXLocal;
	}

	int Input::getMouseY()
	{
		return mouseYLocal;
	}


	bool Input::quitEvent()
	{
		return eventExists(&events, SDL_QUIT);
	}
}