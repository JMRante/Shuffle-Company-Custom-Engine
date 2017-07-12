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
		clearColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}

    //Render process
	//foreach view
	//foreach buffer
    //foreach shader
    //foreach model
    //foreach material
    //Call Renderer

	void Renderer::render(State* state)
	{
        //Views
        //Render main, 3D view
        glViewport(0, 0, sc::config.get("WINDOW_WIDTH"), sc::config.get("WINDOW_HEIGHT"));
		glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);

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
}