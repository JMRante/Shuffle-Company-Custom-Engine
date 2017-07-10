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

        //Render mouse select view
        //input.mouseSelectedEntity = renderForMouseSelect(state);
	}

	ID Renderer::renderForMouseSelect(State* state)
	{
        glViewport(0, 0, 1, 1);
		Stage* stage = state->getComponent<Stage>(CTID("E_STAGE"));
		unsigned int stageSelectCount = 0;

		if (stage != NULL)
		{
			stageSelectCount = stage->getMouseSelectCount();
		}

		unsigned int indexCount = 1 + stageSelectCount;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Render 3D, perspective elements
		//Render stage with selection spots
		if (stage != NULL)
		{
			state->getComponent<DrawModel>(CTID("E_STAGE"))->stageMouseRender(renderCameraEntityId);
		}

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

		//SDL_GL_SwapWindow(window);

		unsigned char resultColor[4];
		GLint viewport[4];

		glGetIntegerv(GL_VIEWPORT, viewport);
		glReadPixels(input.getMouseX(), input.getMouseY(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &resultColor);

		unsigned int indexGet = ((unsigned int)resultColor[0] << 16) | ((unsigned int)resultColor[1] << 8) | ((unsigned int)resultColor[2] << 0);

		if ((size_t)indexGet <= (state->mouseSelectModels.size() + state->mouseSelectOrthos.size() + stageSelectCount))
		{
			if ((size_t)indexGet > (state->mouseSelectModels.size() + stageSelectCount))
			{
				if (stage != NULL)
				{
					stage->setMouseSelected(-1);
				}

				return state->mouseSelectOrthos[(indexGet - state->mouseSelectModels.size() - stageSelectCount) - 1]->entityId;
			}
			else if (indexGet > stageSelectCount)
			{
				if (stage != NULL)
				{
					stage->setMouseSelected(-1);
				}

				return state->mouseSelectModels[(indexGet - stageSelectCount) - 1]->entityId;
			}
			else if (indexGet > 0)
			{
				if (stage != NULL)
				{
					stage->setMouseSelected(indexGet);
				}

				return CTID("STAGESELECTED");
			}			
		}

		if (stage != NULL)
		{
			stage->setMouseSelected(-1);
		}

		return CTID("NULL");
	}

	void Renderer::setCameraEntity(ID cameraEntityId)
	{
		this->renderCameraEntityId = cameraEntityId;
	}
}