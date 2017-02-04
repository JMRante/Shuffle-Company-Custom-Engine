/*

    SHUFFLE COMPANY SOURCE CODE

    sc_gameElement.cpp
    ------------------------------------



*/

#include "sc_gameElement.h"

namespace sc
{
	GameElement::GameElement(std::string modelId)
	{
		worldTransform.position = glm::vec3(0.0f, 0.0f, 0.0f);
		worldTransform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		worldTransform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		updateTransform();

		model = assets.getModel(modelId);
	}

	GameElement::GameElement(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::string modelId)
	{
		worldTransform.position = position;
		worldTransform.rotation = rotation;
		worldTransform.scale = scale;
		updateTransform();

		model = assets.getModel(modelId);
	}

	void GameElement::update()
	{

	}

	void GameElement::setPosition(glm::vec3 position)
	{
		worldTransform.position = position;
		updateTransform();
	}

	void GameElement::setRotation(glm::vec3 rotation)
	{
		worldTransform.rotation = rotation;
		updateTransform();
	}

	void GameElement::setScale(glm::vec3 scale)
	{
		worldTransform.scale = scale;
		updateTransform();
	}

	void GameElement::updateTransform()
	{
		worldTransform.transform = glm::translate(glm::mat4(1.0f), worldTransform.position) * glm::mat4_cast(glm::quat(worldTransform.rotation)) * glm::scale(glm::mat4(1.0f), worldTransform.scale);
	}

	CameraElement::CameraElement(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::string modelId, float near, float far) : GameElement(position, rotation, scale, modelId)
	{
		fov = (float)sc::config.get("FOV");

		LOG_D << "FOV: " << fov;

		aspectRatio = ((float)sc::config.get("WINDOW_WIDTH"))/((float)sc::config.get("WINDOW_HEIGHT"));
		this->near = near;
		this->far = far;

		LOG_D << "Aspect Ratio: " << aspectRatio << "Near: " << near << " Far: " << far;

		projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
		viewTransform.transform = projection * worldTransform.transform;
	}
}