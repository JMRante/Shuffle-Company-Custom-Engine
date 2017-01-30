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
// #include "sc_ui.h"
#include "sc_assets.h"

namespace sc
{
	class Renderer
	{
	private:
		SDL_Window *window;
		sc::World *renderWorld;
		// sc::UI *renderUI;
		sc::Assets *renderAssets;
		
	public:
		Renderer(SDL_Window *window);
		void render();
		void renderGameElement(GameElement *gameElement);
		void setWorld(sc::World *world);
		// void setUI(sc::UI *ui);
		void setClearColor(float r, float g, float b);
	};
}

#endif