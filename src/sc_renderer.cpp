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
		//Render 3D, perspective elements
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Render each Model
		State* cs = game.currentState;

		for (auto drawIt = cs->drawModelPool.begin(); drawIt != cs->drawModelPool.end(); drawIt++)
		{
			int textureCount = 0;

			if (drawIt->isVisible)
			{
				Model* drawModel = drawIt->model;

				glUseProgram(drawModel->material->shader->GLid);

				//Bind all int values to the shader
				for (size_t i = 0; i < drawModel->material->integerMaterialArguments.size(); i++)
				{
					glUniform1i(glGetUniformLocation(drawModel->material->shader->GLid, (const GLchar*)("int_" + iToS(i)).c_str()), drawModel->material->integerMaterialArguments[i]);
				}
				//Bind all float values to the shader
				for (size_t i = 0; i < drawModel->material->floatMaterialArguments.size(); i++)
				{
					glUniform1f(glGetUniformLocation(drawModel->material->shader->GLid, (const GLchar*)("float_" + iToS(i)).c_str()), drawModel->material->floatMaterialArguments[i]);
				}

				//Bind all vec4 values to the shader
				for (size_t i = 0; i < drawModel->material->vec4MaterialArguments.size(); i++)
				{
					glUniform4f(glGetUniformLocation(drawModel->material->shader->GLid, (const GLchar*)("vec4_" + iToS(i)).c_str()), 
						drawModel->material->vec4MaterialArguments[i][0],
						drawModel->material->vec4MaterialArguments[i][1],
						drawModel->material->vec4MaterialArguments[i][2],
						drawModel->material->vec4MaterialArguments[i][3]);
				}

				//Bind all textures to the shader
				for (size_t i = 0; i < drawModel->material->textureMaterialArguments.size(); i++)
				{
					glActiveTexture(GL_TEXTURE0 + textureCount);

					if (drawModel->material->textureMaterialArguments[i]->array)
					{
						glBindTexture(GL_TEXTURE_2D_ARRAY, drawModel->material->textureMaterialArguments[i]->GLid);
						glUniform1i(glGetUniformLocation(drawModel->material->shader->GLid, (const GLchar*)("textureArray_" + iToS(i)).c_str()), textureCount);
					}
					else
					{
						glBindTexture(GL_TEXTURE_2D, drawModel->material->textureMaterialArguments[i]->GLid);
						glUniform1i(glGetUniformLocation(drawModel->material->shader->GLid, (const GLchar*)("texture_" + iToS(i)).c_str()), textureCount);
					}

					textureCount++;
				}

				//Bind transform to shader
				Camera* cam = cs->cameraPool.get(renderCameraEntityId);
				glm::mat4 pvw = cam->getProjectionMatrix() * cam->getViewMatrix() * cs->transformPool.get(drawIt->entityId)->getWorldMatrix();
				glUniformMatrix4fv(glGetUniformLocation(drawModel->material->shader->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));

				glBindVertexArray(drawModel->mesh->VAOid);
					glDrawElements(GL_TRIANGLES, drawModel->mesh->indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}

		//Render 2D, orthographic elements
		glClear(GL_DEPTH_BUFFER_BIT);

		//Render rectangles
		for (auto drawIt = cs->drawRectanglePool.begin(); drawIt != cs->drawRectanglePool.end(); drawIt++)
		{
			if (drawIt->isVisible)
			{
				Mesh* mesh = assets.getMesh(ID("ME_QUAD"));
				Shader* shad = assets.getShader(ID("SH_COLOR"));
				glUseProgram(shad->GLid);

				glUniform4f(glGetUniformLocation(shad->GLid, (const GLchar*)"flatColor"), 
					drawIt->color[0],
					drawIt->color[1],
					drawIt->color[2],
					drawIt->color[3]);

				//Bind transform to shader
				glm::mat4 pvw = cs->cameraPool.get(renderCameraEntityId)->getOrthoMatrix() * cs->transformPool.get(drawIt->entityId)->getWorldMatrix();
				glUniformMatrix4fv(glGetUniformLocation(shad->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));				

				glBindVertexArray(mesh->VAOid);
					glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}

		//Render Sprites
		for (auto drawIt = cs->drawSpritePool.begin(); drawIt != cs->drawSpritePool.end(); drawIt++)
		{
			if (drawIt->isVisible)
			{
				Mesh* mesh = assets.getMesh(ID("ME_QUAD"));
				Shader* shad = assets.getShader(ID("SH_SPRITE"));
				glUseProgram(shad->GLid);

				glUniform1f(glGetUniformLocation(shad->GLid, (const GLchar*)"texCoordScaleX"), drawIt->sprite->texCoordX);
				glUniform1f(glGetUniformLocation(shad->GLid, (const GLchar*)"texCoordScaleY"), drawIt->sprite->texCoordY);
				
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, drawIt->sprite->GLid);
				glUniform1i(glGetUniformLocation(shad->GLid, (const GLchar*)"sprite"), 0);

				//Bind transform to shader
				glm::mat4 pvw = cs->cameraPool.get(renderCameraEntityId)->getOrthoMatrix() * cs->transformPool.get(drawIt->entityId)->getWorldMatrix();
				glUniformMatrix4fv(glGetUniformLocation(shad->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));				

				glBindVertexArray(mesh->VAOid);
					glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}

		//Render Text
		for (auto drawIt = cs->drawTextPool.begin(); drawIt != cs->drawTextPool.end(); drawIt++)
		{
			if (drawIt->isVisible)
			{
				Shader* shad = assets.getShader(ID("SH_FONT"));
				glUseProgram(shad->GLid);

				glUniform4f(glGetUniformLocation(shad->GLid, (const GLchar*)"fontColor"), 
					drawIt->color[0],
					drawIt->color[1],
					drawIt->color[2],
					drawIt->color[3]);
				
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, drawIt->font->textureGLid);
				glUniform1i(glGetUniformLocation(shad->GLid, (const GLchar*)"fontTexture"), 0);

				glm::mat4 pvw = cs->cameraPool.get(renderCameraEntityId)->getOrthoMatrix();
				glUniformMatrix4fv(glGetUniformLocation(shad->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));

				GLfloat x = 0.0f;

				switch (drawIt->alignment)
				{
				case TextAlign::right:
					x = drawIt->x;
					break;
				case TextAlign::center:
					x = drawIt->x - (drawIt->getWidth() / 2.0f);
					break;
				case TextAlign::left:
					x = drawIt->x - drawIt->getWidth();
					break;
				}

				GLfloat y = drawIt->y;

				std::string::const_iterator cIt;
				std::string drawText = drawIt->getText();

				glBindVertexArray(drawIt->font->VAOid);


				for (cIt = drawText.begin(); cIt != drawText.end(); cIt++)
				{
					FontCharacter* fontChar = &(drawIt->font->characters[*cIt]);

					GLfloat xpos = x + fontChar->bearing.x;
					GLfloat ypos = y - (fontChar->size.y - fontChar->bearing.y);

					GLfloat w = fontChar->size.x;
					GLfloat h = fontChar->size.y;

					GLfloat vertices[6][4] = {
						{ xpos,     ypos + h,   fontChar->textureCoords.x, fontChar->textureCoords.z },
						{ xpos,     ypos,       fontChar->textureCoords.x, fontChar->textureCoords.w },
						{ xpos + w, ypos,       fontChar->textureCoords.y, fontChar->textureCoords.w },

						{ xpos,     ypos + h,   fontChar->textureCoords.x, fontChar->textureCoords.z },
						{ xpos + w, ypos,       fontChar->textureCoords.y, fontChar->textureCoords.w },
						{ xpos + w, ypos + h,   fontChar->textureCoords.y, fontChar->textureCoords.z }
					};

					glBindBuffer(GL_ARRAY_BUFFER, drawIt->font->VBOid);
						glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
					glBindBuffer(GL_ARRAY_BUFFER, 0);

					glDrawArrays(GL_TRIANGLES, 0, 6);

					x += (fontChar->advance >> 6);
				}

				glBindVertexArray(0);
			}
		}

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