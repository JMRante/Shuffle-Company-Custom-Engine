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

		//Render each world element
		for (int i = 0; i < (int)renderWorld->elements.size(); i++)
		{
			renderGameElement(&renderWorld->elements[i]);
		}

		SDL_GL_SwapWindow(window);
	}

	void Renderer::renderGameElement(GameElement *gameElement)
	{
		switch (gameElement->model->material->textureCount)
		{
			case 0:
				LOG_D << "Rendering " << gameElement->model->id;

				glUseProgram(gameElement->model->material->shader->GLid);
				glBindVertexArray(gameElement->model->mesh->VAOid);
					glDrawElements(GL_TRIANGLES, gameElement->model->mesh->indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			default:
				break;
		}
	}

	void Renderer::setWorld(sc::World *world)
	{
		this->renderWorld = world;
	}

	// void Renderer::setUI(sc::UI *ui)
	// {
	// 	this->ui = ui;
	// }

	void Renderer::setClearColor(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);

		LOG_D << "Set clear color to " << r << " " << g << " " << b;
	}
}