/*

    SHUFFLE COMPANY SOURCE CODE

    sc_renderer.h
    ------------------------------------



*/

#ifndef SC_RENDERER
#define SC_RENDERER

#include <SDL.h>

#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#include "sc_log.h"

namespace sc
{
	class Renderer
	{
	private:
		SDL_Window *window;
	public:
		Renderer(SDL_Window *window);
		void render();
		void setClearColor(float r, float g, float b);
	};
}

#endif