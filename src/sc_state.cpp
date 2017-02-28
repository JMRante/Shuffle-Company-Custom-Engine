/*

    SHUFFLE COMPANY SOURCE CODE

    sc_state.cpp
    ------------------------------------



*/

#include "sc_state.h"

namespace sc
{
	State::State() {}

	void State::copy(State* otherState)
	{
		entities = otherState->entities;

		transformPool.copy(otherState->transformPool);
		cameraPool.copy(otherState->cameraPool);
		drawModelPool.copy(otherState->drawModelPool);
		drawRectanglePool.copy(otherState->drawRectanglePool);
		drawSpritePool.copy(otherState->drawSpritePool);
		drawTextPool.copy(otherState->drawTextPool);

		debugCameraPool.copy(otherState->debugCameraPool);

		stage = otherState->stage;
	}

	bool State::addEntity(ID id)
	{
		if (!entityExists(id))
		{
			entities.push_back(id);
			return true;
		}

		LOG_E << "Cannot add existing entity " << id.get();
		return false;
	}

	bool State::entityExists(ID id)
	{
		for (auto ei = entities.begin(); ei != entities.end(); ei++)
		{
			if (ei->is(id))
			{
				return true;
			}
		}

		return false;
	}

	bool State::removeEntity(ID id)
	{
		for (auto ei = entities.begin(); ei != entities.end(); ei++)
		{
			if (ei->is(id))
			{
				transformPool.remove(id);
				cameraPool.remove(id);
				drawModelPool.remove(id);
				drawRectanglePool.remove(id);
				drawSpritePool.remove(id);
				drawTextPool.remove(id);

				debugCameraPool.remove(id);

				entities.erase(ei);
				return true;
			}
		}

		LOG_E << "Cannot remove non-existing entity " << id.get();
		return false;
	}
}