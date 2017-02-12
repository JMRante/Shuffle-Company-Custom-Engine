/*

    SHUFFLE COMPANY SOURCE CODE

    sc_component.h
    ------------------------------------



*/

#ifndef SC_COMPONENT
#define SC_COMPONENT

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "glm/ext.hpp"

#include "sc_log.h"
#include "sc_assets.h"

namespace sc
{
	class Component
	{
	private:
		std::string id;
		std::string entityId;

	public:
		Component(std::string id);

		std::string getId();
		std::string getEntityId();
		void setEntityId(std::string id);
	};

	class Transform : public Component
	{
	private:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::mat4 worldMatrix;

	public:
		Transform();
		Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

		glm::mat4 getWorldMatrix();
		void setPosition(glm::vec3 position);
		void setRotation(glm::vec3 rotation);
		void setScale(glm::vec3 scale);
		glm::vec3 getPosition();
		glm::vec3 getRotation();
		glm::vec3 getScale();
		void calculateWorldMatrix();
	};

	class Camera : public Component
	{
	private:
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		float fov;
		float aspectRatio;
		float near;
		float far;

		Transform* transform;
		glm::vec3 forward;
		glm::vec3 up;
		glm::vec3 side;

	public:
		Camera(Transform* transform);
		Camera(Transform* transform, float near, float far);

		glm::mat4 getProjectionMatrix();
		void calculateProjectionMatrix();
		glm::mat4 getViewMatrix();
		void calculateViewMatrix();
		glm::vec3 getForward();
		glm::vec3 getUp();
		glm::vec3 getSide();

		void setTransform(Transform* transform);
	};	

	class Draw : public Component
	{
	private:
		sc::Model *model;

	public:
		bool isVisible;

		Draw();
		Draw(std::string modelId, bool isVisible);

		void setModel(std::string modelId);
		Model *getModel();
	};
}

#endif