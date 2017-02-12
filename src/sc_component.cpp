/*

    SHUFFLE COMPANY SOURCE CODE

    sc_component.cpp
    ------------------------------------



*/

#include "sc_component.h"
#include <cmath>

namespace sc
{
	/*
		Component
					*/
	Component::Component(std::string id)
	{
		this->id = id;
	}

	std::string Component::getId()
	{
		return id;
	}

	std::string Component::getEntityId()
	{
		return entityId;
	}

	void Component::setEntityId(std::string id)
	{
		entityId = id;
	}

	/*
		Transform
					*/
	Transform::Transform() : Component("TRANSFORM")
	{
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
		calculateWorldMatrix();
	}

	Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : Component("TRANSFORM")
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
	Camera::Camera(Transform* transform) : Component("CAMERA")
	{
		fov = (float)sc::config.get("FOV");
		aspectRatio = ((float)sc::config.get("WINDOW_WIDTH"))/((float)sc::config.get("WINDOW_HEIGHT"));
		this->near = 0.1f;
		this->far = 100.0f;

		this->transform = transform;
		this->forward = glm::vec3(0.0f, 0.0f, -1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->side = glm::vec3(1.0f, 0.0f, 0.0f);

		calculateViewMatrix();
		calculateProjectionMatrix();
	}

	Camera::Camera(Transform* transform, float near, float far) : Component("CAMERA")
	{
		fov = (float)sc::config.get("FOV");
		aspectRatio = ((float)sc::config.get("WINDOW_WIDTH"))/((float)sc::config.get("WINDOW_HEIGHT"));
		this->near = near;
		this->far = far;

		this->transform = transform;
		this->forward = glm::vec3(0.0f, 0.0f, -1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->side = glm::vec3(1.0f, 0.0f, 0.0f);

		calculateViewMatrix();
		calculateProjectionMatrix();
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

	void Camera::setTransform(Transform* transform)
	{
		this->transform = transform;
	}


	/*
		Draw
				*/
	Draw::Draw() : Component("DRAW")
	{
		model = NULL;
		this->isVisible = false;
	}

	Draw::Draw(std::string modelId, bool isVisible) : Component("DRAW")
	{
		model = assets.getModel(modelId);
		this->isVisible = isVisible;
	}

	void Draw::setModel(std::string modelId)
	{
		model = assets.getModel(modelId);
	}

	Model* Draw::getModel()
	{
		return model;
	}
}