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
#include "sc_world.h"
#include "sc_ui.h"

namespace sc
{
	class Renderer
	{
	private:
		SDL_Window *window;
		sc::World *renderWorld;
		sc::UI *renderUI;
		sc::Assets *renderAssets;
		
	public:
		Renderer(SDL_Window *window);
		void render();
		void setRenderWorld(sc::World *world);
		void setRenderUI(sc::UI *ui);
		void setRenderAssets(sc::Assets *assets);
		void setClearColor(float r, float g, float b);
	};
}

#endif