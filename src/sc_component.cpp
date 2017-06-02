/*

    SHUFFLE COMPANY SOURCE CODE

    sc_component.cpp
    ------------------------------------



*/

#include "sc_component.h"

#include <cmath>

#include "sc_game.h"
#include "sc_state.h"

namespace sc
{
	/*
		Component
					*/
	Component::Component() 
	{
		entityId = ID("NULL");
		addType(ID("NULL"));
		state = NULL;
	}

	bool Component::isType(ID id)
	{
		for (size_t i = 0; i < types.size(); i++)
		{
			if (types[i].is(id))
			{
				return true;
			}
		}

		return false;
	}

	bool Component::sameTypes(Component* comp)
	{
		if (types.size() != comp->types.size())
		{
			return false;
		}

		for (size_t i = 0; i < types.size(); i++)
		{
			bool found = false;

			for (size_t j = 0; j < comp->types.size(); j++)
			{
				if (types[i].is(comp->types[j]))
				{
					found = true;
				}
			}

			if (!found)
			{
				return false;
			}
		}

		return true;
	}

	void Component::addType(ID id)
	{
		types.push_back(id);
	}

	void Component::onStateInsert() {}

	void Component::onStateRemove() {}


	/*
		Transform
					*/
	Transform::Transform() : Component()
	{
		addType(ID("TRANSFORM"));

		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
		calculateWorldMatrix();
	}

	Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : Component()
	{
		addType(ID("TRANSFORM"));

		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
		calculateWorldMatrix();
	}

	glm::mat4 Transform::getWorldMatrix()
	{
		return worldMatrix;
	}

	void Transform::calculateWorldMatrix()
	{
		worldMatrix = glm::translate(glm::mat4(1.0f), position) * glm::eulerAngleYXZ(rotation[1], rotation[0], rotation[2]) * glm::scale(glm::mat4(1.0f), scale);
	}


	/*
		Camera
				*/
	Camera::Camera() : Component()
	{
		addType(ID("CAMERA"));

		fov = (float)config.get("FOV");
		aspectRatio = ((float)config.get("WINDOW_WIDTH"))/((float)config.get("WINDOW_HEIGHT"));
		this->near = 0.1f;
		this->far = 100.0f;

		this->forward = glm::vec3(0.0f, 0.0f, -1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->side = glm::vec3(1.0f, 0.0f, 0.0f);

		calculateProjectionMatrix();
		calculateOrthoMatrix();
	}

	Camera::Camera(float near, float far) : Component()
	{
		addType(ID("CAMERA"));

		fov = (float)config.get("FOV");
		aspectRatio = ((float)config.get("WINDOW_WIDTH"))/((float)config.get("WINDOW_HEIGHT"));
		this->near = near;
		this->far = far;

		this->forward = glm::vec3(0.0f, 0.0f, -1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->side = glm::vec3(1.0f, 0.0f, 0.0f);

		calculateProjectionMatrix();
		calculateOrthoMatrix();
	}

	glm::mat4 Camera::getProjectionMatrix()
	{
		return projectionMatrix;
	}

	void Camera::calculateProjectionMatrix()
	{
		projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, near, far);
	}

	glm::mat4 Camera::getViewMatrix()
	{
		return viewMatrix;
	}

	void Camera::calculateViewMatrix()
	{
		if (state != NULL)
		{
			Transform* transform = state->getComponent<Transform>(entityId);

			if (transform != NULL)
			{
				glm::vec3 pos = transform->position;
				glm::vec3 rot = transform->rotation;
				float pitch = rot.x;
				float yaw = rot.y;

				forward.x = cos(pitch) * sin(yaw);
				forward.y = sin(pitch);
				forward.z = cos(pitch) * cos(yaw);
				forward = glm::normalize(forward);
				up = glm::vec3(0.0f, 1.0f, 0.0f);
				side = glm::normalize(glm::cross(forward, up));

				viewMatrix = glm::lookAt(pos, pos + forward, up);
			}
			else
			{
				LOG_E << "Transform does not exist for " << entityId.get() << ", cannot calculate view matrix";
			}
		}
		else
		{
			LOG_E << entityId.get() << " has not been added to a state yet, cannot calculate view matrix";
		}
	}

	glm::mat4 Camera::getOrthoMatrix()
	{
		return orthoMatrix;
	}

	void Camera::calculateOrthoMatrix()
	{
		orthoMatrix = glm::ortho(0.0f, (float)config.get("WINDOW_WIDTH"), 0.0f, (float)config.get("WINDOW_HEIGHT"), -1000.0f, 1000.0f);  
	}

	glm::vec3 Camera::getForward()
	{
		return forward;
	}

	glm::vec3 Camera::getUp()
	{
		return up;
	}

	glm::vec3 Camera::getSide()
	{
		return side;
	}


	/*
		Draw
			*/
	Draw::Draw() : Component()
	{
		isMouseSelectable = false;
		isVisible = false;
	}

	void Draw::addToMouseSelectable() {}
	void Draw::removeFromMouseSelectable() {}


	/*
		DrawModel
					*/
	DrawModel::DrawModel() : Draw()
	{
		addType(ID("DRAWMODEL"));

		model = assets.modelStack.get("MO_ERROR");
		this->isVisible = false;
	}

	DrawModel::DrawModel(ID modelId, bool isVisible) : Draw()
	{
		addType(ID("DRAWMODEL"));

		model = assets.modelStack.get(modelId);
		this->isVisible = isVisible;
	}

	void DrawModel::render(ID cameraId)
	{
		if (isVisible)
		{
			int textureCount = 0;

			glUseProgram(model->material->shader->GLid);

			//Bind all int values to the shader
			for (size_t i = 0; i < model->material->integerMaterialArguments.size(); i++)
			{
				glUniform1i(glGetUniformLocation(model->material->shader->GLid, (const GLchar*)("int_" + iToS(i)).c_str()), model->material->integerMaterialArguments[i]);
			}
			//Bind all float values to the shader
			for (size_t i = 0; i < model->material->floatMaterialArguments.size(); i++)
			{
				glUniform1f(glGetUniformLocation(model->material->shader->GLid, (const GLchar*)("float_" + iToS(i)).c_str()), model->material->floatMaterialArguments[i]);
			}

			//Bind all vec4 values to the shader
			for (size_t i = 0; i < model->material->vec4MaterialArguments.size(); i++)
			{
				glUniform4f(glGetUniformLocation(model->material->shader->GLid, (const GLchar*)("vec4_" + iToS(i)).c_str()), 
					model->material->vec4MaterialArguments[i][0],
					model->material->vec4MaterialArguments[i][1],
					model->material->vec4MaterialArguments[i][2],
					model->material->vec4MaterialArguments[i][3]);
			}

			//Bind all textures to the shader
			for (size_t i = 0; i < model->material->textureMaterialArguments.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + textureCount);

				if (model->material->textureMaterialArguments[i]->array)
				{
					glBindTexture(GL_TEXTURE_2D_ARRAY, model->material->textureMaterialArguments[i]->GLid);
					glUniform1i(glGetUniformLocation(model->material->shader->GLid, (const GLchar*)("textureArray_" + iToS(i)).c_str()), textureCount);
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, model->material->textureMaterialArguments[i]->GLid);
					glUniform1i(glGetUniformLocation(model->material->shader->GLid, (const GLchar*)("texture_" + iToS(i)).c_str()), textureCount);
				}

				textureCount++;
			}

			//Bind transform to shader
			Camera* cam = state->getComponent<Camera>(cameraId);

			glm::mat4 pvw = cam->getProjectionMatrix() * cam->getViewMatrix() * state->getComponent<Transform>(entityId)->getWorldMatrix();
			glUniformMatrix4fv(glGetUniformLocation(model->material->shader->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));

			glBindVertexArray(model->mesh->VAOid);
				glDrawElements(GL_TRIANGLES, model->mesh->indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}

	void DrawModel::mouseRender(ID cameraId, unsigned int index)
	{
		if (isVisible)
		{
			glm::vec4 indexColor = glm::vec4((float)((index >> 16) & 0xff)/255.0,
											 (float)((index >> 8) & 0xff)/255.0,
											 (float)((index >> 0) & 0xff)/255.0,
											 1.0);

			Shader* shad = assets.shaderStack.get(ID("SH_COLOR"));
			glUseProgram(shad->GLid);

			glUniform4f(glGetUniformLocation(shad->GLid, (const GLchar*)"flatColor"), 
				indexColor[0],
				indexColor[1],
				indexColor[2],
				indexColor[3]);

			Camera* cam = state->getComponent<Camera>(cameraId);

			glm::mat4 pvw = cam->getProjectionMatrix() * cam->getViewMatrix() * state->getComponent<Transform>(entityId)->getWorldMatrix();
			glUniformMatrix4fv(glGetUniformLocation(model->material->shader->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));

			glBindVertexArray(model->mesh->VAOid);
				glDrawElements(GL_TRIANGLES, model->mesh->indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}

	void DrawModel::onStateInsert()
	{
		state->modelPointers.push_back(this);
	}

	void DrawModel::onStateRemove()
	{
		for (auto ni = state->modelPointers.begin(); ni != state->modelPointers.end(); ni++)
		{
			if ((*ni)->entityId.is(entityId) && (*ni)->sameTypes((Component*) this))
			{
				state->modelPointers.erase(ni);
			}
		}
	}

	void DrawModel::addToMouseSelectable()
	{
		if (!isMouseSelectable)
		{
			state->mouseSelectModels.push_back(this);
			isMouseSelectable = true;	
		}
	}
	
	void DrawModel::removeFromMouseSelectable()
	{
		if (isMouseSelectable)
		{
			for (auto ni = state->mouseSelectModels.begin(); ni != state->mouseSelectModels.end(); ni++)
			{
				if ((*ni)->entityId.is(entityId) && (*ni)->sameTypes((Component*) this))
				{
					state->mouseSelectModels.erase(ni);
				}
			}

			isMouseSelectable = false;
		}
	}


	/*
		DrawOrtho
					*/
	DrawOrtho::DrawOrtho() : Draw()
	{
		addType(ID("DrawOrtho"));
		layer = 0;
	}

	void DrawOrtho::render(ID cameraId) {}
	void DrawOrtho::mouseRender(ID cameraId, unsigned int index) {}

	void DrawOrtho::setLayer(int layer)
	{
		this->layer = layer;
		std::sort(state->orthoPointers.begin(), state->orthoPointers.end(), DrawOrtho::compare);
	}

	void DrawOrtho::onStateInsert()
	{
		state->orthoPointers.push_back(this);
		std::sort(state->orthoPointers.begin(), state->orthoPointers.end(), DrawOrtho::compare);
	}

	void DrawOrtho::onStateRemove()
	{
		for (auto oi = state->orthoPointers.begin(); oi != state->orthoPointers.end(); oi++)
		{
			if ((*oi)->entityId.is(entityId) && (*oi)->sameTypes((Component*) this))
			{
				state->orthoPointers.erase(oi);
			}
		}
	}

	bool DrawOrtho::compare(DrawOrtho* l, DrawOrtho* r)
	{
		return (l->layer < r->layer);
	}

	void DrawOrtho::addToMouseSelectable()
	{
		if (!isMouseSelectable)
		{
			state->mouseSelectOrthos.push_back(this);
			std::sort(state->mouseSelectOrthos.begin(), state->mouseSelectOrthos.end(), DrawOrtho::compare);
			isMouseSelectable = true;
		}
	}
	
	void DrawOrtho::removeFromMouseSelectable()
	{
		if (isMouseSelectable)
		{
			for (auto oi = state->mouseSelectOrthos.begin(); oi != state->mouseSelectOrthos.end(); oi++)
			{
				if ((*oi)->entityId.is(entityId) && (*oi)->sameTypes((Component*) this))
				{
					state->mouseSelectOrthos.erase(oi);
				}
			}

			isMouseSelectable = false;
		}
	}

	/*
		DrawRectangle
						*/
	DrawRectangle::DrawRectangle(float x, float y, float width, float height, float pivotX, float pivotY, glm::vec4 color, bool isVisible) : DrawOrtho()
	{
		addType(ID("DRAWRECTANGLE"));

		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->pivotX = pivotX;
		this->pivotY = pivotY;
		this->color = color;
	}

	void DrawRectangle::calculateTransform()
	{
		if (state != NULL)
		{
			Transform* transform = state->getComponent<Transform>(entityId);

			if (transform != NULL)
			{
				transform->scale = glm::vec3(width, height, 0.0f);
				transform->position = glm::vec3(x - pivotX, y - pivotY, (float) layer);
				transform->calculateWorldMatrix();
			}
			else
			{
				LOG_E << "Transform does not exist for " << entityId.get() << ", cannot calculate DrawRectangle transform";
			}
		}
		else
		{
			LOG_E << entityId.get() << " has not been added to a state yet, cannot calculate DrawRectangle transform";
		}
	}

	void DrawRectangle::render(ID cameraId)
	{
		if (state != NULL)
		{
			if (isVisible)
			{
				Mesh* mesh = assets.meshStack.get(ID("ME_QUAD"));
				Shader* shad = assets.shaderStack.get(ID("SH_COLOR"));
				glUseProgram(shad->GLid);

				glUniform4f(glGetUniformLocation(shad->GLid, (const GLchar*)"flatColor"), 
					color[0],
					color[1],
					color[2],
					color[3]);

				//Bind transform to shader
				glm::mat4 pvw = state->getComponent<Camera>(cameraId)->getOrthoMatrix() * state->getComponent<Transform>(entityId)->getWorldMatrix();
				glUniformMatrix4fv(glGetUniformLocation(shad->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));				

				glBindVertexArray(mesh->VAOid);
					glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}
		else
		{
			LOG_E << entityId.get() << " has not been added to a state yet, cannot render DrawRectangle";
		}
	}

	void DrawRectangle::mouseRender(ID cameraId, unsigned int index)
	{
		if (state != NULL)
		{
			if (isVisible)
			{
				glm::vec4 indexColor = glm::vec4((float)((index >> 16) & 0xff)/255.0,
												 (float)((index >> 8) & 0xff)/255.0,
												 (float)((index >> 0) & 0xff)/255.0,
												 1.0);

				Mesh* mesh = assets.meshStack.get(ID("ME_QUAD"));
				Shader* shad = assets.shaderStack.get(ID("SH_COLOR"));
				glUseProgram(shad->GLid);

				glUniform4f(glGetUniformLocation(shad->GLid, (const GLchar*)"flatColor"), 
					indexColor[0],
					indexColor[1],
					indexColor[2],
					indexColor[3]);

				//Bind transform to shader
				glm::mat4 pvw = state->getComponent<Camera>(cameraId)->getOrthoMatrix() * state->getComponent<Transform>(entityId)->getWorldMatrix();
				glUniformMatrix4fv(glGetUniformLocation(shad->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));				

				glBindVertexArray(mesh->VAOid);
					glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}
		else
		{
			LOG_E << entityId.get() << " has not been added to a state yet, cannot render DrawRectangle";
		}
	}


	/*
		DrawSprite
					*/
	DrawSprite::DrawSprite(float x, float y, float scaleX, float scaleY, float pivotX, float pivotY, ID spriteId, bool isVisible) : DrawOrtho()
	{
		addType(ID("DRAWSPRITE"));

		this->x = x;
		this->y = y;
		this->scaleX = scaleX;
		this->scaleY = scaleY;
		this->pivotX = pivotX;
		this->pivotY = pivotY;
		this->sprite = assets.spriteStack.get(spriteId);
		this->isVisible = isVisible;
	}

	void DrawSprite::calculateTransform()
	{
		if (state != NULL)
		{
			Transform* transform = state->getComponent<Transform>(entityId);

			if (transform != NULL)
			{
				transform->scale = glm::vec3(scaleX * sprite->width, scaleY * sprite->height, 0.0f);
				transform->position = glm::vec3(x - pivotX, y - pivotY, (float) layer);
				transform->calculateWorldMatrix();
			}
			else
			{
				LOG_E << "Transform does not exist for " << entityId.get() << ", cannot calculate DrawSprite transform";
			}
		}
		else
		{
			LOG_E << entityId.get() << " has not been added to a state yet, cannot calculate DrawSprite transform";
		}
	}

	void DrawSprite::render(ID cameraId)
	{
		if (state != NULL)
		{
			if (isVisible)
			{
				Mesh* mesh = assets.meshStack.get(ID("ME_QUAD"));
				Shader* shad = assets.shaderStack.get(ID("SH_SPRITE"));
				glUseProgram(shad->GLid);

				glUniform1f(glGetUniformLocation(shad->GLid, (const GLchar*)"texCoordScaleX"), sprite->texCoordX);
				glUniform1f(glGetUniformLocation(shad->GLid, (const GLchar*)"texCoordScaleY"), sprite->texCoordY);
				
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, sprite->GLid);
				glUniform1i(glGetUniformLocation(shad->GLid, (const GLchar*)"sprite"), 0);

				//Bind transform to shader
				glm::mat4 pvw = state->getComponent<Camera>(cameraId)->getOrthoMatrix() * state->getComponent<Transform>(entityId)->getWorldMatrix();
				glUniformMatrix4fv(glGetUniformLocation(shad->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));				

				glBindVertexArray(mesh->VAOid);
					glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}
		else
		{
			LOG_E << entityId.get() << " has not been added to a state yet, cannot render DrawSprite";
		}
	}

	void DrawSprite::mouseRender(ID cameraId, unsigned int index)
	{
		if (state != NULL)
		{
			if (isVisible)
			{
				glm::vec4 indexColor = glm::vec4((float)((index >> 16) & 0xff)/255.0,
												 (float)((index >> 8) & 0xff)/255.0,
												 (float)((index >> 0) & 0xff)/255.0,
												 1.0);

				Mesh* mesh = assets.meshStack.get(ID("ME_QUAD"));
				Shader* shad = assets.shaderStack.get(ID("SH_COLOR")); //Should really be a custom shader which culls transparent pixels of original sprite
				glUseProgram(shad->GLid);

				glUniform4f(glGetUniformLocation(shad->GLid, (const GLchar*)"flatColor"), 
					indexColor[0],
					indexColor[1],
					indexColor[2],
					indexColor[3]);

				//Bind transform to shader
				glm::mat4 pvw = state->getComponent<Camera>(cameraId)->getOrthoMatrix() * state->getComponent<Transform>(entityId)->getWorldMatrix();
				glUniformMatrix4fv(glGetUniformLocation(shad->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));				

				glBindVertexArray(mesh->VAOid);
					glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}
		else
		{
			LOG_E << entityId.get() << " has not been added to a state yet, cannot render DrawSprite";
		}
	}


	/*
		DrawText
				*/
	DrawText::DrawText(float x, float y, std::string text, glm::vec4 color, ID fontId) : DrawOrtho()
	{
		addType(ID("DRAWTEXT"));

		this->x = x;
		this->y = y;
		this->text = text;
		this->color = color;
		this->font = assets.fontStack.get(fontId);

		isVisible = true;
		hAlignment = TextHAlign::left;
		vAlignment = TextVAlign::top;
		justification = TextHAlign::left;
		lineSeperation = 5.0f;

		calculateWidth();
		calculateHeight();
	}

	void DrawText::setText(std::string text)
	{
		this->text = text;

		calculateWidth();
		calculateHeight();
	}

	std::string DrawText::getText()
	{
		return text;
	}

	float DrawText::getWidth()
	{
		return width;
	}

	float DrawText::getHeight()
	{
		return height;
	}

	float DrawText::getLineWidth(int line)
	{
		return lineWidths[line];
	}

	void DrawText::calculateWidth()
	{
		float tempWidth = 0.0f;
		width = 0.0f;

		for (size_t i = 0; i < text.length(); i++)
		{
			if (text[i] == '\n')
			{
				lineWidths.push_back(tempWidth);

				if (tempWidth > width)
				{
					width = tempWidth;
				}

				tempWidth = 0.0f;
			}

			FontCharacter* fontChar = &(font->characters[text[i]]);

			if (i == 0)
			{
				if (text.length() == 1)
				{
					tempWidth += fontChar->size.x;
				}
				else
				{
					tempWidth += ((fontChar->advance >> 6) - fontChar->bearing.x);
				}
			}
			else if (i == text.length() - 1)
			{
				tempWidth += (fontChar->bearing.x + fontChar->size.x);
			}
			else
			{
				tempWidth += (fontChar->advance >> 6);
			}
		}

		lineWidths.push_back(tempWidth);

		if (tempWidth > width)
		{
			width = tempWidth;
		}
	}

	void DrawText::calculateHeight()
	{
		height = 0.0f; 
		int lineCount = 1;

		for (size_t i = 0; i < text.length(); i++)
		{
			if (text[i] == '\n')
			{
				lineCount++;
			}
		}

		height = (lineCount * font->maxCharHeight) + ((lineCount - 1) * lineSeperation);
	}

	float DrawText::getDrawStartX(int line)
	{
		float tempX = 0.0f;

		switch(justification)
		{
		case TextHAlign::left:
			tempX = x;
			break;
		case TextHAlign::center:
			tempX = x + ((width - getLineWidth(line)) / 2.0f);
			break;
		case TextHAlign::right:
			tempX = x + (width - getLineWidth(line));
			break;
		}

		switch (hAlignment)
		{
		case TextHAlign::left:
			return tempX;
		case TextHAlign::center:
			return tempX - (width / 2.0f);
		case TextHAlign::right:
			return tempX - width;
		}

		return 0.0f;
	}

	float DrawText::getDrawStartY()
	{
		switch (vAlignment)
		{
		case TextVAlign::top:
			return y;
		case TextVAlign::middle:
			return y + (height / 2.0f);
		case TextVAlign::bottom:
			return y + height;
		}

		return 0.0f;
	}

	void DrawText::render(ID cameraId)
	{
		if (state != NULL)
		{
			if (isVisible)
			{
				Shader* shad = assets.shaderStack.get(ID("SH_FONT"));
				glUseProgram(shad->GLid);

				glUniform4f(glGetUniformLocation(shad->GLid, (const GLchar*)"fontColor"), 
					color[0],
					color[1],
					color[2],
					color[3]);
				
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, font->textureGLid);
				glUniform1i(glGetUniformLocation(shad->GLid, (const GLchar*)"fontTexture"), 0);

				glm::mat4 pvw = state->getComponent<Camera>(cameraId)->getOrthoMatrix();
				glUniformMatrix4fv(glGetUniformLocation(shad->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));

				int currentLine = 0;
				GLfloat x = getDrawStartX(currentLine);
				GLfloat y = getDrawStartY();

				std::string::const_iterator cIt;

				glBindVertexArray(font->VAOid);

				for (cIt = text.begin(); cIt != text.end(); cIt++)
				{
					//New line
					if ((*cIt) == '\n')
					{
						currentLine++;
						x = getDrawStartX(currentLine);
						y -= (font->maxCharHeight + lineSeperation);
					}
					else
					{
						FontCharacter* fontChar = &(font->characters[*cIt]);

						GLfloat xpos = x + fontChar->bearing.x;
						GLfloat ypos = y - (fontChar->size.y - fontChar->bearing.y);

						GLfloat w = fontChar->size.x;
						GLfloat h = fontChar->size.y;

						GLfloat vertices[6][5] = {
							{ xpos,     ypos + h, (float) layer,  fontChar->textureCoords.x, fontChar->textureCoords.z },
							{ xpos,     ypos,     (float) layer,  fontChar->textureCoords.x, fontChar->textureCoords.w },
							{ xpos + w, ypos,     (float) layer,  fontChar->textureCoords.y, fontChar->textureCoords.w },

							{ xpos,     ypos + h, (float) layer,  fontChar->textureCoords.x, fontChar->textureCoords.z },
							{ xpos + w, ypos,     (float) layer,  fontChar->textureCoords.y, fontChar->textureCoords.w },
							{ xpos + w, ypos + h, (float) layer,  fontChar->textureCoords.y, fontChar->textureCoords.z }
						};

						glBindBuffer(GL_ARRAY_BUFFER, font->VBOid);
							glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
						glBindBuffer(GL_ARRAY_BUFFER, 0);

						glDrawArrays(GL_TRIANGLES, 0, 6);

						x += (fontChar->advance >> 6);
					}
				}

				glBindVertexArray(0);
			}
		}
		else
		{
			LOG_E << entityId.get() << " has not been added to a state yet, cannot render DrawText";
		}
	}

	void DrawText::mouseRender(ID cameraId, unsigned int index)
	{
		if (state != NULL)
		{
			if (isVisible)
			{
				glm::vec4 indexColor = glm::vec4((float)((index >> 16) & 0xff)/255.0,
												 (float)((index >> 8) & 0xff)/255.0,
												 (float)((index >> 0) & 0xff)/255.0,
												 1.0);

				Shader* shad = assets.shaderStack.get(ID("SH_COLOR"));
				glUseProgram(shad->GLid);

				glUniform4f(glGetUniformLocation(shad->GLid, (const GLchar*)"flatColor"), 
					indexColor[0],
					indexColor[1],
					indexColor[2],
					indexColor[3]);

				glm::mat4 pvw = state->getComponent<Camera>(cameraId)->getOrthoMatrix();
				glUniformMatrix4fv(glGetUniformLocation(shad->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));

				int currentLine = 0;
				GLfloat x = getDrawStartX(currentLine);
				GLfloat y = getDrawStartY();

				std::string::const_iterator cIt;

				glBindVertexArray(font->VAOid);

				for (cIt = text.begin(); cIt != text.end(); cIt++)
				{
					//New line
					if ((*cIt) == '\n')
					{
						currentLine++;
						x = getDrawStartX(currentLine);
						y -= (font->maxCharHeight + lineSeperation);
					}
					else
					{
						FontCharacter* fontChar = &(font->characters[*cIt]);

						GLfloat xpos = x + fontChar->bearing.x;
						GLfloat ypos = y - (fontChar->size.y - fontChar->bearing.y);

						GLfloat w = fontChar->size.x;
						GLfloat h = fontChar->size.y;

						GLfloat vertices[6][5] = {
							{ xpos,     ypos + h, (float) layer,  fontChar->textureCoords.x, fontChar->textureCoords.z },
							{ xpos,     ypos,     (float) layer,  fontChar->textureCoords.x, fontChar->textureCoords.w },
							{ xpos + w, ypos,     (float) layer,  fontChar->textureCoords.y, fontChar->textureCoords.w },

							{ xpos,     ypos + h, (float) layer,  fontChar->textureCoords.x, fontChar->textureCoords.z },
							{ xpos + w, ypos,     (float) layer,  fontChar->textureCoords.y, fontChar->textureCoords.w },
							{ xpos + w, ypos + h, (float) layer,  fontChar->textureCoords.y, fontChar->textureCoords.z }
						};

						glBindBuffer(GL_ARRAY_BUFFER, font->VBOid);
							glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
						glBindBuffer(GL_ARRAY_BUFFER, 0);

						glDrawArrays(GL_TRIANGLES, 0, 6);

						x += (fontChar->advance >> 6);
					}
				}

				glBindVertexArray(0);
			}
		}
		else
		{
			LOG_E << entityId.get() << " has not been added to a state yet, cannot render DrawText";
		}
	}
}