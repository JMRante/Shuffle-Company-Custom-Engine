/*

    SHUFFLE COMPANY SOURCE CODE

    sc_renderer.cpp
    ------------------------------------



*/

#include "sc_renderer.h"

namespace sc
{
	Renderer::Renderer(SDL_Window *window)
	{
		this->window = window;
	}

	void Renderer::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		LOG_D << "Render";

		SDL_GL_SwapWindow(window);
	}

	void Renderer::setClearColor(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);

		LOG_D << "Set clear color to " << r << " " << g << " " << b;
	}
}