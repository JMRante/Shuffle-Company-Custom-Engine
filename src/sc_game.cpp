/*

    SHUFFLE COMPANY SOURCE CODE

    sc_game.cpp
    ------------------------------------



*/

#include "sc_game.h"
#include "sc_world.h"

namespace sc
{
	Game game;

	void Game::start()
	{
		currentState = new sc::World();
		nextState = new sc::World();

		assets.loadDefaults();

		//Build elements
		EntityManager* em = &nextState->entityManager;

		nextState->stage.loadStage("Custom/Levels/TestLevel.shuff");
		em->addEntity(ID("E_STAGE"));
		Transform* tran = em->transformPool.add(ID("E_STAGE"), Transform());
		em->drawModelPool.add(ID("E_STAGE"), DrawModel(ID("MO_STAGE"), true));

		em->addEntity(ID("E_CAMERA"));
		tran = em->transformPool.add(ID("E_CAMERA"), Transform());
		tran->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		tran->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		Camera* camera = em->cameraPool.add(ID("E_CAMERA"), Camera(0.01f, 100.0f));
		camera->calculateViewMatrix();
		em->debugCameraPool.add(ID("E_CAMERA"), DebugCamera(0.07f, 0.1f));

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

		updateWorldState();
	}

	bool Game::update()
	{
		input.update();

		EntityManager* em = &currentState->entityManager;

		for (auto it = em->debugCameraPool.begin(); it != em->debugCameraPool.end(); it++) { (*it).update(); }

		if (input.quitEvent())
		{
			return true;
		}

		updateWorldState();
		return false;
	}

	void Game::updateWorldState()
	{
		LOG_D << "Updating World State";

		currentState->copy(nextState);
	}
}