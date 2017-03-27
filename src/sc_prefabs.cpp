/*

    SHUFFLE COMPANY SOURCE CODE

    sc_prefabs.cpp
    ------------------------------------



*/

#include "sc_prefabs.h"

namespace sc
{
	ID createStage(std::string filepath)
	{
		ID id = ID("E_STAGE");
		game.nextState->stage.loadStage(filepath);
		game.nextState->addEntity(id);
		game.nextState->transformPool.add(id, Transform());
		game.nextState->drawModelPool.add(id, DrawModel(ID("MO_STAGE"), true));

		return id;
	}

	ID createDebugCamera(ID id, glm::vec3 position, glm::vec3 rotation, float near, float far, float moveSpeed, float turnSpeed)
	{
		game.nextState->addEntity(id);

		Transform* tran = game.nextState->transformPool.add(id, Transform());
		tran->position = position;
		tran->rotation = rotation;
		tran->calculateWorldMatrix();

		Camera* camera = game.nextState->cameraPool.add(id, Camera(near, far));
		camera->calculateViewMatrix();

		game.nextState->debugCameraPool.add(id, DebugCamera(moveSpeed, turnSpeed));

		return id;
	}

	ID createEditorCamera(ID id, glm::vec3 position, float pitch, float near, float far, float keyMoveSpeed, float mouseMoveSpeed)
	{
		game.nextState->addEntity(id);

		Transform* tran = game.nextState->transformPool.add(id, Transform());
		tran->position = position;
		tran->rotation = glm::vec3(glm::radians(pitch), glm::radians(180.0f), 0.0f);
		tran->calculateWorldMatrix();

		Camera* camera = game.nextState->cameraPool.add(id, Camera(near, far));
		camera->calculateViewMatrix();

		game.nextState->editorCameraPool.add(id, EditorCamera(keyMoveSpeed, mouseMoveSpeed));

		return id;
	}

	ID createFramerateCounter(ID id, glm::vec2 position, glm::vec4 color, ID fontId)
	{
		game.nextState->addEntity(id);
		game.nextState->transformPool.add(id, Transform());
		game.nextState->drawTextPool.add(id, DrawText(position.x, position.y, "", color, fontId, true));
		game.nextState->framerateCounterPool.add(id, FramerateCounter());

		return id;
	}

	ID createUIRectangle(ID id, glm::vec2 position, glm::vec2 size, glm::vec2 pivot, glm::vec4 color)
	{
		game.nextState->addEntity(id);
		game.nextState->transformPool.add(id, Transform());
		DrawRectangle* dr = game.nextState->drawRectanglePool.add(id, DrawRectangle(position.x, position.y, size.x, size.y, pivot.x, pivot.y, color, true));
		dr->calculateTransform();

		return id;
	}

	ID createUISprite(ID id, glm::vec2 position, glm::vec2 scale, glm::vec2 pivot, ID spriteId)
	{
		game.nextState->addEntity(id);
		game.nextState->transformPool.add(id, Transform());
		DrawSprite* ds = game.nextState->drawSpritePool.add(id, DrawSprite(position.x, position.y, scale.x, scale.y, pivot.x, pivot.y, spriteId, true));
		ds->calculateTransform();

		return id;
	}

	ID createUIText(ID id, glm::vec2 position, std::string text, ID fontId, glm::vec4 color)
	{
		game.nextState->addEntity(id);
		game.nextState->transformPool.add(id, Transform());
		game.nextState->drawTextPool.add(id, DrawText(position.x, position.y, text, color, fontId, true));

		return id;
	}

	ID createCursor()
	{
		ID id = ID("E_CURSOR");
		game.nextState->addEntity(id);
		game.nextState->transformPool.add(id, Transform());
		DrawSprite* ds = game.nextState->drawSpritePool.add(id, DrawSprite(input.getMouseX(), input.getMouseY(), 1.0, 1.0, 0.0f, 32.0f, ID("SP_POINTCUR"), true));
		ds->calculateTransform();
		game.nextState->cursorPool.add(id, Cursor());

		return id;
	}
}