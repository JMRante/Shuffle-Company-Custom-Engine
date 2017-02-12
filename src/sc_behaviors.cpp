/*

    SHUFFLE COMPANY SOURCE CODE

    sc_behaviors.cpp
    ------------------------------------



*/

#include "sc_behaviors.h"
#include "sc_world.h"

namespace sc
{
	Behavior::Behavior() : sc::Component("BEHAVIOR")
	{
	}

	DebugCamera::DebugCamera(float moveSpeed, float mouseSpeed) : Behavior()
	{
		this->moveSpeed = moveSpeed;
		this->mouseSpeed = mouseSpeed;
		yaw = 180.0f;
		pitch = 0.0f;
	}

	void DebugCamera::update(sc::World* pastWorld, sc::World* nextWorld)
	{
		sc::Transform* pastTrans = pastWorld->entityManager.getTransform(getEntityId());
		sc::Transform* nextTrans = nextWorld->entityManager.getTransform(getEntityId());
		sc::Camera* pastCamera = pastWorld->entityManager.getCamera(getEntityId());
		sc::Camera* nextCamera = nextWorld->entityManager.getCamera(getEntityId());

		//Rotation
		float mouseXDelta = (float)input.getMouseXDelta();
		float mouseYDelta = (float)input.getMouseYDelta();

		yaw += mouseXDelta * -mouseSpeed;
		pitch += mouseYDelta * -mouseSpeed;
		yaw = glm::mod(yaw, 360.0f);
		pitch = glm::clamp(pitch, -89.0f, 89.0f);

		nextTrans->setRotation(glm::vec3(glm::radians(pitch), glm::radians(yaw), 0.0f));

		//Translation
		glm::vec3 pastPosition = pastTrans->getPosition();
		glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);

		if (input.keyHeld(SDLK_d))
		{
			translate += pastCamera->getSide();
		}

		if (input.keyHeld(SDLK_w))
		{
			translate += pastCamera->getForward();
		}

		if (input.keyHeld(SDLK_a))
		{
			translate -= pastCamera->getSide();
		}

		if (input.keyHeld(SDLK_s))
		{
			translate -= pastCamera->getForward();
		}

		if (translate != glm::vec3(0.0f, 0.0f, 0.0f))
		{
			translate = moveSpeed * glm::normalize(translate);			
		}

		nextTrans->setPosition(pastPosition + translate);


		nextCamera->setTransform(nextTrans);
		nextCamera->calculateViewMatrix();
	}
}