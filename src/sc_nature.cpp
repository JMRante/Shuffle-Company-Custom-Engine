/*

    SHUFFLE COMPANY SOURCE CODE

    sc_nature.cpp
    ------------------------------------



*/

#include "sc_nature.h"
#include "sc_game.h"
#include "sc_state.h"

namespace sc
{
	Nature::Nature() : sc::Component() 
	{
		isActive = true;
	}


	/*
		DebugCamera
					*/
	DebugCamera::DebugCamera(float moveSpeed, float mouseSpeed) : Nature()
	{
		this->moveSpeed = moveSpeed;
		this->mouseSpeed = mouseSpeed;
		yaw = 180.0f;
		pitch = 0.0f;
	}

	void DebugCamera::update()
	{
		Transform* currentTrans = game.currentState->transformPool.get(entityId);
		Transform* nextTrans = game.nextState->transformPool.get(entityId);
		Camera* currentCamera = game.currentState->cameraPool.get(entityId);
		Camera* nextCamera = game.nextState->cameraPool.get(entityId);
		DebugCamera* next = game.nextState->debugCameraPool.get(entityId);

		//Rotation
		float mouseXDelta = (float)input.getMouseXDelta();
		float mouseYDelta = (float)input.getMouseYDelta();

		yaw += mouseXDelta * -mouseSpeed;
		pitch += mouseYDelta * -mouseSpeed;
		yaw = glm::mod(yaw, 360.0f);
		pitch = glm::clamp(pitch, -89.0f, 89.0f);
		
		next->yaw = yaw;
		next->pitch = pitch;
		nextTrans->rotation = glm::vec3(glm::radians(pitch), glm::radians(yaw), 0.0f);

		//Translation
		glm::vec3 currentPosition = currentTrans->position;
		glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);

		if (input.keyHeld(SDLK_w))
		{
			translate += currentCamera->getForward();
		}

		if (input.keyHeld(SDLK_d))
		{
			translate += currentCamera->getSide();
		}

		if (input.keyHeld(SDLK_s))
		{
			translate -= currentCamera->getForward();
		}

		if (input.keyHeld(SDLK_a))
		{
			translate -= currentCamera->getSide();
		}

		if (translate != glm::vec3(0.0f, 0.0f, 0.0f))
		{
			translate = moveSpeed * glm::normalize(translate);
			nextTrans->position = currentPosition + translate;
			nextCamera->calculateViewMatrix();	
		}
	}


	/*
		EditorCamera
						*/
	EditorCamera::EditorCamera(float keyMoveSpeed, float mouseMoveSpeed)
	{
		this->keyMoveSpeed = keyMoveSpeed;
		this->mouseMoveSpeed = mouseMoveSpeed;
	}

	void EditorCamera::update()
	{
		Transform* currentTrans = game.currentState->transformPool.get(entityId);
		Transform* nextTrans = game.nextState->transformPool.get(entityId);
		Camera* nextCamera = game.nextState->cameraPool.get(entityId);

		glm::vec3 currentPosition = currentTrans->position;
		glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);

		//Mouse Control
		if (input.mouseButtonHeld(SDL_BUTTON_MIDDLE))
		{
			float mouseXDelta = (float)input.getMouseXDelta();
			float mouseYDelta = (float)input.getMouseYDelta();

			translate = mouseMoveSpeed * glm::vec3(mouseXDelta, 0.0f, mouseYDelta);
		}
		else
		{
			//Keyboard Control
			if (input.keyHeld(SDLK_UP))
			{
				translate -= glm::vec3(0.0f, 0.0f, 1.0f);
			}

			if (input.keyHeld(SDLK_RIGHT))
			{
				translate += glm::vec3(1.0f, 0.0f, 0.0f);
			}

			if (input.keyHeld(SDLK_DOWN))
			{
				translate += glm::vec3(0.0f, 0.0f, 1.0f);
			}

			if (input.keyHeld(SDLK_LEFT))
			{
				translate -= glm::vec3(1.0f, 0.0f, 0.0f);
			}

			if (translate != glm::vec3(0.0f, 0.0f, 0.0f))
			{
				translate = keyMoveSpeed * glm::normalize(translate);
			}
		}

		nextTrans->position = currentPosition + translate;
		nextCamera->calculateViewMatrix();
	}


	/*
		Cursor
				*/
	Cursor::Cursor()
	{
		state = CursorState::point;

		pointSprite = assets.getSprite(ID("SP_POINTCUR"));
		hoverSprite = assets.getSprite(ID("SP_HOVERCUR"));
		clickSprite = assets.getSprite(ID("SP_CLICKCUR"));
		dragSprite = assets.getSprite(ID("SP_DRAGCUR"));
	}

	void Cursor::update()
	{
		DrawSprite* nextSprite = game.nextState->drawSpritePool.get(entityId);

		int mouseX = input.getMouseX();
		int mouseY = input.getMouseY();

		nextSprite->x = mouseX;
		nextSprite->y = mouseY;
		nextSprite->calculateTransform();

		if (input.mouseButtonHeld(SDL_BUTTON_MIDDLE))
		{
			state = CursorState::drag;
		}
		else
		{
			state = CursorState::point;
		}

		switch (state)
		{
		case CursorState::point:
			nextSprite->sprite = pointSprite;
			break;
		case CursorState::hover:
			nextSprite->sprite = hoverSprite;
			break;	
		case CursorState::click:
			nextSprite->sprite = clickSprite;
			break;
		case CursorState::drag:
			nextSprite->sprite = dragSprite;
			break;
		}
	}


	/*
		FramerateCounter
						*/
	FramerateCounter::FramerateCounter()
	{
		framerateHistoryCount = -1;
		framerateAverage = 0;
	}

	void FramerateCounter::update()
	{
		//Calculate
		float currentFramerate;

		if (delay > MS_PER_FRAME)
		{
			//Framerate is under cap
			currentFramerate = 1000.0f / ((float) delay);
		}
		else
		{
			//Framerate is above cap
			currentFramerate = 60.0f;
		}

		if (framerateHistoryCount < 60)
		{
			framerateHistoryCount++;
			framerateHistory[framerateHistoryCount] = currentFramerate;
		}
		else
		{
			for (int i = 0; i < 60; i++)
			{
				if (i == 59)
				{
					framerateHistory[i] = currentFramerate;
				}
				else
				{
					framerateHistory[i] = framerateHistory[i + 1];
				}
			}
		}

		float sum = 0;

		for (int i = 0; i < 60; i++)
		{
			sum += framerateHistory[i];
		}

		framerateAverage = sum / 60.0f;

		//Update draw
		DrawText* dt = game.nextState->drawTextPool.get(entityId);

		if (dt == NULL)
		{
			LOG_D << framerateAverage;			
		}
		else
		{
			dt->text = fToS(framerateAverage);
		}
	}
}