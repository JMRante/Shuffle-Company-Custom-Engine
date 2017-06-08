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
	Nature::Nature() : Component() 
	{
		addType(ID("NATURE"));
		isActive = true;
	}

	void Nature::onStateInsert()
	{
		state->naturePointers.push_back(this);
	}

	void Nature::onStateRemove()
	{
		for (auto ni = state->naturePointers.begin(); ni != state->naturePointers.end(); ni++)
		{
			if ((*ni)->entityId.is(entityId) && (*ni)->sameTypes((Component*) this))
			{
				state->naturePointers.erase(ni);
			}
		}
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
		Transform* trans = state->getComponent<Transform>(entityId);
		Camera* camera = state->getComponent<Camera>(entityId);

		//Rotation
		float mouseXDelta = (float)input.getMouseXDelta();
		float mouseYDelta = (float)input.getMouseYDelta();

		yaw += mouseXDelta * -mouseSpeed * getDeltaSec();
		pitch += mouseYDelta * -mouseSpeed * getDeltaSec();
		yaw = glm::mod(yaw, 360.0f);
		pitch = glm::clamp(pitch, -89.0f, 89.0f);
		
		trans->rotation = glm::vec3(glm::radians(pitch), glm::radians(yaw), 0.0f);

		//Translation
		glm::vec3 currentPosition = trans->position;
		glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);

		if (input.keyHeld(SDLK_w))
		{
			translate += camera->getForward();
		}

		if (input.keyHeld(SDLK_d))
		{
			translate += camera->getSide();
		}

		if (input.keyHeld(SDLK_s))
		{
			translate -= camera->getForward();
		}

		if (input.keyHeld(SDLK_a))
		{
			translate -= camera->getSide();
		}

		if (translate != glm::vec3(0.0f, 0.0f, 0.0f))
		{
			translate = moveSpeed * glm::normalize(translate) * getDeltaSec();
			trans->position = currentPosition + translate;
			camera->calculateViewMatrix();	
		}
	}


	/*
		EditorCamera
						*/
	EditorCamera::EditorCamera(float keyMoveSpeed, float mouseMoveSpeed)
	{
		this->keyMoveSpeed = keyMoveSpeed;
		this->mouseMoveSpeed = mouseMoveSpeed;
		this->cameraLayer = 0;
	}

	void EditorCamera::update()
	{
		Transform* trans = state->getComponent<Transform>(entityId);
		Camera* camera = state->getComponent<Camera>(entityId);
		Stage* stage = state->getComponent<Stage>(ID("E_STAGE"));

		if (editSlotTransforms.size() == 0)
		{
			editSlotTransforms = state->getComponentFromTagged<Transform>(ID("T_EDITSLOT"));
		}

		glm::vec3 currentPosition = trans->position;
		glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);

		//Scroll horizontally
		//Mouse Control
		if (input.mouseButtonHeld(SDL_BUTTON_MIDDLE))
		{
			float mouseXDelta = (float)input.getMouseXDelta();
			float mouseYDelta = (float)input.getMouseYDelta();

			translate = mouseMoveSpeed * glm::vec3(mouseXDelta, 0.0f, mouseYDelta) * getDeltaSec();
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
				translate = keyMoveSpeed * glm::normalize(translate) * getDeltaSec();
			}
		}

		//Change layer vertically
		//Mouse Control
		int stageHeight = stage->getHeight();

		if (input.getMouseWheelDelta() != 0)
		{
			if (input.getMouseWheelDelta() > 0 && cameraLayer < (stageHeight - 1))
			{
				translate += glm::vec3(0.0f, 1.0f, 0.0f);
				cameraLayer++;

				for (size_t i = 0; i < editSlotTransforms.size(); i++)
				{
					editSlotTransforms[i]->position += glm::vec3(0.0f, 1.0f, 0.0f);
					editSlotTransforms[i]->calculateWorldMatrix();
				}
			}
			else if (input.getMouseWheelDelta() < 0 && cameraLayer > 0)
			{
				translate -= glm::vec3(0.0f, 1.0f, 0.0f);
				cameraLayer--;

				for (size_t i = 0; i < editSlotTransforms.size(); i++)
				{
					editSlotTransforms[i]->position -= glm::vec3(0.0f, 1.0f, 0.0f);
					editSlotTransforms[i]->calculateWorldMatrix();
				}
			}
		}
		else 
		{
			//Keyboard Control
			if (input.keyPressed(SDLK_q) && cameraLayer < (stageHeight - 1))
			{
				translate += glm::vec3(0.0f, 1.0f, 0.0f);
				cameraLayer++;

				for (size_t i = 0; i < editSlotTransforms.size(); i++)
				{
					editSlotTransforms[i]->position += glm::vec3(0.0f, 1.0f, 0.0f);
					editSlotTransforms[i]->calculateWorldMatrix();
				}
			}
			else if (input.keyPressed(SDLK_a) && cameraLayer > 0)
			{
				translate -= glm::vec3(0.0f, 1.0f, 0.0f);
				cameraLayer--;

				for (size_t i = 0; i < editSlotTransforms.size(); i++)
				{
					editSlotTransforms[i]->position -= glm::vec3(0.0f, 1.0f, 0.0f);
					editSlotTransforms[i]->calculateWorldMatrix();
				}
			}
		}

		trans->position = currentPosition + translate;
		camera->calculateViewMatrix();
	}

	int EditorCamera::getCameraLayer()
	{
		return cameraLayer;
	}


	/*
		EditorSlot
				*/
	EditorSlot::EditorSlot(int x, int z) 
	{
		this->x = x;
		this->z = z;
	}

	void EditorSlot::update()
	{
		Stage* stage = state->getComponent<Stage>(ID("E_STAGE"));
		DrawModel* dm = state->getComponent<DrawModel>(entityId);
		EditorCamera* ec = state->getComponent<EditorCamera>(ID("E_CAMERA"));

		if (entityId.is(input.mouseSelectedEntity))
		{
			dm->model = assets.modelStack.get(ID("MO_EDITSLOTB"));

			if (input.mouseButtonHeld(SDL_BUTTON_LEFT))
			{
				std::vector<glm::ivec3> singleSlot;
				singleSlot.push_back(glm::ivec3(x, ec->getCameraLayer(), z));
				stage->drawBrush(&singleSlot, 0);
				stage->updateStageMesh();
			}
		}
		else
		{
			dm->model = assets.modelStack.get(ID("MO_EDITSLOTA"));
		}
	}


	/*
		Cursor
				*/
	Cursor::Cursor()
	{
		cursorState = CursorState::point;

		pointSprite = assets.spriteStack.get(ID("SP_POINTCUR"));
		hoverSprite = assets.spriteStack.get(ID("SP_HOVERCUR"));
		clickSprite = assets.spriteStack.get(ID("SP_CLICKCUR"));
		dragSprite = assets.spriteStack.get(ID("SP_DRAGCUR"));
	}

	void Cursor::update()
	{
		DrawSprite* sprite = state->getComponent<DrawSprite>(entityId);

		int mouseX = input.getMouseX();
		int mouseY = input.getMouseY();

		sprite->x = mouseX;
		sprite->y = mouseY;
		sprite->calculateTransform();

		if (input.mouseButtonHeld(SDL_BUTTON_MIDDLE))
		{
			cursorState = CursorState::drag;
		}
		else
		{
			cursorState = CursorState::point;
		}

		if (state->entityHasTag(input.mouseSelectedEntity, ID("T_BUTTON")))
		{
			cursorState = CursorState::hover;
		}

		if (input.mouseButtonHeld(SDL_BUTTON_LEFT))
		{
			cursorState = CursorState::click;
		}

		switch (cursorState)
		{
		case CursorState::point:
			sprite->sprite = pointSprite;
			break;
		case CursorState::hover:
			sprite->sprite = hoverSprite;
			break;	
		case CursorState::click:
			sprite->sprite = clickSprite;
			break;
		case CursorState::drag:
			sprite->sprite = dragSprite;
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
		DrawText* dt = state->getComponent<DrawText>(entityId);

		if (dt == NULL)
		{
			LOG_D << framerateAverage;			
		}
		else
		{
			dt->setText(fToS(framerateAverage));
		}
	}
}