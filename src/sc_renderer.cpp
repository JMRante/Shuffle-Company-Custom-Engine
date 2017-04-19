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

		//Render each Model
		for (auto drawIt = state->drawModelPool.begin(); drawIt != state->drawModelPool.end(); drawIt++)
		{
			drawIt->render(renderCameraEntityId);
		}

		//Render 2D, orthographic elements
		glClear(GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_ALWAYS);

		for (auto drawIt = state->orthoPointers.begin(); drawIt != state->orthoPointers.end(); drawIt++) 
		{
			//This happens because in this one case virtual functions don't work. Gave up on finding the
			//reason why.
			LOG_D << (*drawIt)->entityId.get(); LOG_FLUSH;
			(*drawIt)->render(renderCameraEntityId);

			/*
			if ((*drawIt)->isType(ID("DRAWRECTANGLE")))
			{
				DrawRectangle* dr = static_cast<DrawRectangle*>(*drawIt);
				dr->render(renderCameraEntityId);
			}
			else if ((*drawIt)->isType(ID("DRAWSPRITE")))
			{
				DrawSprite* ds = static_cast<DrawSprite*>(*drawIt);
				ds->render(renderCameraEntityId);
			}
			else if ((*drawIt)->isType(ID("DRAWTEXT")))
			{
				DrawText* dt = static_cast<DrawText*>(*drawIt);
				dt->render(renderCameraEntityId);
			}*/
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