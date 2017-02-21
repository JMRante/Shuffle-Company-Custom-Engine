/*

    SHUFFLE COMPANY SOURCE CODE

    sc_component.cpp
    ------------------------------------



*/

#include "sc_component.h"

#include <cmath>

#include "sc_game.h"
#include "sc_world.h"

namespace sc
{
	/*
		Component
					*/
	Component::Component() {}

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

	void Transform::setPosition(glm::vec3 position)
	{
		this->position = position;
		calculateWorldMatrix();
	}

	void Transform::setRotation(glm::vec3 rotation)
	{
		this->rotation = rotation;
		calculateWorldMatrix();
	}

	void Transform::setScale(glm::vec3 scale)
	{
		this->scale = scale;
		calculateWorldMatrix();
	}

	glm::vec3 Transform::getPosition()
	{
		return position;
	}

	glm::vec3 Transform::getRotation()
	{
		return rotation;
	}

	glm::vec3 Transform::getScale()
	{
		return scale;
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
		Transform* transform = game.nextState->entityManager.transformPool.get(entityId);
		glm::vec3 pos = transform->getPosition();
		glm::vec3 rot = transform->getRotation();
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
	DrawRectangle::DrawRectangle(float x, float y, float width, float height, glm::vec4 color, bool isVisible)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->color = color;
	}

	void DrawRectangle::change(float x, float y, float width, float height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;

		calculateTransform();
	}

	void DrawRectangle::calculateTransform()
	{
		Transform* transform = game.nextState->entityManager.transformPool.get(entityId);
		transform->setScale(glm::vec3(width, height, 0.0f));
		transform->setPosition(glm::vec3(x, y, 0.0f));
	}


	/*
		DrawSprite
					*/
	DrawSprite::DrawSprite(float x, float y, float scaleX, float scaleY, ID texId, bool isVisible)
	{
		this->x = x;
		this->y = y;
		this->scaleX = scaleX;
		this->scaleY = scaleY;
		this->texture = assets.getTexture(texId);
		this->isVisible = isVisible;

		texCoordX = (float)texture->width / (float)powerOfTwo(texture->width);
		texCoordY = (float)texture->height / (float)powerOfTwo(texture->height);
	}

	void DrawSprite::change(float x, float y, float scaleX, float scaleY)
	{
		this->x = x;
		this->y = y;
		this->scaleX = scaleX;
		this->scaleY = scaleY;

		calculateTransform();
	}

	void DrawSprite::calculateTransform()
	{
		Transform* transform = game.nextState->entityManager.transformPool.get(entityId);
		transform->setScale(glm::vec3(scaleX * texture->width, scaleY * texture->height, 0.0f));
		transform->setPosition(glm::vec3(x, y, 0.0f));
	}

	float DrawSprite::getTexCoordScaleX()
	{
		return texCoordX;
	}

	float DrawSprite::getTexCoordScaleY()
	{
		return texCoordY;
	}
}