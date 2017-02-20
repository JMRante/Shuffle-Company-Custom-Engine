/*

    SHUFFLE COMPANY SOURCE CODE

    sc_nature.cpp
    ------------------------------------



*/

#include "sc_nature.h"
#include "sc_game.h"
#include "sc_world.h"

namespace sc
{
	Nature::Nature() : sc::Component() {}

	DebugCamera::DebugCamera(float moveSpeed, float mouseSpeed) : Nature()
	{
		this->moveSpeed = moveSpeed;
		this->mouseSpeed = mouseSpeed;
		yaw = 180.0f;
		pitch = 0.0f;
	}

	void DebugCamera::update()
	{
		Transform* currentTrans = game.currentState->entityManager.transformPool.get(entityId);
		Transform* nextTrans = game.nextState->entityManager.transformPool.get(entityId);
		Camera* currentCamera = game.currentState->entityManager.cameraPool.get(entityId);
		Camera* nextCamera = game.nextState->entityManager.cameraPool.get(entityId);
		DebugCamera* next = game.nextState->entityManager.debugCameraPool.get(entityId);

		//Rotation
		float mouseXDelta = (float)input.getMouseXDelta();
		float mouseYDelta = (float)input.getMouseYDelta();

		yaw += mouseXDelta * -mouseSpeed;
		pitch += mouseYDelta * -mouseSpeed;
		yaw = glm::mod(yaw, 360.0f);
		pitch = glm::clamp(pitch, -89.0f, 89.0f);
		
		next->yaw = yaw;
		next->pitch = pitch;
		nextTrans->setRotation(glm::vec3(glm::radians(pitch), glm::radians(yaw), 0.0f));

		//Translation
		glm::vec3 currentPosition = currentTrans->getPosition();
		glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);

		if (input.keyHeld(SDLK_d))
		{
			translate += currentCamera->getSide();
		}

		if (input.keyHeld(SDLK_w))
		{
			translate += currentCamera->getForward();
		}

		if (input.keyHeld(SDLK_a))
		{
			translate -= currentCamera->getSide();
		}

		if (input.keyHeld(SDLK_s))
		{
			translate -= currentCamera->getForward();
		}

		if (translate != glm::vec3(0.0f, 0.0f, 0.0f))
		{
			translate = moveSpeed * glm::normalize(translate);			
		}

		nextTrans->setPosition(currentPosition + translate);
		nextCamera->calculateViewMatrix();
	}
}