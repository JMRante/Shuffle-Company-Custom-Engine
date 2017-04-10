/*

    SHUFFLE COMPANY SOURCE CODE

    sc_prefabs.cpp
    ------------------------------------



*/

#include "sc_prefabs.h"

namespace sc
{
	PrefabFactory::PrefabFactory(State* targetState)
	{
		this->targetState = targetState;
	}

	ID PrefabFactory::createStage(std::string filepath)
	{
		ID id = ID("E_STAGE");
		targetState->stage.loadStage(filepath);
		targetState->addEntity(id);
		targetState->transformPool.add(id, Transform());
		targetState->drawModelPool.add(id, DrawModel(ID("MO_STAGE"), true));

		return id;
	}

	ID PrefabFactory::createDebugCamera(ID id, glm::vec3 position, glm::vec3 rotation, float near, float far, float moveSpeed, float turnSpeed)
	{
		targetState->addEntity(id);

		Transform* tran = targetState->transformPool.add(id, Transform());
		tran->position = position;
		tran->rotation = rotation;
		tran->calculateWorldMatrix();

		Camera* camera = targetState->cameraPool.add(id, Camera(near, far));
		camera->calculateViewMatrix();

		targetState->debugCameraPool.add(id, DebugCamera(moveSpeed, turnSpeed));

		return id;
	}

	ID PrefabFactory::createEditorCamera(ID id, glm::vec3 position, float pitch, float near, float far, float keyMoveSpeed, float mouseMoveSpeed)
	{
		targetState->addEntity(id);

		Transform* tran = targetState->transformPool.add(id, Transform());
		tran->position = position;
		tran->rotation = glm::vec3(glm::radians(pitch), glm::radians(180.0f), 0.0f);
		tran->calculateWorldMatrix();

		Camera* camera = targetState->cameraPool.add(id, Camera(near, far));
		camera->calculateViewMatrix();

		targetState->editorCameraPool.add(id, EditorCamera(keyMoveSpeed, mouseMoveSpeed));

		return id;
	}

	ID PrefabFactory::createFramerateCounter(ID id, glm::vec2 position, glm::vec4 color, ID fontId)
	{
		targetState->addEntity(id);
		targetState->transformPool.add(id, Transform());
		targetState->drawTextPool.add(id, DrawText(position.x, position.y, "", color, fontId));
		targetState->framerateCounterPool.add(id, FramerateCounter());

		return id;
	}

	ID PrefabFactory::createUIRectangle(ID id, glm::vec2 position, glm::vec2 size, glm::vec2 pivot, glm::vec4 color)
	{
		targetState->addEntity(id);
		targetState->transformPool.add(id, Transform());
		DrawRectangle* dr = targetState->drawRectanglePool.add(id, DrawRectangle(position.x, position.y, size.x, size.y, pivot.x, pivot.y, color, true));
		dr->calculateTransform();

		return id;
	}

	ID PrefabFactory::createUISprite(ID id, glm::vec2 position, glm::vec2 scale, glm::vec2 pivot, ID spriteId)
	{
		targetState->addEntity(id);
		targetState->transformPool.add(id, Transform());
		DrawSprite* ds = targetState->drawSpritePool.add(id, DrawSprite(position.x, position.y, scale.x, scale.y, pivot.x, pivot.y, spriteId, true));
		ds->calculateTransform();

		return id;
	}

	ID PrefabFactory::createUIText(ID id, glm::vec2 position, std::string text, glm::vec4 color, ID fontId, TextHAlign ha, TextVAlign va, TextHAlign just)
	{
		targetState->addEntity(id);
		targetState->transformPool.add(id, Transform());
		DrawText* dt = targetState->drawTextPool.add(id, DrawText(position.x, position.y, text, color, fontId));
		dt->hAlignment = ha;
		dt->vAlignment = va;
		dt->justification = just;

		return id;
	}

	ID PrefabFactory::createCursor()
	{
		ID id = ID("E_CURSOR");

		if (!targetState->entityExists(id))
		{
			targetState->addEntity(id);
			targetState->transformPool.add(id, Transform());
			DrawSprite* ds = targetState->drawSpritePool.add(id, DrawSprite(input.getMouseX(), input.getMouseY(), 1.0, 1.0, 0.0f, 32.0f, ID("SP_POINTCUR"), true));
			ds->calculateTransform();
			targetState->cursorPool.add(id, Cursor());			
		}
		else
		{
			LOG_E << "Cursor already created";
		}

		return id;
	}
}