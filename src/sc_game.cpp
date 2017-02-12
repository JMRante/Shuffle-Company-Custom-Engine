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


		//Load Assets
		sc::Vertex tempVert;
		std::vector<sc::Vertex> vecVert;

		tempVert.position = glm::vec3(0.5f, 0.5f, 0.0f);
		tempVert.normal = glm::vec3(0.0f, 0.0f, -1.0f);
		tempVert.textureCoord = glm::vec2(0.0f, 0.0f);
		vecVert.push_back(tempVert);

		tempVert.position = glm::vec3(0.5f, -0.5f, 0.0f);
		vecVert.push_back(tempVert);

		tempVert.position = glm::vec3(-0.5f, -0.5f, 0.0f);
		vecVert.push_back(tempVert);

		tempVert.position = glm::vec3(-0.5f, 0.5f, 0.0f);
		vecVert.push_back(tempVert);

		static const int ind[] = {0, 1, 3, 1, 2, 3};
		std::vector<int> vecInd(ind, ind + sizeof(ind) / sizeof(ind[0]));

		sc::assets.loadMesh("ME_SQUARE", &vecVert, &vecInd);		
		sc::assets.loadMesh("ME_SPHERE", "Resources/Meshes/ME_SPHERE.obj");

		sc::assets.loadShader("SH_PASS", "Resources/Shaders/sc_shader_testVertex.glsl", "Resources/Shaders/sc_shader_testFragment.glsl");

		std::vector<glm::vec4> tempVec4;
		tempVec4.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		sc::assets.loadMaterial("MA_RED", NULL, NULL, &tempVec4, NULL, "SH_PASS");
		tempVec4.clear();
		tempVec4.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		sc::assets.loadMaterial("MA_BLUE", NULL, NULL, &tempVec4, NULL, "SH_PASS");

		sc::assets.loadModel("MO_TESTA", "ME_SQUARE", "MA_RED");
		sc::assets.loadModel("MO_TESTB", "ME_SPHERE", "MA_BLUE");


		//Build elements
		sc::EntityManager* em = &nextState->entityManager;

		em->addEntity("E_CAMERA");
		em->getTransform("E_CAMERA")->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		em->getTransform("E_CAMERA")->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		sc::Camera camera(em->getTransform("E_CAMERA"), 0.01f, 100.0f);
		em->addCamera("E_CAMERA", camera);
		sc::DebugCamera* cameraBehave = new sc::DebugCamera(0.07f, 0.1f);
		em->addBehavior("E_CAMERA", cameraBehave);

		em->addEntity("E_TESTA");
		sc::Transform* transformA = em->getTransform("E_TESTA");
		transformA->setPosition(glm::vec3(0.0f, 0.0f, -4.0f));
		transformA->setRotation(glm::vec3(0.0f, glm::radians(45.0f), 0.0f));
		sc::Draw drawA("MO_TESTA", true);
		em->addDraw("E_TESTA", drawA);

		em->addEntity("E_TESTB");
		sc::Transform* transformB = em->getTransform("E_TESTB");
		transformB->setPosition(glm::vec3(-1.0f, 0.0f, -2.0f));
		transformB->setScale(glm::vec3(0.25f, 0.25f, 0.25f));
		sc::Draw drawB("MO_TESTB", true);
		em->addDraw("E_TESTB", drawB);

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