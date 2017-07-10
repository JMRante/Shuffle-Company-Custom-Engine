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
		entityId = CTID("NULL");
		addType(CTID("NULL"));
		state = NULL;
		isActive = true;
	}

	bool Component::isType(ID id)
	{
		if (find(types.begin(), types.end(), id) != types.end())
		{
			return true;
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
			if (find(comp->types.begin(), comp->types.end(), types[i]) != comp->types.end())
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

	bool Component::getActive()
	{
		Transform* tran = state->getComponent<Transform>(entityId);

		if (tran != NULL)
		{
			return (tran->getActive() && isActive);
		}

		return isActive;
	}

	void Component::setActive(bool set)
	{
		isActive = set;
	}


	/*
		Transform
					*/
	Transform::Transform() : Component()
	{
		addType(CTID("TRANSFORM"));

		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		this->scale = glm::vec3(1.0f, 1.0f, 1.0f);

		parent = NULL;
		dirty = true;

		useParentTransform = true;
		useParentScale = true;
		useParentRotation = true;
	}

	Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : Component()
	{
		addType(CTID("TRANSFORM"));

		this->position = position;
		this->rotation = rotation;
		this->scale = scale;

		parent = NULL;
		dirty = true;

		useParentTransform = true;
		useParentScale = true;
		useParentRotation = true;
	}

	void Transform::onStateInsert() 
	{
		state->transformPointers.push_back(this);
	}

	void Transform::onStateRemove()
	{
		for (auto oi = state->transformPointers.begin(); oi != state->transformPointers.end(); oi++)
		{
			if ((*oi)->entityId == entityId && (*oi)->sameTypes((Component*) this))
			{
				state->transformPointers.erase(oi);
			}
		}
	}

	bool Transform::getActive()
	{
		return isActive;
	}

	void Transform::setActive(bool set)
	{
		isActive = set;

		for (auto it = children.begin(); it != children.end(); it++) 
		{
			(*it)->setActive(set);
		}
	}

	glm::mat4 Transform::calculate()
	{
		if (dirty)
		{
			if (parent == NULL)
			{
				matrix = glm::translate(glm::mat4(1.0f), position) * glm::eulerAngleYXZ(rotation[1], rotation[0], rotation[2]) * glm::scale(glm::mat4(1.0f), scale);
			}
			else
			{
				matrix = parent->calculate();

				if (!useParentTransform)
				{
					matrix *= glm::translate(glm::mat4(1.0f), glm::vec3(-parent->getPosX(), -parent->getPosY(), -parent->getPosZ()));
				}

				if (!useParentRotation)
				{
					matrix *= glm::eulerAngleYXZ(-parent->getRotY(), -parent->getRotX(), -parent->getRotZ());
				}

				if (!useParentScale)
				{
					matrix *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/parent->getScaX(), 1.0f/parent->getScaY(), 1.0f/parent->getScaZ()));				
				}

				matrix *= glm::translate(glm::mat4(1.0f), position) * glm::eulerAngleYXZ(rotation[1], rotation[0], rotation[2]) * glm::scale(glm::mat4(1.0f), scale);
			}

			dirty = false;
		}

		return matrix;
	}

	glm::mat4 Transform::getMatrix()
	{
		return matrix;
	}

	void Transform::setParent(Transform* newParent)
	{	
		if (parent != NULL)
		{
			for (auto it = parent->children.begin(); it != parent->children.end(); it++) 
			{
				if ((*it)->entityId == entityId)
				{
					parent->children.erase(it);
				}
			}
		}

		parent = newParent;
		parent->children.push_back(this);
	}

	void Transform::removeParent()
	{
		if (parent != NULL)
		{
			for (auto it = parent->children.begin(); it != parent->children.end(); it++) 
			{
				if ((*it)->entityId == entityId)
				{
					parent->children.erase(it);
				}
			}
		}

		parent = NULL;
	}

	glm::vec3 Transform::getPos() 
	{
		return position;
	}

	float Transform::getPosX() 
	{
		return position.x;
	}

	float Transform::getPosY() 
	{
		return position.y;
	}

	float Transform::getPosZ() 
	{
		return position.z;
	}

	void Transform::setPos(glm::vec3 pos) 
	{
		position = pos;
		dirty = true;
	}

	void Transform::setPosX(float x) 
	{
		position.x = x;
		dirty = true;
	}

	void Transform::setPosY(float y) 
	{
		position.y = y;
		dirty = true;
	}

	void Transform::setPosZ(float z) 
	{
		position.z = z;
		dirty = true;
	}

	glm::vec3 Transform::getRot() 
	{
		return rotation;
	}

	float Transform::getRotX() 
	{
		return rotation.x;
	}

	float Transform::getRotY() 
	{
		return rotation.y;
	}

	float Transform::getRotZ() 
	{
		return rotation.z;
	}

	void Transform::setRot(glm::vec3 rot) 
	{
		rotation = rot;
		dirty = true;
	}

	void Transform::setRotX(float x) 
	{
		rotation.x = x;
		dirty = true;
	}

	void Transform::setRotY(float y) 
	{
		rotation.y = y;
		dirty = true;
	}

	void Transform::setRotZ(float z) 
	{
		rotation.z = z;
		dirty = true;
	}

	glm::vec3 Transform::getSca() 
	{
		return scale;
	}

	float Transform::getScaX() 
	{
		return scale.x;
	}

	float Transform::getScaY() 
	{
		return scale.y;
	}

	float Transform::getScaZ() 
	{
		return scale.z;
	}

	void Transform::setSca(glm::vec3 sca) 
	{
		scale = sca;
		dirty = true;
	}

	void Transform::setScaX(float x) 
	{
		scale.x = x;
		dirty = true;
	}

	void Transform::setScaY(float y) 
	{
		scale.y = y;
		dirty = true;
	}

	void Transform::setScaZ(float z) 
	{
		scale.z = z;
		dirty = true;
	}


	/*
		Camera
				*/
	Camera::Camera() : Component()
	{
		addType(CTID("CAMERA"));

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
		addType(CTID("CAMERA"));

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
				glm::vec3 pos = transform->getPos();
				glm::vec3 rot = transform->getRot();
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
				LOG_E << "Transform does not exist, cannot calculate view matrix";
			}
		}
		else
		{
			LOG_E << "Entity has not been added to a state yet, cannot calculate view matrix";
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
	}

	void Draw::addToMouseSelectable() {}
	void Draw::removeFromMouseSelectable() {}


	/*
		DrawModel
					*/
	DrawModel::DrawModel() : Draw()
	{
		addType(CTID("DRAWMODEL"));

		model = assets.modelStack.get(CTID("MO_ERROR"));
	}

	DrawModel::DrawModel(ID modelId) : Draw()
	{
		addType(CTID("DRAWMODEL"));

		model = assets.modelStack.get(modelId);
	}

	void DrawModel::render(ID cameraId)
	{
		if (getActive())
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

			glm::mat4 pvw = cam->getProjectionMatrix() * cam->getViewMatrix() * state->getComponent<Transform>(entityId)->getMatrix();
			glUniformMatrix4fv(glGetUniformLocation(model->material->shader->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));

			glBindVertexArray(model->mesh->VAOid);
				glDrawElements(GL_TRIANGLES, model->mesh->indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}

	void DrawModel::mouseRender(ID cameraId, unsigned int index)
	{
		if (getActive())
		{
			glm::vec4 indexColor = glm::vec4((float)((index >> 16) & 0xff)/255.0,
											 (float)((index >> 8) & 0xff)/255.0,
											 (float)((index >> 0) & 0xff)/255.0,
											 1.0);

			Shader* shad = assets.shaderStack.get(CTID("SH_COLOR"));
			glUseProgram(shad->GLid);

			glUniform4f(glGetUniformLocation(shad->GLid, (const GLchar*)"flatColor"), 
				indexColor[0],
				indexColor[1],
				indexColor[2],
				indexColor[3]);

			Camera* cam = state->getComponent<Camera>(cameraId);

			glm::mat4 pvw = cam->getProjectionMatrix() * cam->getViewMatrix() * state->getComponent<Transform>(entityId)->getMatrix();
			glUniformMatrix4fv(glGetUniformLocation(model->material->shader->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));

			glBindVertexArray(model->mesh->VAOid);
				glDrawElements(GL_TRIANGLES, model->mesh->indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}

	void DrawModel::stageMouseRender(ID cameraId)
	{
		if (getActive())
		{
			Shader* shad = assets.shaderStack.get(CTID("SH_STAGESELECT"));
			glUseProgram(shad->GLid);

			Camera* cam = state->getComponent<Camera>(cameraId);

			glm::mat4 pvw = cam->getProjectionMatrix() * cam->getViewMatrix() * state->getComponent<Transform>(entityId)->getMatrix();
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
			if ((*ni)->entityId == entityId && (*ni)->sameTypes((Component*) this))
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
				if ((*ni)->entityId == entityId && (*ni)->sameTypes((Component*) this))
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
		addType(CTID("DrawOrtho"));
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
			if ((*oi)->entityId == entityId && (*oi)->sameTypes((Component*) this))
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
				if ((*oi)->entityId == entityId && (*oi)->sameTypes((Component*) this))
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
	DrawRectangle::DrawRectangle() : DrawOrtho()
	{
		addType(CTID("DRAWRECTANGLE"));
	}

	void DrawRectangle::initialize(float x, float y, float width, float height, float pivotX, float pivotY, glm::vec4 color)
	{
		Transform* tran = state->getComponent<Transform>(entityId);

		if (tran != NULL)
		{
			tran->setPosX(x);
			tran->setPosY(y);
			tran->setScaX(width);
			tran->setScaY(height);
			tran->useParentScale = false;
		}
		else
		{
			LOG_E << "Entity cannot have a DrawRectangle, it doesn't have a Transform";
		}

		this->pivotX = pivotX;
		this->pivotY = pivotY;
		this->color = color;
	}

	void DrawRectangle::render(ID cameraId)
	{
		if (state != NULL)
		{
			if (getActive())
			{
				Mesh* mesh = assets.meshStack.get(CTID("ME_QUAD"));
				Shader* shad = assets.shaderStack.get(CTID("SH_COLOR"));
				glUseProgram(shad->GLid);

				glUniform4f(glGetUniformLocation(shad->GLid, (const GLchar*)"flatColor"), 
					color[0],
					color[1],
					color[2],
					color[3]);

				//Bind transform to shader
				glm::mat4 pvw = state->getComponent<Camera>(cameraId)->getOrthoMatrix() * glm::translate(glm::mat4(1.0f), glm::vec3(pivotX, pivotY, 0.0f)) * state->getComponent<Transform>(entityId)->getMatrix();
				glUniformMatrix4fv(glGetUniformLocation(shad->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));				

				glBindVertexArray(mesh->VAOid);
					glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}
		else
		{
			LOG_E << "Entity has not been added to a state yet, cannot render DrawRectangle";
		}
	}

	void DrawRectangle::mouseRender(ID cameraId, unsigned int index)
	{
		if (state != NULL)
		{
			if (getActive())
			{
				glm::vec4 indexColor = glm::vec4((float)((index >> 16) & 0xff)/255.0,
												 (float)((index >> 8) & 0xff)/255.0,
												 (float)((index >> 0) & 0xff)/255.0,
												 1.0);

				Mesh* mesh = assets.meshStack.get(CTID("ME_QUAD"));
				Shader* shad = assets.shaderStack.get(CTID("SH_COLOR"));
				glUseProgram(shad->GLid);

				glUniform4f(glGetUniformLocation(shad->GLid, (const GLchar*)"flatColor"), 
					indexColor[0],
					indexColor[1],
					indexColor[2],
					indexColor[3]);

				//Bind transform to shader
				glm::mat4 pvw = state->getComponent<Camera>(cameraId)->getOrthoMatrix() * glm::translate(glm::mat4(1.0f), glm::vec3(pivotX, pivotY, 0.0f)) * state->getComponent<Transform>(entityId)->getMatrix();
				glUniformMatrix4fv(glGetUniformLocation(shad->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));				

				glBindVertexArray(mesh->VAOid);
					glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}
		else
		{
			LOG_E << "Entity has not been added to a state yet, cannot render DrawRectangle";
		}
	}


	/*
		DrawSprite
					*/
	DrawSprite::DrawSprite() : DrawOrtho()
	{
		addType(CTID("DRAWSPRITE"));
	}

	void DrawSprite::initialize(float x, float y, float scaleX, float scaleY, float pivotX, float pivotY, ID spriteId)
	{
		Transform* tran = state->getComponent<Transform>(entityId);
		this->sprite = assets.spriteStack.get(spriteId);

		if (tran != NULL)
		{
			tran->setPosX(x);
			tran->setPosY(y);
			tran->setScaX(sprite->width);
			tran->setScaY(sprite->height);
			tran->useParentScale = false;
		}
		else
		{
			LOG_E << "Entity cannot have a DrawSprite, it doesn't have a Transform";
		}

		this->pivotX = pivotX;
		this->pivotY = pivotY;
		this->scaleX = scaleX;
		this->scaleY = scaleY;
	}

	void DrawSprite::render(ID cameraId)
	{
		if (state != NULL)
		{
			if (getActive())
			{
				Mesh* mesh = assets.meshStack.get(CTID("ME_QUAD"));
				Shader* shad = assets.shaderStack.get(CTID("SH_SPRITE"));
				glUseProgram(shad->GLid);

				glUniform1f(glGetUniformLocation(shad->GLid, (const GLchar*)"texCoordScaleX"), sprite->texCoordX);
				glUniform1f(glGetUniformLocation(shad->GLid, (const GLchar*)"texCoordScaleY"), sprite->texCoordY);
				
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, sprite->GLid);
				glUniform1i(glGetUniformLocation(shad->GLid, (const GLchar*)"sprite"), 0);

				//Bind transform to shader
				glm::mat4 pvw = state->getComponent<Camera>(cameraId)->getOrthoMatrix() * glm::translate(glm::mat4(1.0f), glm::vec3(pivotX, pivotY, 0)) * state->getComponent<Transform>(entityId)->getMatrix() * glm::scale(glm::mat4(1.0f), glm::vec3(scaleX, scaleY, 0.0f));
				glUniformMatrix4fv(glGetUniformLocation(shad->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));				

				glBindVertexArray(mesh->VAOid);
					glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}
		else
		{
			LOG_E << "Entity has not been added to a state yet, cannot render DrawSprite";
		}
	}

	void DrawSprite::mouseRender(ID cameraId, unsigned int index)
	{
		if (state != NULL)
		{
			if (getActive())
			{
				glm::vec4 indexColor = glm::vec4((float)((index >> 16) & 0xff)/255.0,
												 (float)((index >> 8) & 0xff)/255.0,
												 (float)((index >> 0) & 0xff)/255.0,
												 1.0);

				Mesh* mesh = assets.meshStack.get(CTID("ME_QUAD"));
				Shader* shad = assets.shaderStack.get(CTID("SH_COLOR")); //Should really be a custom shader which culls transparent pixels of original sprite
				glUseProgram(shad->GLid);

				glUniform4f(glGetUniformLocation(shad->GLid, (const GLchar*)"flatColor"), 
					indexColor[0],
					indexColor[1],
					indexColor[2],
					indexColor[3]);

				//Bind transform to shader
				glm::mat4 pvw = state->getComponent<Camera>(cameraId)->getOrthoMatrix() * glm::translate(glm::mat4(1.0f), glm::vec3(pivotX, pivotY, 0)) * state->getComponent<Transform>(entityId)->getMatrix() * glm::scale(glm::mat4(1.0f), glm::vec3(scaleX, scaleY, 0.0f));
				glUniformMatrix4fv(glGetUniformLocation(shad->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));				

				glBindVertexArray(mesh->VAOid);
					glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}
		else
		{
			LOG_E << "Entity has not been added to a state yet, cannot render DrawSprite";
		}
	}


	/*
		DrawText
				*/
	DrawText::DrawText() : DrawOrtho()
	{
		addType(CTID("DRAWTEXT"));
	}

	void DrawText::initialize(float x, float y, std::string text, glm::vec4 color, ID fontId)
	{
		Transform* tran = state->getComponent<Transform>(entityId);

		if (tran != NULL)
		{
			tran->setPosX(x);
			tran->setPosY(y);
			tran->useParentScale = false;
		}
		else
		{
			LOG_E << "Entity cannot have a DrawText, it doesn't have a Transform";
		}

		this->text = text;
		this->color = color;
		this->font = assets.fontStack.get(fontId);

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

	float DrawText::getDrawOffsetX(int line)
	{
		float tempX = 0.0f;

		switch(justification)
		{
		case TextHAlign::left:
			break;
		case TextHAlign::center:
			tempX = (width - getLineWidth(line)) / 2.0f;
			break;
		case TextHAlign::right:
			tempX = width - getLineWidth(line);
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

	float DrawText::getDrawOffsetY()
	{
		switch (vAlignment)
		{
		case TextVAlign::top:
			return 0.0f;
		case TextVAlign::middle:
			return height / 2.0f;
		case TextVAlign::bottom:
			return height;
		}

		return 0.0f;
	}

	void DrawText::render(ID cameraId)
	{
		if (state != NULL)
		{
			if (getActive())
			{
				Transform* tran = state->getComponent<Transform>(entityId);
				Shader* shad = assets.shaderStack.get(CTID("SH_FONT"));
				glUseProgram(shad->GLid);

				glUniform4f(glGetUniformLocation(shad->GLid, (const GLchar*)"fontColor"), 
					color[0],
					color[1],
					color[2],
					color[3]);
				
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, font->textureGLid);
				glUniform1i(glGetUniformLocation(shad->GLid, (const GLchar*)"fontTexture"), 0);

				glm::mat4 pvw = state->getComponent<Camera>(cameraId)->getOrthoMatrix() * tran->getMatrix();
				glUniformMatrix4fv(glGetUniformLocation(shad->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));

				int currentLine = 0;
				GLfloat x = getDrawOffsetX(currentLine);
				GLfloat y = getDrawOffsetY();

				std::string::const_iterator cIt;

				glBindVertexArray(font->VAOid);

				for (cIt = text.begin(); cIt != text.end(); cIt++)
				{
					//New line
					if ((*cIt) == '\n')
					{
						currentLine++;
						x = getDrawOffsetX(currentLine);
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
			LOG_E << "Entity has not been added to a state yet, cannot render DrawText";
		}
	}

	void DrawText::mouseRender(ID cameraId, unsigned int index)
	{
		if (state != NULL)
		{
			if (getActive())
			{
				glm::vec4 indexColor = glm::vec4((float)((index >> 16) & 0xff)/255.0,
												 (float)((index >> 8) & 0xff)/255.0,
												 (float)((index >> 0) & 0xff)/255.0,
												 1.0);

				Transform* tran = state->getComponent<Transform>(entityId);
				Shader* shad = assets.shaderStack.get(CTID("SH_COLOR"));
				glUseProgram(shad->GLid);

				glUniform4f(glGetUniformLocation(shad->GLid, (const GLchar*)"flatColor"), 
					indexColor[0],
					indexColor[1],
					indexColor[2],
					indexColor[3]);

				glm::mat4 pvw = state->getComponent<Camera>(cameraId)->getOrthoMatrix() * tran->getMatrix();
				glUniformMatrix4fv(glGetUniformLocation(shad->GLid, "PVW"), 1, GL_FALSE, glm::value_ptr(pvw));

				int currentLine = 0;
				GLfloat x = getDrawOffsetX(currentLine);
				GLfloat y = getDrawOffsetY();

				std::string::const_iterator cIt;

				glBindVertexArray(font->VAOid);

				for (cIt = text.begin(); cIt != text.end(); cIt++)
				{
					//New line
					if ((*cIt) == '\n')
					{
						currentLine++;
						x = getDrawOffsetX(currentLine);
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
			LOG_E << "Entity has not been added to a state yet, cannot render DrawText";
		}
	}


	/*
		EditorOperationManager
								*/
	EditorOperation::EditorOperation() {}


	SetBrush::SetBrush(int newBrush, glm::ivec3 position) : EditorOperation()
	{
		this->newBrush = newBrush;
		this->previousBrush = 0;
		this->position = position;	
	}

	void SetBrush::operate()
	{
		Stage* stage = state->getComponent<Stage>(CTID("E_STAGE"));
		previousBrush = stage->get(position.x, position.y, position.z);
		stage->set(position.x, position.y, position.z, newBrush);
		stage->updateStageMesh();
	}

	void SetBrush::reverse()
	{
		Stage* stage = state->getComponent<Stage>(CTID("E_STAGE"));
		stage->set(position.x, position.y, position.z, previousBrush);
		stage->updateStageMesh();
	}


	EditorOperationManager::EditorOperationManager(size_t maxOperations)
	{
		this->maxOperations = maxOperations;
		currentOperation = 0;
	}

	void EditorOperationManager::doOperation(EditorOperation* operation)
	{
		operation->state = state;
		operation->operate();

		if (currentOperation != 0)
		{
			operations.erase(operations.begin(), operations.begin() + currentOperation);
			currentOperation = 0;
		}

		operations.insert(operations.begin(), operation);

		if (operations.size() >= maxOperations)
		{
			operations.pop_back();
		}
	}

	void EditorOperationManager::undoOperation()
	{
		if (currentOperation < operations.size())
		{
			operations[currentOperation]->reverse();
			currentOperation++;
		}
	}

	void EditorOperationManager::redoOperation()
	{
		if (currentOperation > 0)
		{
			currentOperation--;
			operations[currentOperation]->operate();
		}
	}

	void EditorOperationManager::clearOperations()
	{
		operations.clear();
		currentOperation = 0;
	}
}