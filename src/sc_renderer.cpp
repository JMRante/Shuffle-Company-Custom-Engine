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

	void Renderer::render()
	{
		//Render the world (Draw components)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Render each world element
		EntityManager* em = &(game.currentState->entityManager);

		for (auto drawIt = em->drawModelPool.begin(); drawIt != em->drawModelPool.end(); drawIt++)
		{
			if (drawIt->isVisible)
			{
				Model* drawModel = drawIt->model;

				glUseProgram(drawModel->material->shader->GLid);

				//Bind all int values to the shader
				for (size_t i = 0; i < drawModel->material->integerMaterialArguments.size(); i++)
				{
					glUniform1i(glGetUniformLocation(drawModel->material->shader->GLid, (const GLchar*)("int_" + sc::IntToString(i)).c_str()), drawModel->material->integerMaterialArguments[i]);
				}
				//Bind all float values to the shader
				for (size_t i = 0; i < drawModel->material->floatMaterialArguments.size(); i++)
				{
					glUniform1f(glGetUniformLocation(drawModel->material->shader->GLid, (const GLchar*)("float_" + sc::IntToString(i)).c_str()), drawModel->material->floatMaterialArguments[i]);
				}

				//Bind all vec4 values to the shader
				for (size_t i = 0; i < drawModel->material->vec4MaterialArguments.size(); i++)
				{
					glUniform4f(glGetUniformLocation(drawModel->material->shader->GLid, (const GLchar*)("vec4_" + sc::IntToString(i)).c_str()), 
						drawModel->material->vec4MaterialArguments[i][0],
						drawModel->material->vec4MaterialArguments[i][1],
						drawModel->material->vec4MaterialArguments[i][2],
						drawModel->material->vec4MaterialArguments[i][3]);
				}

				//Bind all textures to the shader
				for (size_t i = 0; i < drawModel->material->textureMaterialArguments.size(); i++)
				{
					glActiveTexture(GL_TEXTURE0 + i);

					if (drawModel->material->textureMaterialArguments[i]->array)
					{
						glBindTexture(GL_TEXTURE_2D_ARRAY, drawModel->material->textureMaterialArguments[i]->GLid);
						glUniform1i(glGetUniformLocation(drawModel->material->shader->GLid, (const GLchar*)("textureArray_" + sc::IntToString(i)).c_str()), i);
					}
					else
					{
						glBindTexture(GL_TEXTURE_2D, drawModel->material->textureMaterialArguments[i]->GLid);
						glUniform1i(glGetUniformLocation(drawModel->material->shader->GLid, (const GLchar*)("texture_" + sc::IntToString(i)).c_str()), i);
					}
				}

				//Bind transform to shader
				Camera* cam = em->cameraPool.get(renderCameraEntityId);
				glm::mat4 pvw = cam->getProjectionMatrix() * cam->getViewMatrix() * em->transformPool.get(drawIt->entityId)->getWorldMatrix();
				glUniformMatrix4fv(glGetUniformLocation(drawModel->material->shader->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));				

				glBindVertexArray(drawModel->mesh->VAOid);
					glDrawElements(GL_TRIANGLES, drawModel->mesh->indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}

		//Render the UI (UIDraw components)
		// glClear(GL_DEPTH_BUFFER_BIT);

		// for (auto UIDrawIt = em->getUIDrawRectanglePoolBegin(); UIDrawIt != em->getUIDrawRectanglePoolEnd(); UIDrawIt++)
		// {
		// 	if (UIIDrawIt->isVisible)
		// 	{
		// 		sc::Mesh* mesh = getMesh("ME_QUAD");
		// 		sc::Shader* shad = getShader("SH_COLOR");
		// 		glUseProgram(shad->GLid);

		// 		//Bind all vec4 values to the shader
		// 		glUniform4f(glGetUniformLocation(shad->GLid, (const GLchar*)("vec4_" + sc::IntToString(i)).c_str()), 
		// 			drawModel->material->vec4MaterialArguments[i][0],
		// 			drawModel->material->vec4MaterialArguments[i][1],
		// 			drawModel->material->vec4MaterialArguments[i][2],
		// 			drawModel->material->vec4MaterialArguments[i][3]);

		// 		//Bind transform to shader
		// 		glm::mat4 pvw = em->getCamera(renderCameraEntityId)->getOrthoMatrix() * em->getTransform(UIDrawIt->getEntityId())->getWorldMatrix();
		// 		glUniformMatrix4fv(glGetUniformLocation(shad->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));				

		// 		glBindVertexArray(mesh->VAOid);
		// 			glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
		// 		glBindVertexArray(0);
		// 	}
		// }

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