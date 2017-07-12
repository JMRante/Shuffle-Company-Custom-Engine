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

		getFiles("Custom/Levels");

		//Create Editor Scene
		//Build elements
		pf.createStage("Custom/Levels/TestLevel.shuff");

		//pf.createDebugCamera(CTID("E_CAMERA"), glm::vec3(1.5f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.1f, 100.0f, 0.07f, 0.1f);
		pf.createEditor(CTID("E_EDITOR"));
		pf.createEditorCamera(CTID("E_CAMERA"), glm::vec3(0.0f, 12.0f, 10.0f), -60.0f, 0.1f, 100.0f, 6.25f, 0.625f);
		
		pf.createFramerateCounter(CTID("E_FRAMECOUNT"), glm::vec2(16, sc::config.get("WINDOW_HEIGHT") - 32), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), CTID("FT_MONO"));
		state->getComponent<DrawText>(CTID("E_FRAMECOUNT"))->setLayer(0);

		ID cursorID = pf.createCursor();
		state->getComponent<DrawSprite>(cursorID)->setLayer(100);

		Stage* stage = state->getComponent<Stage>(CTID("E_STAGE"));
		int stageWidth = stage->getWidth();
		int stageDepth = stage->getDepth();

		for (int i = 0; i < stageWidth; i++)
		{
			for (int j = 0; j < stageDepth; j++)
			{
				pf.createEditorSlot(RTID(std::string("E_EDITSLOT" + iToS(i) + "_" + iToS(j)).c_str()), glm::vec3((float) i, 0.05f, (float) j), i, j);
			}
		}

		pf.createEditorSpriteButton(CTID("E_SAVEBUTTON"), glm::vec2(config.get("WINDOW_WIDTH") - 96.0f, config.get("WINDOW_HEIGHT") - 48.0f), 1, glm::vec2(48.0f, 48.0f), CTID("SP_SAVEICON"), new SaveLevelEvent(state, "Custom/Levels/newFormat.shuff"));
		pf.createEditorSpriteButton(CTID("E_LOADBUTTON"), glm::vec2(config.get("WINDOW_WIDTH") - 48.0f, config.get("WINDOW_HEIGHT") - 48.0f), 1, glm::vec2(48.0f, 48.0f), CTID("SP_LOADICON"), new LoadLevelEvent(state, "Custom/Levels/newFormat.shuff"));

		// Transform* textFieldPanelTran = state->getComponent<Transform>(pf.createEditorPanel(CTID("E_FIELDPANEL"), glm::vec2(0.0f, config.get("WINDOW_HEIGHT") - 48.0f), 0, glm::vec2(720.0f, 48.0f)));
		// Transform* titleLabelTran = state->getComponent<Transform>(pf.createEditorLabel(CTID("E_TITLELABEL"), glm::vec2(16.0f, 18.0f), CTID("FT_EDITOR"), "Title:"));
		// Transform* titleFieldTran = state->getComponent<Transform>(pf.createEditorTextField(CTID("E_TITLEFIELD"), glm::vec2(state->getComponent<DrawText>(CTID("E_TITLELABEL"))->getWidth() + 26.0f, 10.0f), 1, 50, CTID("FT_EDITOR"), "My Stage", NULL, new SetStageTitle(state)));
		// titleLabelTran->setParent(textFieldPanelTran);
		// titleFieldTran->setParent(textFieldPanelTran);
		//pf.createFileSelector(CTID("E_FILESELECT"), "temp", new SaveLevelEvent(state, "Custom/Levels/newFormat.shuff"));

		//pf.createUISprite(CTID("E_SPRITE"), glm::vec2(256, 16), glm::vec2(1.0f, 1.0f), glm::vec2(0, 0), CTID("SP_TEST"));
		//pf.createUIRectangle(CTID("E_RECTB"), glm::vec2(sc::config.get("WINDOW_WIDTH") / 2, sc::config.get("WINDOW_HEIGHT") / 2), glm::vec2(640, 256), glm::vec2(320, 128), glm::vec4(0.0f, 0.0f, 5.0f, 1.0f));
		//pf.createUIText(CTID("E_TEXT"), glm::vec2(sc::config.get("WINDOW_WIDTH") / 2, sc::config.get("WINDOW_HEIGHT") / 2), "There are things\nbeyond Hello World?\nDunno.\nBut multiple lines of varying size now fit well.\nHahaha!", glm::vec4(1.0, 1.0, 1.0f, 1.0), CTID("FT_TEST"), TextHAlign::center, TextVAlign::middle, TextHAlign::center);
		//state->getComponent<DrawSprite>(CTID("E_SPRITE"))->setLayer(0);
		//state->getComponent<DrawRectangle>(CTID("E_RECTB"))->setLayer(0);
		//state->getComponent<DrawText>(CTID("E_TEXT"))->setLayer(1);
	}

	bool Game::update()
	{
		input.update();

		for (auto it = state->naturePointers.begin(); it != state->naturePointers.end(); it++) 
		{
			if ((*it)->getActive())
			{
				(*it)->update(); 
			}
		}

		for (auto it = state->dirtyTransforms.begin(); it != state->dirtyTransforms.end(); it++) 
		{
			(*it)->calculate();
		}

		if (input.quitEvent())
		{
			return true;
		}

		return false;
	}
}