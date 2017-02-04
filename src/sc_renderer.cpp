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
		LOG_D << "Rendering " << gameElement->model->id;

		glUseProgram(gameElement->model->material->shader->GLid);

		//Bind all int values to the shader
		for (int i = 0; i < (int)gameElement->model->material->integerMaterialArguments.size(); i++)
		{
			glUniform1i(glGetUniformLocation(gameElement->model->material->shader->GLid, (const GLchar*)("int_" + sc::IntToString(i)).c_str()), gameElement->model->material->integerMaterialArguments[i]);
		}
		//Bind all float values to the shader
		for (int i = 0; i < (int)gameElement->model->material->floatMaterialArguments.size(); i++)
		{
			glUniform1f(glGetUniformLocation(gameElement->model->material->shader->GLid, (const GLchar*)("float_" + sc::IntToString(i)).c_str()), gameElement->model->material->floatMaterialArguments[i]);
		}

		//Bind all vec4 values to the shader
		for (int i = 0; i < (int)gameElement->model->material->vec4MaterialArguments.size(); i++)
		{
			glUniform4f(glGetUniformLocation(gameElement->model->material->shader->GLid, (const GLchar*)("vec4_" + sc::IntToString(i)).c_str()), 
				gameElement->model->material->vec4MaterialArguments[i][0],
				gameElement->model->material->vec4MaterialArguments[i][1],
				gameElement->model->material->vec4MaterialArguments[i][2],
				gameElement->model->material->vec4MaterialArguments[i][3]);
		}

		//Bind all textures to the shader
		for (int i = 0; i < (int)gameElement->model->material->textureMaterialArguments.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, gameElement->model->material->textureMaterialArguments[i]->GLid);
			glUniform1i(glGetUniformLocation(gameElement->model->material->shader->GLid, (const GLchar*)("texture_" + sc::IntToString(i)).c_str()), i);
		}

		//Bind transform to shader
		glUniformMatrix4fv(glGetUniformLocation(gameElement->model->material->shader->GLid, "transform"), 1, GL_FALSE, glm::value_ptr(gameElement->worldTransform.transform));
		glUniformMatrix4fv(glGetUniformLocation(gameElement->model->material->shader->GLid, "camera"), 1, GL_FALSE, glm::value_ptr(camera->viewTransform.transform));

		glBindVertexArray(gameElement->model->mesh->VAOid);
			glDrawElements(GL_TRIANGLES, gameElement->model->mesh->indexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Renderer::setWorld(sc::World *world)
	{
		this->renderWorld = world;
	}

	void Renderer::initCamera(float near, float far)
	{
		if (camera != NULL)
		{
			delete camera;
		}

		camera = new CameraElement(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), "", near, far);
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