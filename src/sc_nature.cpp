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
		
		trans->setRot(glm::vec3(glm::radians(pitch), glm::radians(yaw), 0.0f));

		//Translation
		glm::vec3 currentPosition = trans->getPos();
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
			trans->setPos(currentPosition + translate);
			camera->calculateViewMatrix();	
		}
	}


	/*
		EditorCamera
						*/
	EditorCamera::EditorCamera(float keyMoveSpeed, float mouseMoveSpeed) : Nature()
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

		glm::vec3 currentPosition = trans->getPos();
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
				stage->setMouseSelectLayer(cameraLayer);
				stage->updateStageMesh();

				for (size_t i = 0; i < editSlotTransforms.size(); i++)
				{
					editSlotTransforms[i]->setPos(editSlotTransforms[i]->getPos() + glm::vec3(0.0f, 1.0f, 0.0f));
				}
			}
			else if (input.getMouseWheelDelta() < 0 && cameraLayer > 0)
			{
				translate -= glm::vec3(0.0f, 1.0f, 0.0f);
				cameraLayer--;
				stage->setMouseSelectLayer(cameraLayer);
				stage->updateStageMesh();

				for (size_t i = 0; i < editSlotTransforms.size(); i++)
				{
					editSlotTransforms[i]->setPos(editSlotTransforms[i]->getPos() - glm::vec3(0.0f, 1.0f, 0.0f));
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
				stage->setMouseSelectLayer(cameraLayer);
				stage->updateStageMesh();

				for (size_t i = 0; i < editSlotTransforms.size(); i++)
				{
					editSlotTransforms[i]->setPos(editSlotTransforms[i]->getPos() + glm::vec3(0.0f, 1.0f, 0.0f));
				}
			}
			else if (input.keyPressed(SDLK_a) && cameraLayer > 0)
			{
				translate -= glm::vec3(0.0f, 1.0f, 0.0f);
				cameraLayer--;
				stage->setMouseSelectLayer(cameraLayer);
				stage->updateStageMesh();

				for (size_t i = 0; i < editSlotTransforms.size(); i++)
				{
					editSlotTransforms[i]->setPos(editSlotTransforms[i]->getPos() - glm::vec3(0.0f, 1.0f, 0.0f));
				}
			}
		}

		trans->setPos(currentPosition + translate);
		camera->calculateViewMatrix();
	}

	int EditorCamera::getCameraLayer()
	{
		return cameraLayer;
	}


	/*
		EditorSlot
				*/
	EditorSlot::EditorSlot(int x, int z) : Nature()
	{
		this->x = x;
		this->z = z;
	}

	void EditorSlot::update()
	{
		DrawModel* dm = state->getComponent<DrawModel>(entityId);
		Stage* stage = state->getComponent<Stage>(ID("E_STAGE"));

		if (entityId.is(input.mouseSelectedEntity))
		{
			EditorCamera* ec = state->getComponent<EditorCamera>(ID("E_CAMERA"));
			EditorOperationManager* eom = state->getComponent<EditorOperationManager>(ID("E_EDITOR"));

			dm->model = assets.modelStack.get(ID("MO_EDITSLOTB"));

			if (input.mouseButtonHeld(SDL_BUTTON_LEFT) && stage->get(x, ec->getCameraLayer(), z) != 1)
			{
				eom->doOperation(new SetBrush(1, glm::ivec3(x, ec->getCameraLayer(), z)));
			}
		}
		else
		{
			dm->model = assets.modelStack.get(ID("MO_EDITSLOTA"));
		}
	}


	/*
		EditorControl
						*/
	EditorControl::EditorControl() : Nature() {}

	void EditorControl::update()
	{
		EditorOperationManager* eom = state->getComponent<EditorOperationManager>(ID("E_EDITOR"));

		if (input.keyPressed(SDLK_r))
		{
			eom->redoOperation();
		}
		else if (input.keyPressed(SDLK_u))
		{
			eom->undoOperation();
		}

		std::vector<Transform*> trans = state->getComponentFromTagged<Transform>(ID("T_MAINEDIT"));
		if (input.keyPressed(SDLK_g))
		{
			for (auto it = trans.begin(); it != trans.end(); it++)
			{
				(*it)->setActive(false);
			}
		}
		else if (input.keyPressed(SDLK_h))
		{
			for (auto it = trans.begin(); it != trans.end(); it++)
			{
				(*it)->setActive(true);
			}
		}

		if (ID("STAGESELECTED").is(input.mouseSelectedEntity))
		{
			Stage* stage = state->getComponent<Stage>(ID("E_STAGE"));
			glm::ivec3 coord = stage->getSelectedBlock();

			if (input.mouseButtonHeld(SDL_BUTTON_RIGHT) && coord.x != -1 && stage->get(coord.x, coord.y, coord.z) != 0)
			{
				eom->doOperation(new SetBrush(0, coord));
				stage->setMouseSelected(-1);
			}
		}
	}

	/*
		Cursor
				*/
	Cursor::Cursor() : Nature()
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
		Transform* tran = state->getComponent<Transform>(entityId);

		int mouseX = input.getMouseX();
		int mouseY = input.getMouseY();

		tran->setPosX(mouseX);
		tran->setPosY(mouseY);

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
	FramerateCounter::FramerateCounter() : Nature()
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

	Button::Button(Event* event) : Nature()
	{
		highlightSpeed = 4.0f;
		highlightPosition = 0.0f;
		normalColor = COL_EDITOR_UI_BUTTON;
		highlightColor = COL_EDTIOR_UI_BUTTONEDGE;
		this->event = event;
	}

	Button::~Button()
	{
		delete event;
	}

	void Button::update()
	{
		bool isMouseHere = entityId.is(input.mouseSelectedEntity);
		DrawRectangle* fore = state->getComponent<DrawRectangle>(ID(entityId.getStr() + "FORE"));

		if (fore != NULL)
		{
			if (isMouseHere)
			{
				if (highlightPosition <= 1.0f)
				{
					highlightPosition += highlightSpeed * getDeltaSec();
				}
				
				if (highlightPosition > 1.0f)
				{
					highlightPosition = 1.0f;
				}
			}
			else
			{
				if (highlightPosition >= 0.0f)
				{
					highlightPosition -= highlightSpeed * getDeltaSec();
				}
				
				if (highlightPosition < 0.0f)
				{
					highlightPosition = 0.0f;
				}
			}

			if (isMouseHere && input.mouseButtonHeld(SDL_BUTTON_LEFT))
			{
				fore->color = COL_EDITOR_UI_BUTTONCLICK;
			}
			else
			{
				fore->color = glm::vec4(lerp(normalColor.x, highlightColor.x, highlightPosition), lerp(normalColor.y, highlightColor.y, highlightPosition), lerp(normalColor.z, highlightColor.z, highlightPosition), 1.0f);
			}
		}

		if (isMouseHere && input.mouseButtonPressed(SDL_BUTTON_LEFT))
		{
			event->happen();
		}
	}

	FileSelectorItem::FileSelectorItem()
	{
		fileName = "";
	}

	void FileSelectorItem::update()
	{

	}
}