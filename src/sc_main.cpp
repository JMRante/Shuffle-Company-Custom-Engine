/*

	SHUFFLE COMPANY SOURCE CODE

	sc_main.cpp
	------------------------------------

	This is where it all kicks off! The "main" function resides here. It initiates the game engine
	and starts off the main game loop.

*/

#include <iostream>

#include <SDL2\SDL.h>

#include <GL\glew.h>
#include <SDL2\SDL_opengl.h>
#include <GL\glu.h>

//Function Foward Declarations
void closeout();

//Global Variable Declarations
SDL_Window *window = nullptr;
SDL_GLContext glContext = nullptr;

void closeout()
{
	SDL_DestroyWindow(window);
	window = nullptr;

	SDL_Quit();	
}

int main(int argc, char **argv)
{
	//Initiate SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;

		closeout();
		return 1;
	}

	//Initiate Window
	window = SDL_CreateWindow("Shuffle Company", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);

	if (window == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;

		closeout();
		return 1;
	}

	//Initiate SDL OpenGL version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Initiate SDL OpenGL context
	glContext = SDL_GL_CreateContext(window);

	if (glContext == nullptr)
	{
		std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;

		closeout();
		return 1;
	}

	//Initiate GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();

	if (glewError != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(glewError) << std::endl;

		closeout();
		return 1;
	}

	//Main Game Loop
	bool hasQuit = false;

	while (!hasQuit)
	{

	}

	closeout();
	return 0;
}