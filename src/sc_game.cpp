/*

    SHUFFLE COMPANY SOURCE CODE

    sc_game.cpp
    ------------------------------------



*/

#include "sc_game.h"

namespace sc
{
	void Game::start()
	{
		currentState = new sc::World();
		nextState = new sc::World();

		assets.loadDefaults();

		//Build elements
		sc::EntityManager* em = &nextState->entityManager;

		currentState->stage.loadStage("Custom/Levels/TestLevel.shuff");
		em->addEntity("E_STAGE");
		sc::Draw drawStage("MO_STAGE", true);
		em->addDraw("E_STAGE", drawStage);

		em->addEntity("E_CAMERA");
		em->getTransform("E_CAMERA")->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		em->getTransform("E_CAMERA")->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		sc::Camera camera(em->getTransform("E_CAMERA"), 0.01f, 100.0f);
		em->addCamera("E_CAMERA", camera);
		sc::DebugCamera* cameraBehave = new sc::DebugCamera(0.07f, 0.1f);
		em->addBehavior("E_CAMERA", cameraBehave);

		// em->addEntity("E_TESTA");
		// sc::Transform* transformA = em->getTransform("E_TESTA");
		// transformA->setPosition(glm::vec3(0.0f, 0.0f, -4.0f));
		// transformA->setRotation(glm::vec3(0.0f, glm::radians(45.0f), 0.0f));
		// sc::Draw drawA("MO_TESTB", true);
		// em->addDraw("E_TESTA", drawA);

		// em->addEntity("E_TESTB");
		// sc::Transform* transformB = em->getTransform("E_TESTB");
		// transformB->setPosition(glm::vec3(-1.0f, 0.0f, -2.0f));
		// transformB->setScale(glm::vec3(0.25f, 0.25f, 0.25f));
		// sc::Draw drawB("MO_TESTB", true);
		// em->addDraw("E_TESTB", drawB);

		updateWorldState();
	}

	bool Game::update()
	{
		input.update();

		for (auto behaveIt = currentState->entityManager.getBehaviorPoolBegin(); behaveIt != currentState->entityManager.getBehaviorPoolEnd(); behaveIt++)
		{
			(*behaveIt)->update(currentState, nextState);
		}

		if (input.quitEvent())
		{
			return true;
		}

		return false;
	}

	sc::World* Game::updateWorldState()
	{
		LOG_D << "Updating World State";

		currentState->copy(nextState);
		return currentState;
	}
}