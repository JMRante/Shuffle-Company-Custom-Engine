/*

    SHUFFLE COMPANY SOURCE CODE

    sc_input.h
    ------------------------------------



*/

#ifndef SC_INPUT
#define SC_INPUT

#include <vector>
#include <algorithm>
#include <string>

#include <SDL.h>

#include "sc_log.h"
#include "sc_config.h"
#include "sc_utility.h"

#define TEXTINPUT_COOLDOWN 5

namespace sc
{
	class Input
	{
	private:
		std::vector<SDL_EventType> events;

		std::vector<SDL_Keycode> keysPressed;
		std::vector<SDL_Keycode> keysHeld;
		std::vector<SDL_Keycode> keysReleased;

		std::vector<Uint8> mousePressed;
		std::vector<Uint8> mouseHeld;
		std::vector<Uint8> mouseReleased;
		int mouseXDelta;
		int mouseYDelta;
		int mouseXLocal;
		int mouseYLocal;
		int mouseWheelDelta;
		bool firstMouseInput;

		std::string textInputBuffer;
		size_t textInputBufferCap;
		size_t textInsertPosition;
		int backspaceCooldown;
		int textCursorCooldown;

		bool eventExists(std::vector<SDL_EventType>* eventInput, SDL_EventType event);
		bool keyInputExists(std::vector<SDL_Keycode>* keyInput, SDL_Keycode key);
		bool mouseInputExists(std::vector<Uint8>* mouseInput, Uint8 button);

	public:
		ID mouseSelectedEntity;

		Input();
		void update();

		bool keyPressed(SDL_Keycode key);
		bool keyHeld(SDL_Keycode key);
		bool keyReleased(SDL_Keycode key);

		bool mouseButtonPressed(Uint8 button);
		bool mouseButtonHeld(Uint8 button);
		bool mouseButtonReleased(Uint8 button);
		int getMouseXDelta();
		int getMouseYDelta();
		int getMouseX();
		int getMouseY();
		int getMouseWheelDelta();

		void startTextInput();
		void stopTextInput();
		void initTextInputBuffer(std::string startString, size_t cap);
		bool gettingTextInput();
		std::string getTextInputBuffer();
		size_t getTextInsertPosition();
		void decTextInsertPosition();
		void incTextInsertPosition();
		
		bool quitEvent();
	};

	extern Input input;
}

#endif