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

	void Renderer::render(State* state)
	{
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

	ID Renderer::renderForMouseSelect(State* state)
	{
		unsigned int indexCount = 1;

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Render 3D, perspective elements
		//Render each opaque model
		for (auto drawIt = state->mouseSelectModels.begin(); drawIt != state->mouseSelectModels.end(); drawIt++)
		{
			(*drawIt)->mouseRender(renderCameraEntityId, indexCount);
			indexCount++;
		}

		//Render 2D, orthographic elements
		glClear(GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_ALWAYS);
			for (auto drawIt = state->mouseSelectOrthos.begin(); drawIt != state->mouseSelectOrthos.end(); drawIt++) 
			{
				(*drawIt)->mouseRender(renderCameraEntityId, indexCount);
				indexCount++;
			}
		glDepthFunc(GL_LESS);

		unsigned char res[4];
		GLint viewport[4];

		glGetIntegerv(GL_VIEWPORT, viewport);
		glReadPixels(input.getMouseX(), input.getMouseY(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);

		unsigned int indexGet = ((unsigned int)res[0] << 16) | ((unsigned int)res[1] << 8) | ((unsigned int)res[2] << 0);

		if ((size_t)indexGet > state->mouseSelectModels.size() && (size_t)indexGet <= (state->mouseSelectModels.size() + state->mouseSelectOrthos.size()))
		{
			return state->mouseSelectOrthos[(indexGet - state->mouseSelectModels.size()) - 1]->entityId;
		}
		else if (indexGet > 0)
		{
			return state->mouseSelectModels[indexGet - 1]->entityId;
		}

		return ID("NULL");
	}

	void Renderer::setCameraEntity(ID cameraEntityId)
	{
		this->renderCameraEntityId = cameraEntityId;
	}
}