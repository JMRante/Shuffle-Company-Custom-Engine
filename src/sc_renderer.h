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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"

#include "sc_log.h"
#include "sc_utility.h"
#include "sc_game.h"
#include "sc_world.h"
#include "sc_entity.h"
#include "sc_component.h"
// #include "sc_ui.h"
#include "sc_assets.h"

namespace sc
{
	class Renderer
	{
	private:
		SDL_Window *window;
		sc::World *renderWorld;
		std::string renderCameraEntityId;
		sc::Assets *renderAssets;
		
	public:
		Renderer(SDL_Window *window);
		void render();
		void setWorld(sc::World *world);
		void setCameraEntity(std::string cameraEntityId);
		void setClearColor(glm::vec4 rgba);
	};
}

#endif