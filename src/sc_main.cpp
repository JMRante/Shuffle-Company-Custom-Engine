/*

	SHUFFLE COMPANY SOURCE CODE

	sc_main.cpp
	------------------------------------

	This is where it all kicks off! The "main" function resides here. It initiates the game engine
	and starts off the main game loop.

*/

//Outside Headers
#include "sc_main.h"

#include <cmath>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#include <IL/il.h> 
#include <IL/ilu.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"

//Shuffle Company Headers
#include "sc_config.h"
#include "sc_log.h"
#include "sc_game.h"
#include "sc_renderer.h"

//Function Foward Declarations
bool initiate();
void closeout();

//Global Variable Declarations
SDL_Window *window;
SDL_GLContext glContext;

bool hasQuit = false;

Uint32 startTime = 0;
Sint32 delay = 0;

bool initiate()
{
	sc::openLog("shuffleLog.txt");

	//Initiate config
	if (!sc::config.loadConfig("shuffle.config"))
	{
		LOG_E << "Failed to load config file";
		return false;
	}

	LOG_I << "Shuffle Company initializing";

	//Initiate SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		LOG_E << SDL_GetError();
		return false;
	}

	//Initiate Window
	window = SDL_CreateWindow("Shuffle Company", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sc::config.get("WINDOW_WIDTH"), sc::config.get("WINDOW_HEIGHT"), SDL_WINDOW_OPENGL);

	if (window == NULL)
	{
		LOG_E << SDL_GetError();
		return false;
	}

	//Initiate Fullscreen
	if (SDL_SetWindowFullscreen(window, sc::config.get("FULLSCREEN") ? SDL_WINDOW_FULLSCREEN : 0))
	{
		LOG_E << SDL_GetError();
		return false;
	}

	//Initiate SDL OpenGL version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Initiate SDL OpenGL context
	glContext = SDL_GL_CreateContext(window);

	if (glContext == NULL)
	{
		LOG_E << SDL_GetError();
		return false;
	}

	//Initiate GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();

	if (glewError != GLEW_OK)
	{
		LOG_E << glewGetErrorString(glewError);
		return false;
	}

	//Initiate OpenGL options
    glViewport(0, 0, sc::config.get("WINDOW_WIDTH"), sc::config.get("WINDOW_HEIGHT"));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND); 
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Initiate SDL options
	SDL_GL_SetSwapInterval(1);
	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_ShowCursor(SDL_DISABLE);

	//Initiate DevIL
	ilInit();
	iluInit();
	ilClearColour(0, 0, 0, 0);

	ILenum ilError = ilGetError();
	if (ilError != IL_NO_ERROR)
	{
		LOG_E << "Error initiating DevIL: " << iluErrorString(ilError);
		return false;
	}

	sc::assets.loadBaseAssets();

	return true;
}

void closeout()
{
	sc::assets.clearWorldAssets();
	sc::assets.clearBaseAssets();
	sc::closeLog();
	SDL_Quit();	
}

int main(int argc, char **argv)
{
	if (initiate())
	{
		//Main Game Loop
		LOG_I << "Initiating Game Loop";
		LOG_D << "Debug mode set to " << sc::config.get("LOG_DEBUG");
		LOG_D << "Info mode set to " << sc::config.get("LOG_INFO");

		sc::Game game;
		sc::Renderer renderer(window);
		

		game.start();
		renderer.setCameraEntity(sc::ID("E_CAMERA"));

		while (!hasQuit)
		{
			startTime = SDL_GetTicks();

			hasQuit = game.update();
			renderer.render(game.state);

			delay = startTime + MS_PER_FRAME - SDL_GetTicks();
			if (delay >= 0)
			{
				SDL_Delay(delay);
			}
		}
	}
	else
	{
		LOG_E << "Initiation Error: Failed to start";
	}

	closeout();

	return 0;
}