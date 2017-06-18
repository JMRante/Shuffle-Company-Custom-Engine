/*

    SHUFFLE COMPANY SOURCE CODE

    sc_events.cpp
    ------------------------------------



*/

#include "sc_events.h"
#include "sc_game.h"
#include "sc_state.h"

namespace sc
{
	Event::Event(State* state) 
	{
		this->state = state;
	}


	/*
		SaveLevelEvent
						*/
	SaveLevelEvent::SaveLevelEvent(State* state, std::string levelFile) : Event(state)
	{
		this->levelFile = levelFile;
	}

	void SaveLevelEvent::happen()
	{
		Stage* stage = state->getComponent<Stage>(ID("E_STAGE"));
		stage->writeStageFile(levelFile);
	}


	/*
		LoadLevelEvent
						*/
	LoadLevelEvent::LoadLevelEvent(State* state, std::string levelFile) : Event(state)
	{
		this->levelFile = levelFile;
	}

	void LoadLevelEvent::happen()
	{
		Stage* stage = state->getComponent<Stage>(ID("E_STAGE"));
		stage->readStageFile(levelFile);
		stage->updateStageMesh();

		EditorOperationManager* eom = state->getComponent<EditorOperationManager>(ID("E_EDITOR"));
		eom->clearOperations();
	}


	/*
		LoadLevelEvent
						*/
	OpenLoadSelector::OpenLoadSelector(State* state) : Event(state) {}

	void OpenLoadSelector::happen()
	{

	}

	/*
		LoadLevelEvent
						*/
	CloseSelector::CloseSelector(State* state) : Event(state) {}

	void CloseSelector::happen()
	{

	}
}