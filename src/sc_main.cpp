/*

	SHUFFLE COMPANY SOURCE CODE

	sc_main.cpp
	------------------------------------

	This is where it all kicks off! The "main" function resides here. It initiates the game engine
	and starts off the main game loop.

*/

//Outside Headers
#include <cmath>
#include <iostream>
#include <string>

#include <SDL.h>

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SOIL.h"

//Shuffle Company Headers
#include "sc_config.h"

//Defines
#define MS_PER_FRAME 16

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define FOV 45.0f

//Function Foward Declarations
bool initiate();
void closeout();

void update();
void render();

//Global Variable Declarations
SDL_Window *window;
SDL_GLContext glContext;
Config config;

bool hasQuit = false;

bool initiate()
{
	//Initiate config
	if (!config.loadConfig("shuffle.config"))
	{
		std::cout << "Failed to load config file: " << std::endl;
		return false;
	}

	//Initiate SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Initiate Window
	window = SDL_CreateWindow("Shuffle Company", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.get("WINDOW_WIDTH"), config.get("WINDOW_HEIGHT"), SDL_WINDOW_OPENGL);

	if (window == NULL)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Initiate Fullscreen
	if (SDL_SetWindowFullscreen(window, config.get("FULLSCREEN") ? SDL_WINDOW_FULLSCREEN : 0))
	{
		std::cout << "SDL_SetWindowFullscreen Error: " << SDL_GetError() << std::endl;
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
		std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Initiate GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();

	if (glewError != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(glewError) << std::endl;
		return false;
	}

	//Initiate OpenGL options
    glViewport(0, 0, config.get("WINDOW_WIDTH"), config.get("WINDOW_HEIGHT"));
	glEnable(GL_DEPTH_TEST); 

	return true;
}

void closeout()
{
	SDL_Quit();	
}

void update()
{
	//Handle events
	SDL_Event event;

	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_QUIT)
		{
			hasQuit = true;
		}
	}
}

void render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SDL_GL_SwapWindow(window);
}

int main(int argc, char **argv)
{
	if (initiate())
	{
		//Main Game Loop
		Uint32 startTime;
		Sint32 delay;

		while (!hasQuit)
		{
			startTime = SDL_GetTicks();

			update();
			render();

			delay = startTime + MS_PER_FRAME - SDL_GetTicks();
			if (delay >= 0)
			{
				SDL_Delay(delay);
			}
		}
	}
	else
	{
		std::cout << "Initiation Error: Failed to start" << std::endl;
	}

	closeout();

	return 0;
}