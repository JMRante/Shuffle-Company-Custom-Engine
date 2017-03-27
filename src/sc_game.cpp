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
		createStage("Custom/Levels/TestLevel.shuff");

		//createDebugCamera(ID("E_CAMERA"), glm::vec3(1.5f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.1f, 100.0f, 0.07f, 0.1f);
		createEditorCamera(ID("E_CAMERA"), glm::vec3(0.0f, 12.0f, 10.0f), -60.0f, 0.1f, 100.0f, 0.1f, 0.01f);
		createFramerateCounter(ID("E_FRAMECOUNT"), glm::vec2(16, sc::config.get("WINDOW_HEIGHT") - 16 - 16), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), ID("FT_MONO"));
		createCursor();

		// createUIRectangle(ID("E_RECTA"), glm::vec2(32, 640), glm::vec2(128, 128), glm::vec2(0, 0), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		// createUIRectangle(ID("E_RECTB"), glm::vec2(0, 0), glm::vec2(128, 128), glm::vec2(64, 64), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

		// createUISprite(ID("E_SPRITE"), glm::vec2(256, 16), glm::vec2(1.0f, 1.0f), glm::vec2(0, 0), ID("SP_TEST"));
		// createUIText(ID("E_TEXT"), glm::vec2(512, 256), "There are things beyond Hello World?", ID("FT_TEST"), glm::vec4(1.0, 1.0, 1.0f, 1.0));

		updateState();
	}

	bool Game::update()
	{
		input.update();

		for (auto it = NaturePoolBase::begin(); it != NaturePoolBase::end(); it++) 
		{ 
			if ((*it)->isActive) 
			{
				(*it)->update(); 
			}
		}

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