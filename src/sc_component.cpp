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
	}

	/*
		Transform
					*/
	Transform::Transform() : Component()
	{
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
		calculateWorldMatrix();
	}

	Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : Component()
	{
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
		Transform* transform = game.nextState->transformPool.get(entityId);
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

	glm::mat4 Camera::getOrthoMatrix()
	{
		return orthoMatrix;
	}

	void Camera::calculateOrthoMatrix()
	{
		orthoMatrix = glm::ortho(0.0f, (float)config.get("WINDOW_WIDTH"), 0.0f, (float)config.get("WINDOW_HEIGHT"), -1.0f, 1.0f);  
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
		DrawModel
					*/
	DrawModel::DrawModel() : Component()
	{
		model = assets.getModel("MO_ERR");
		this->isVisible = false;
	}

	DrawModel::DrawModel(ID modelId, bool isVisible) : Component()
	{
		model = assets.getModel(modelId);
		this->isVisible = isVisible;
	}


	/*
		DrawRectangle
						*/
	DrawRectangle::DrawRectangle(float x, float y, float width, float height, float pivotX, float pivotY, glm::vec4 color, bool isVisible) : Component()
	{
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
		Transform* transform = game.nextState->transformPool.get(entityId);
		transform->scale = glm::vec3(width, height, 0.0f);
		transform->position = glm::vec3(x - pivotX, y - pivotY, 0.0f);
		transform->calculateWorldMatrix();
	}


	/*
		DrawSprite
					*/
	DrawSprite::DrawSprite(float x, float y, float scaleX, float scaleY, float pivotX, float pivotY, ID spriteId, bool isVisible) : Component()
	{
		this->x = x;
		this->y = y;
		this->scaleX = scaleX;
		this->scaleY = scaleY;
		this->pivotX = pivotX;
		this->pivotY = pivotY;
		this->sprite = assets.getSprite(spriteId);
		this->isVisible = isVisible;
	}

	void DrawSprite::calculateTransform()
	{
		Transform* transform = game.nextState->transformPool.get(entityId);
		transform->scale = glm::vec3(scaleX * sprite->width, scaleY * sprite->height, 0.0f);
		transform->position = glm::vec3(x - pivotX, y - pivotY, 0.0f);
		transform->calculateWorldMatrix();
	}


	/*
		DrawText
				*/
	DrawText::DrawText(float x, float y, std::string text, glm::vec4 color, ID fontId, bool isVisible) : Component()
	{
		this->x = x;
		this->y = y;
		this->text = text;
		this->color = color;
		this->font = assets.getFont(fontId);
		this->isVisible = isVisible;
	}
}