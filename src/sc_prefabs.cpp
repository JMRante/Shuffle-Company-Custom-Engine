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
		game.nextState->stage.loadStage(filepath);
		game.nextState->addEntity(ID("E_STAGE"));
		game.nextState->transformPool.add(ID("E_STAGE"), Transform());
		game.nextState->drawModelPool.add(ID("E_STAGE"), DrawModel(ID("MO_STAGE"), true));

		return ID("E_STAGE");
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
}