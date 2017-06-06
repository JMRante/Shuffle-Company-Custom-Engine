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
		state = new sc::State();

		PrefabFactory pf(state);

		//Create Editor Scene
		//Build elements
		pf.createStage("Custom/Levels/TestLevel.shuff");

		//pf.createDebugCamera(ID("E_CAMERA"), glm::vec3(1.5f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.1f, 100.0f, 0.07f, 0.1f);
		pf.createEditorCamera(ID("E_CAMERA"), glm::vec3(0.0f, 12.0f, 10.0f), -60.0f, 0.1f, 100.0f, 0.1f, 0.01f);
		pf.createFramerateCounter(ID("E_FRAMECOUNT"), glm::vec2(16, sc::config.get("WINDOW_HEIGHT") - 32), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), ID("FT_MONO"));
		state->getComponent<DrawText>(ID("E_FRAMECOUNT"))->setLayer(0);
		ID cursorID = pf.createCursor();
		state->getComponent<DrawSprite>(cursorID)->setLayer(100);

		Stage* stage = state->getComponent<Stage>(ID("E_STAGE"));
		int stageWidth = stage->getWidth();
		int stageDepth = stage->getDepth();

		for (int i = 0; i < stageWidth; i++)
		{
			for (int j = 0; j < stageDepth; j++)
			{
				pf.createEditorSlot(ID(std::string("E_EDITSLOT" + iToS(i) + "_" + iToS(j)).c_str()), glm::vec3((float) i, 0.05f, (float) j), i, j);
			}
		}

		pf.createSpriteButton(ID("E_SAVEBUTTON"), glm::vec2(sc::config.get("WINDOW_WIDTH") - 96.0f, sc::config.get("WINDOW_HEIGHT") - 48.0f), glm::vec2(48.0f, 48.0f), ID("SP_SAVEICON"));
		pf.createSpriteButton(ID("E_LOADBUTTON"), glm::vec2(sc::config.get("WINDOW_WIDTH") - 48.0f, sc::config.get("WINDOW_HEIGHT") - 48.0f), glm::vec2(48.0f, 48.0f), ID("SP_LOADICON"));

		//pf.createUISprite(ID("E_SPRITE"), glm::vec2(256, 16), glm::vec2(1.0f, 1.0f), glm::vec2(0, 0), ID("SP_TEST"));
		//pf.createUIRectangle(ID("E_RECTB"), glm::vec2(sc::config.get("WINDOW_WIDTH") / 2, sc::config.get("WINDOW_HEIGHT") / 2), glm::vec2(640, 256), glm::vec2(320, 128), glm::vec4(0.0f, 0.0f, 5.0f, 1.0f));
		//pf.createUIText(ID("E_TEXT"), glm::vec2(sc::config.get("WINDOW_WIDTH") / 2, sc::config.get("WINDOW_HEIGHT") / 2), "There are things\nbeyond Hello World?\nDunno.\nBut multiple lines of varying size now fit well.\nHahaha!", glm::vec4(1.0, 1.0, 1.0f, 1.0), ID("FT_TEST"), TextHAlign::center, TextVAlign::middle, TextHAlign::center);
		//state->getComponent<DrawSprite>(ID("E_SPRITE"))->setLayer(0);
		//state->getComponent<DrawRectangle>(ID("E_RECTB"))->setLayer(0);
		//state->getComponent<DrawText>(ID("E_TEXT"))->setLayer(1);
	}

	bool Game::update()
	{
		input.update();

		for (auto it = state->naturePointers.begin(); it != state->naturePointers.end(); it++) 
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

		return false;
	}
}