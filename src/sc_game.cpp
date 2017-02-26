/*

    SHUFFLE COMPANY SOURCE CODE

    sc_game.cpp
    ------------------------------------



*/

#include "sc_game.h"
#include "sc_state.h"

namespace sc
{
	Game game;

	void Game::start()
	{
		currentState = new sc::State();
		nextState = new sc::State();

		assets.loadDefaults();

		//Build elements
		nextState->stage.loadStage("Custom/Levels/TestLevel.shuff");
		nextState->addEntity(ID("E_STAGE"));
		Transform* tran = nextState->transformPool.add(ID("E_STAGE"), Transform());
		nextState->drawModelPool.add(ID("E_STAGE"), DrawModel(ID("MO_STAGE"), true));

		nextState->addEntity(ID("E_CAMERA"));
		tran = nextState->transformPool.add(ID("E_CAMERA"), Transform());
		tran->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		tran->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		Camera* camera = nextState->cameraPool.add(ID("E_CAMERA"), Camera(0.01f, 100.0f));
		camera->calculateViewMatrix();
		nextState->debugCameraPool.add(ID("E_CAMERA"), DebugCamera(0.07f, 0.1f));

		nextState->addEntity(ID("E_RECT"));
		nextState->transformPool.add(ID("E_RECT"), Transform());
		DrawRectangle* dr = nextState->drawRectanglePool.add(ID("E_RECT"), DrawRectangle(16, 16, 128, 128, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), true));
		dr->calculateTransform();

		nextState->addEntity(ID("E_SPRITE"));
		nextState->transformPool.add(ID("E_SPRITE"), Transform());
		DrawSprite* ds = nextState->drawSpritePool.add(ID("E_SPRITE"), DrawSprite(256, 16, 1.0, 1.0, ID("SP_TEST"), true));
		ds->calculateTransform();

		nextState->addEntity(ID("E_TEXT"));
		nextState->transformPool.add(ID("E_TEXT"), Transform());
		nextState->drawTextPool.add(ID("E_TEXT"), DrawText(512, 256, "Hello World!", glm::vec4(1.0, 1.0, 1.0f, 1.0), ID("FT_TEST"), true));

		// em->addEntity(ID("E_TESTA"));
		// tran = em->transformPool.add(ID("E_TESTA"), Transform());
		// tran->setPosition(glm::vec3(0.0f, 0.0f, -4.0f));
		// tran->setRotation(glm::vec3(0.0f, glm::radians(45.0f), 0.0f));
		// DrawModel drawA(ID("MO_TESTA"), true);
		// em->drawModelPool.add(ID("E_TESTA"), drawA);

		// em->addEntity(ID("E_TESTB"));
		// tran = em->transformPool.add(ID("E_TESTB"), Transform());
		// tran->setPosition(glm::vec3(-1.0f, 0.0f, -2.0f));
		// tran->setScale(glm::vec3(0.25f, 0.25f, 0.25f));
		// DrawModel drawB(ID("MO_TESTB"), true);
		// em->drawModelPool.add(ID("E_TESTB"), drawB);

		updateState();
	}

	bool Game::update()
	{
		input.update();

		for (auto it = currentState->debugCameraPool.begin(); it != currentState->debugCameraPool.end(); it++) { (*it).update(); }

		if (input.quitEvent())
		{
			return true;
		}

		updateState();
		return false;
	}

	void Game::updateState()
	{
		LOG_D << "Updating World State";

		currentState->copy(nextState);
	}
}