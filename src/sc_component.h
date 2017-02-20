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
#include "sc_utility.h"

namespace sc
{
	class Component
	{
	public:
		ID entityId;

		Component();
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
		glm::mat4 orthoMatrix;

		float fov;
		float aspectRatio;
		float near;
		float far;

		glm::vec3 forward;
		glm::vec3 up;
		glm::vec3 side;

	public:
		Camera();
		Camera(float near, float far);

		glm::mat4 getProjectionMatrix();
		void calculateProjectionMatrix();

		glm::mat4 getViewMatrix();
		void calculateViewMatrix();

		glm::mat4 getOrthoMatrix();
		void calculateOrthoMatrix();
		
		glm::vec3 getForward();
		glm::vec3 getUp();
		glm::vec3 getSide();
	};	

	class DrawModel : public Component
	{
	public:
		Model *model;
		bool isVisible;

		DrawModel();
		DrawModel(ID modelId, bool isVisible);
	};
}

#endif