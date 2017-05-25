/*

    SHUFFLE COMPANY SOURCE CODE

    sc_renderer.cpp
    ------------------------------------



*/

#include "sc_renderer.h"

namespace sc
{
	Renderer::Renderer(SDL_Window* window)
	{
		this->window = window;
	}

	void Renderer::render(State* state)
	{
		//Render 3D, perspective elements
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Render each opaque model
		for (auto drawIt = state->modelPointers.begin(); drawIt != state->modelPointers.end(); drawIt++)
		{
			(*drawIt)->render(renderCameraEntityId);
		}

		//Render each transparent model
		for (auto drawIt = state->transparentModelPointers.begin(); drawIt != state->transparentModelPointers.end(); drawIt++)
		{
			(*drawIt)->render(renderCameraEntityId);
		}

		//Render 2D, orthographic elements
		glClear(GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_ALWAYS);
			for (auto drawIt = state->orthoPointers.begin(); drawIt != state->orthoPointers.end(); drawIt++) 
			{
				(*drawIt)->render(renderCameraEntityId);
			}
		glDepthFunc(GL_LESS);

		SDL_GL_SwapWindow(window);
	}

	void Renderer::setCameraEntity(ID cameraEntityId)
	{
		this->renderCameraEntityId = cameraEntityId;
	}

	void Renderer::setClearColor(glm::vec4 rgba)
	{
		glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);

		LOG_D << "Set clear color to " << rgba[0] << ", " << rgba[1] << ", " << rgba[2] << ", " << rgba[3];
	}
}