/*

    SHUFFLE COMPANY SOURCE CODE

    sc_gameElement.h
    ------------------------------------



*/

#ifndef SC_GAMEELEMENT
#define SC_GAMEELEMENT

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "sc_log.h"
#include "sc_assets.h"

namespace sc
{
	struct Transform
	{
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::mat4 transform;
	};

	class GameElement
	{
	public:
		sc::Model *model;

		Transform worldTransform;

		GameElement(std::string modelId);
		GameElement(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::string modelId);
		void update();
		void setPosition(glm::vec3 position);
		void setRotation(glm::vec3 rotation);
		void setScale(glm::vec3 scale);
		void updateTransform();
	};

	class CameraElement : public GameElement
	{
	private:
		glm::mat4 projection;
	public:
		Transform viewTransform;

		float fov;
		float aspectRatio;
		float near;
		float far;

		CameraElement(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::string modelId, float near, float far);
	};
}

#endif