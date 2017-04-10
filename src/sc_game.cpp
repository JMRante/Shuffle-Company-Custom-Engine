/*

    SHUFFLE COMPANY SOURCE CODE

    sc_game.cpp
    ------------------------------------



*/

#include "sc_game.h"
#include "sc_state.h"

namespace sc
{
	void Game::start()
	{
		currentState = new sc::State();
		nextState = new sc::State();

		assets.loadDefaults();

		PrefabFactory pf(nextState);

		//Build elements
		pf.createStage("Custom/Levels/TestLevel.shuff");

		//pf.createDebugCamera(ID("E_CAMERA"), glm::vec3(1.5f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.1f, 100.0f, 0.07f, 0.1f);
		pf.createEditorCamera(ID("E_CAMERA"), glm::vec3(0.0f, 12.0f, 10.0f), -60.0f, 0.1f, 100.0f, 0.1f, 0.01f);
		pf.createFramerateCounter(ID("E_FRAMECOUNT"), glm::vec2(16, sc::config.get("WINDOW_HEIGHT") - 32), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), ID("FT_MONO"));
		ID cursorID = pf.createCursor();
		nextState->drawSpritePool.get(cursorID)->setLayer(100);

		pf.createUISprite(ID("E_SPRITE"), glm::vec2(256, 16), glm::vec2(1.0f, 1.0f), glm::vec2(0, 0), ID("SP_TEST"));
		pf.createUIRectangle(ID("E_RECTB"), glm::vec2(sc::config.get("WINDOW_WIDTH") / 2, sc::config.get("WINDOW_HEIGHT") / 2), glm::vec2(640, 256), glm::vec2(320, 128), glm::vec4(0.0f, 0.0f, 5.0f, 1.0f));
		nextState->drawRectanglePool.get(ID("E_RECTB"))->setLayer(3);
		pf.createUIText(ID("E_TEXT"), glm::vec2(sc::config.get("WINDOW_WIDTH") / 2, sc::config.get("WINDOW_HEIGHT") / 2), "There are things\nbeyond Hello World?\nDunno.\nBut multiple lines of varying size now fit well.\nHahaha!", glm::vec4(1.0, 1.0, 1.0f, 1.0), ID("FT_TEST"), TextHAlign::center, TextVAlign::middle, TextHAlign::center);
		nextState->drawTextPool.get(ID("E_TEXT"))->setLayer(4);

		swapState();
	}

	bool Game::update()
	{
		input.update();

		for (auto it = currentState->naturePointers.begin(); it != currentState->naturePointers.end(); it++) 
		{ 
			if ((*it)->isActive) 
			{
				(*it)->update(currentState, nextState); 
			}
		}

		if (input.quitEvent())
		{
			return true;
		}

		swapState();
		return false;
	}

	void Game::swapState()
	{
		LOG_D << "Swapping game state";

		currentState->copy(nextState);
	}
}